# Garbage Collection Deep Dive

> *GC knowledge is essential for production debugging and performance tuning. Let's explore garbage collection through the story of objects living and dying in the JVM...*

---

## 🎬 The Problem: A World Without GC

Imagine you're a C programmer managing memory manually:

```c
User* user = malloc(sizeof(User));  // Allocate memory
// ... use user ...
free(user);                          // Must remember to free!
user->name;                          // 💥 CRASH! Dangling pointer!

// Or worse...
free(user);
free(user);                          // 💥 Double free!
```

**The Three Nightmares of Manual Memory Management:**

| Problem | What Happens | Real Impact |
|---------|--------------|-------------|
| **Memory Leak** | Forgot to `free()` | App slowly consumes all RAM |
| **Dangling Pointer** | Used after `free()` | Random crashes, security holes |
| **Double Free** | `free()` called twice | Heap corruption, exploits |

**Java's Solution**: The Garbage Collector automatically reclaims memory from objects you're no longer using. No `free()`, no nightmares!

---

## 📖 Chapter 1: The Generational Hypothesis (The Foundation of All Modern GC)

**The Story**: Imagine a hospital nursery. Most babies (objects) leave quickly—they were just visiting. But some stay for years (long-term patients). Would you check on every patient equally often?

> 💡 **"Most objects die young."**

### The Evidence

```java
public void processRequest(Request req) {
    // These objects are born...
    String temp = req.getHeader("Content-Type");    // 🐣 Born
    byte[] buffer = new byte[1024];                  // 🐣 Born
    Response resp = buildResponse(req);              // 🐣 Born
    
    return resp;
}   // ...temp and buffer die here (method returns) ⚰️

// But these live forever:
private static final Logger log = Logger.getLogger(...);  // 🦖 Immortal
private static final Cache<K,V> cache = ...;              // 🦖 Immortal
```

### The Number That Changes Everything

```
┌────────────────────────────────────────────────────────────────┐
│              Object Lifespan Distribution                      │
│                                                                │
│   |                                                            │
│ N |█████████████████████████████████████████                   │
│ u |██████████████████                                          │
│ m |█████████                                                   │
│ b |████                                                        │
│ e |██                                                          │
│ r |█                      █  █  █                              │
│   └────────────────────────────────────────────────────────→   │
│        Short-lived (~95%)                Long-lived (~5%)      │
│         (milliseconds)                    (app lifetime)       │
└────────────────────────────────────────────────────────────────┘
```

**The Insight**: Don't waste time scanning long-lived objects repeatedly. Focus GC effort on the nursery!

---

## 📖 Chapter 2: The Life and Death of an Object

**The Story**: Objects in the JVM live in a city with two neighborhoods: the **Nursery** (Young Generation) and the **Retirement Community** (Old Generation). Let's follow an object's journey.

### The Heap Layout

```
┌──────────────────────────────────────────────────────────────────────────┐
│                              THE HEAP                                     │
│                                                                          │
│  ┌─────────────────────────────────────────┐  ┌────────────────────────┐ │
│  │          YOUNG GENERATION               │  │    OLD GENERATION      │ │
│  │          (The Nursery)                  │  │  (Retirement Community)│ │
│  │                                         │  │                        │ │
│  │  ┌───────────────┐  ┌──────┐  ┌──────┐  │  │                        │ │
│  │  │     EDEN      │  │  S0  │  │  S1  │  │  │   Survivors who made   │ │
│  │  │               │  │      │  │      │  │  │   it past 15 GC cycles │ │
│  │  │  🐣 🐣 🐣 🐣   │  │  🐥  │  │      │  │  │                        │ │
│  │  │  new objects  │  │age:3 │  │empty │  │  │   🦖 🦖 🦖              │ │
│  │  │  born here    │  │      │  │      │  │  │   Caches, Singletons   │ │
│  │  └───────────────┘  └──────┘  └──────┘  │  │                        │ │
│  │                                         │  │                        │ │
│  │  Minor GC: Fast, frequent (~10-50ms)    │  │  Major GC: Slow, rare  │ │
│  └─────────────────────────────────────────┘  └────────────────────────┘ │
│                                                                          │
└──────────────────────────────────────────────────────────────────────────┘
```

### Object's Journey (The Survivor Game)

```
Day 1: Object is born in Eden                    🐣 age=0
       │
       ▼ (Eden fills up → Minor GC)
       
Day 2: Survived! Moved to Survivor S0           🐥 age=1
       │
       ▼ (Another Minor GC)
       
Day 3: Still alive! Moved to S1                 🐥 age=2
       │                    (S0 and S1 swap roles each time)
       ▼ 
       
Day 4-15: Keeps surviving, bouncing between S0/S1...  age=3,4,5...14
       │
       ▼ (age >= 15: Tenuring Threshold)
       
Day 16: Promoted to Old Generation!             🦖 age=15+
        (Now only cleaned in Major GC)
```

**Code that creates this pattern:**

```java
// Short-lived (dies in nursery) - GOOD
for (Request req : requests) {
    String temp = process(req);  // Born, used, dies quickly
}

// Long-lived (survives to Old Gen) - OK if intentional
private static final Map<String, User> userCache = new ConcurrentHashMap<>();
```

---

## 📖 Chapter 3: The Detectives (GC Roots & Reachability)

**The Story**: How does GC know what's garbage? It works like detectives following a trail. If you can't be reached from any "VIP" (GC Root), you're garbage.

### The VIP List (GC Roots)

```
┌────────────────────────────────────────────────────────────────────────┐
│                         GC ROOTS (The VIPs)                             │
├────────────────────────────────────────────────────────────────────────┤
│                                                                        │
│  👔 Local Variables           "I'm on a thread's stack right now"      │
│     └─ Variables in active method calls                                │
│                                                                        │
│  🏛️ Static Fields             "I'm attached to a class forever"        │
│     └─ MyClass.staticField                                             │
│                                                                        │
│  🧵 Active Threads            "The thread object itself"               │
│     └─ Thread.currentThread()                                          │
│                                                                        │
│  🔗 JNI References            "Native code is holding me"              │
│     └─ Objects passed to/from native methods                           │
│                                                                        │
│  🔒 Synchronization Monitors  "Someone is synchronized on me"          │
│     └─ Objects used in synchronized blocks                             │
│                                                                        │
└────────────────────────────────────────────────────────────────────────┘
```

### The Detective Work (Mark and Sweep)

```java
class App {
    static Database db = new Database();     // GC Root: static field
    
    void process() {
        User user = new User("Alice");       // GC Root: local variable
        Order order = new Order(user);       // Reachable: user→order
        
        Cache orphan = new Cache();          // ⚠️ No reference stored!
    }   // After return: user and order are NOT roots anymore
}
```

**The Mark Phase (Following the Trail):**

```
        GC ROOTS                         HEAP
        ════════                    ═══════════════
                                   
    ┌─ App.db ──────────────────▶  [Database] ✓ LIVE
    │                                   │
    │                                   ▼
    │                              [ConnectionPool] ✓ LIVE
    │
    ├─ user (local) ────────────▶  [User "Alice"] ✓ LIVE
    │                                   │
    │                                   ▼
    │                              [Order] ✓ LIVE
    │
    └─ (nothing points to) ───✗    [Cache orphan] ✗ GARBAGE!
```

**The Sweep Phase:**

```
Before:  [Database][Cache][User][Order][garbage][garbage]
After:   [Database][     ][User][Order]                    ← Memory freed!
```

---

## 📖 Chapter 4: The Cleanup Crews (GC Algorithms)

**The Story**: Different situations need different cleanup strategies. A food truck (small app) needs different cleaning than a football stadium (enterprise app).

### The Collector Roster

```
┌─────────────────────────────────────────────────────────────────────────┐
│                    🧹 GARBAGE COLLECTOR LINEUP                          │
├─────────────────────────────────────────────────────────────────────────┤
│                                                                         │
│  ┌───────────────┐   SERIAL GC                                          │
│  │  👤           │   "One janitor, small office"                        │
│  │  ════════     │   • Single-threaded                                  │
│  │  stop...clean │   • Stop-the-world for everything                    │
│  └───────────────┘   • Use: Small apps, single CPU, client JVMs         │
│                      • Flag: -XX:+UseSerialGC                           │
│                                                                         │
│  ┌───────────────┐   PARALLEL GC                                        │
│  │  👥👥👥       │   "Cleaning crew, maximize throughput"               │
│  │  ════════     │   • Multiple threads for Young Gen                   │
│  │  STOP → CLEAN │   • Still stop-the-world, but faster                 │
│  └───────────────┘   • Use: Batch jobs, background processing           │
│                      • Flag: -XX:+UseParallelGC                         │
│                                                                         │
│  ┌───────────────┐   G1 GC (Default since Java 9)                       │
│  │  👥 │ 🏃‍♂️💨   │   "Smart cleaners with time budget"                  │
│  │  ───┼─────    │   • Heap divided into regions                        │
│  │  GC │ App     │   • Targets pause time: -XX:MaxGCPauseMillis=200     │
│  └───────────────┘   • Use: General purpose, most production apps       │
│                      • Flag: -XX:+UseG1GC                               │
│                                                                         │
│  ┌───────────────┐   ZGC (Java 11+)                                     │
│  │  👥   🏃‍♂️🏃‍♀️💨 │   "Concurrent ninjas, <1ms pauses!"                 │
│  │  ────────────  │   • Almost everything concurrent                    │
│  │  GC overlaps   │   • Colored pointers for magic                      │
│  └───────────────┘   • Use: Low-latency (trading, gaming, real-time)    │
│                      • Flag: -XX:+UseZGC                                │
│                                                                         │
└─────────────────────────────────────────────────────────────────────────┘
```

### G1 GC: The Smart One (Deep Dive)

**The Problem G1 Solves**: "I want predictable pauses, not 5-second freezes!"

```
Traditional Heap:                G1 Heap (Regions):
┌────────────────────────┐      ┌───┬───┬───┬───┬───┬───┬───┬───┐
│  Young  │     Old      │      │ E │ E │ S │ O │ O │ O │ H │   │
│  ████   │  ████████    │      ├───┼───┼───┼───┼───┼───┼───┼───┤
│ (fixed) │   (fixed)    │      │ O │ E │ O │ O │ O │ O │ E │ O │
└────────────────────────┘      └───┴───┴───┴───┴───┴───┴───┴───┘
                                 E=Eden  S=Survivor  O=Old  H=Humongous
                                 (Regions can change roles!)
```

**How G1 Achieves Predictable Pauses:**

```
G1 tracks: "Region A has 80% garbage, Region B has 20%"

With 200ms budget:
┌──────────────────────────────────────────────────────────────┐
│  "I can only clean for 200ms..."                             │
│                                                              │
│  Region A: 80% garbage  ──▶  CLEAN THIS FIRST! (high ROI)    │
│  Region B: 20% garbage  ──▶  Skip for now                    │
│  Region C: 90% garbage  ──▶  CLEAN THIS TOO!                 │
│  Region D: 10% garbage  ──▶  Skip                            │
│                                                              │
│  "200ms up! Collected 70% of garbage, app resumes."          │
└──────────────────────────────────────────────────────────────┘
```

### ZGC: The Speed Demon (Java 11+)

**The Magic**: Pauses under 1 millisecond, even with terabyte heaps!

```
Traditional GC:         App: ████████░░░░░░░░████████░░░░░████████
                        GC:          ████████        ████
                                     STOP!           STOP!
                        
ZGC:                    App: ████████████████████████████████████
                        GC:  ░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░
                             (runs concurrently, <1ms pauses)
```

**How? Colored Pointers (Simplified):**

```java
// Normal pointer: just an address
pointer = 0x00000000_12345678
                     ^^^^^^^^
                     address bits

// ZGC colored pointer: address + metadata
pointer = 0x00010000_12345678
          ^^^^
          color bits: "marked", "remapped", etc.
```

### ZGC Generational Mode (Java 21+)

```bash
# Enable for best of both worlds
java -XX:+UseZGC -XX:+ZGenerational MyApp

# Benefits:
# ✅ Sub-millisecond pauses (ZGC magic)
# ✅ Generational efficiency (most objects die young)
# ✅ Lower memory overhead
# ✅ Default in Java 23!
```

---

## 📖 Chapter 5: The Control Panel (GC Tuning)

**The Story**: You're the engineer. Here's your control panel.

### Memory Sizing Knobs

```bash
# The Big Three
-Xms4g          # Starting heap (avoid resize pauses: set = Xmx)
-Xmx4g          # Maximum heap (OOM if exceeded)
-Xmn1g          # Young generation size

# Metaspace (class metadata)
-XX:MetaspaceSize=256m
-XX:MaxMetaspaceSize=512m
```

### GC Selection

```
┌─────────────────────────────────────────────────────────────────────────┐
│  When to use which collector?                                           │
├─────────────────────────────────────────────────────────────────────────┤
│                                                                         │
│  "Small app, single core, limited memory"                               │
│        └─▶  -XX:+UseSerialGC                                            │
│                                                                         │
│  "Batch job, I want maximum throughput, pauses don't matter"            │
│        └─▶  -XX:+UseParallelGC                                          │
│                                                                         │
│  "General purpose, I want balanced pauses (< 200ms)"                    │
│        └─▶  -XX:+UseG1GC  (default since Java 9)                        │
│             -XX:MaxGCPauseMillis=200                                    │
│                                                                         │
│  "Ultra-low latency, I CANNOT have pauses > 1ms"                        │
│        └─▶  -XX:+UseZGC (Java 11+)                                      │
│             -XX:+ZGenerational (Java 21+, even better)                  │
│                                                                         │
└─────────────────────────────────────────────────────────────────────────┘
```

### Production-Ready Command

```bash
java -Xms8g -Xmx8g \
     -XX:+UseG1GC \
     -XX:MaxGCPauseMillis=200 \
     -XX:+HeapDumpOnOutOfMemoryError \
     -XX:HeapDumpPath=/var/dumps/ \
     -Xlog:gc*:file=/var/log/gc.log:time,level,tags \
     -jar myapp.jar
```

---

## 📖 Chapter 6: The Leak Detective (Finding Memory Leaks)

**The Story**: "My app is slow and keeps getting OOM errors!" Time to investigate.

### The Usual Suspects

```java
// 🚨 SUSPECT #1: Static Collection That Never Stops Growing
public class EventLogger {
    private static List<Event> allEvents = new ArrayList<>();  // ⚠️ LEAK!
    
    public void log(Event e) {
        allEvents.add(e);  // Grows forever, never cleaned!
    }
}
// Fix: Use bounded cache like Guava's Cache or Caffeine

// 🚨 SUSPECT #2: Unclosed Resources
public void readFile(String path) throws IOException {
    InputStream is = new FileInputStream(path);  // ⚠️ Never closed!
    // If exception happens, stream stays open forever
}
// Fix: try-with-resources
try (InputStream is = new FileInputStream(path)) {
    // ...
}

// 🚨 SUSPECT #3: ThreadLocal Not Removed
public class RequestContext {
    private static ThreadLocal<User> currentUser = new ThreadLocal<>();
    
    public void setUser(User u) { currentUser.set(u); }
    public User getUser() { return currentUser.get(); }
    // ⚠️ In thread pools, thread is reused but ThreadLocal stays!
}
// Fix: 
public void clearUser() { currentUser.remove(); }  // Call after request!

// 🚨 SUSPECT #4: Non-Static Inner Class
public class Outer {
    private byte[] hugeData = new byte[10_000_000];
    
    public class Inner {  // ⚠️ Holds implicit reference to Outer!
        void doSomething() { }
    }
}
// Fix: Make inner class static if it doesn't need outer reference
public static class Inner { }
```

### The Investigation Tools

```bash
# Take heap dump of running JVM
jmap -dump:live,format=b,file=heap.hprof <pid>

# Or use jcmd (preferred)
jcmd <pid> GC.heap_dump heap.hprof

# Then analyze with Eclipse MAT or VisualVM
# Look for:
#   - "Leak Suspects" report
#   - Objects with many instances
#   - Dominator tree (what's holding memory)
```

---

## 📖 Chapter 7: The Troubleshooting Guide

### Problem: High GC Frequency (Minor GC every second)

```
Symptom:  GC logs show constant Minor GC, high CPU
Cause:    Object churn - creating too many short-lived objects

┌─ Common Code Patterns ──────────────────────────────────────┐
│                                                             │
│  // Bad: Creating strings in hot loop                       │
│  for (int i = 0; i < 1_000_000; i++) {                      │
│      String s = "prefix" + i + "suffix";  // 3 objects!     │
│  }                                                          │
│                                                             │
│  // Better: StringBuilder                                   │
│  StringBuilder sb = new StringBuilder();                    │
│  for (int i = 0; i < 1_000_000; i++) {                      │
│      sb.setLength(0);                                       │
│      sb.append("prefix").append(i).append("suffix");        │
│  }                                                          │
│                                                             │
└─────────────────────────────────────────────────────────────┘
```

### Problem: Long GC Pauses (Application Freezes)

```
Symptom:  Users report app hangs for seconds
Cause:    Full GC on large Old Generation

Solutions:
┌─────────────────────────────────────────────────────────────────────┐
│  1. Increase heap if genuinely needed                               │
│     -Xmx8g → -Xmx16g                                                │
│                                                                     │
│  2. Switch to low-latency collector                                 │
│     -XX:+UseG1GC -XX:MaxGCPauseMillis=100                           │
│     or -XX:+UseZGC                                                  │
│                                                                     │
│  3. Fix memory leaks causing Old Gen growth                         │
│     Take heap dump → Analyze with MAT                               │
│                                                                     │
│  4. Tune generation sizes                                           │
│     -XX:NewRatio=2  (Old = 2x Young)                                │
└─────────────────────────────────────────────────────────────────────┘
```

### Problem: OutOfMemoryError

| Error Message | Meaning | Solution |
|--------------|---------|----------|
| `Java heap space` | Heap exhausted | Increase `-Xmx`, fix memory leak |
| `Metaspace` | Too many classes | Increase `-XX:MaxMetaspaceSize`, check classloader leaks |
| `GC overhead limit exceeded` | 98% time in GC, 2% freed | Memory leak, needs investigation |
| `Unable to create native thread` | OS thread limit | Reduce thread count, increase OS limits |

---

## 📖 Chapter 8: The Grand Quiz (Interview Questions)

### Q: What triggers Minor GC vs Major GC?

```
┌────────────────────┬─────────────────────────────────────────────────┐
│ Minor GC           │ Major/Full GC                                   │
├────────────────────┼─────────────────────────────────────────────────┤
│ Eden space full    │ Old Gen full                                    │
│ Fast (10-50ms)     │ Slow (100ms - seconds)                          │
│ Young Gen only     │ Entire heap                                     │
│ Frequent           │ Should be rare                                  │
│                    │ Also: System.gc(), Metaspace pressure           │
└────────────────────┴─────────────────────────────────────────────────┘
```

### Q: How does G1 achieve predictable pauses?

```
1. Divides heap into ~2000 equal regions (not fixed generations)
2. Tracks "garbage density" per region
3. Given pause target (e.g., 200ms):
   - Sorts regions by garbage amount
   - Collects highest-garbage regions first
   - Stops when time budget exhausted
4. Result: Predictable pauses, not necessarily shortest
```

### Q: What is "Stop-the-World"?

```
ALL application threads pause while GC works.

             App Threads                GC Threads
              ████████                    ░░░░░░
              ████████                    ░░░░░░
STW start ─▶  ░░░░░░░░░░░░░░░░░░░░░░░░░░░░███████
              ░░░░░░░░░░░░░░░░░░░░░░░░░░░░███████
STW end   ─▶  ████████████████████████████░░░░░░
              ████████████████████████████░░░░░░

Why needed? GC must see consistent heap state while marking/moving objects.
Modern GCs minimize STW (G1, ZGC do most work concurrently).
```

### Q: Difference between `System.gc()` and `Runtime.gc()`?

```java
System.gc();                        // Calls...
Runtime.getRuntime().gc();          // ...this

// Both are HINTS to the JVM
// JVM can (and often does) ignore them
// Never rely on these in production code!
```

---

## 🎯 Quick Reference Card

```
┌─────────────────────────────────────────────────────────────────────┐
│                   GC QUICK REFERENCE                                │
├────────────────────┬────────────────────────────────────────────────┤
│ Serial             │ -XX:+UseSerialGC       (small apps)            │
│ Parallel           │ -XX:+UseParallelGC     (throughput)            │
│ G1 (default)       │ -XX:+UseG1GC           (balanced)              │
│ ZGC                │ -XX:+UseZGC            (low latency)           │
├────────────────────┼────────────────────────────────────────────────┤
│ Heap size          │ -Xms / -Xmx                                    │
│ Young Gen          │ -Xmn or -XX:NewRatio                           │
│ Metaspace          │ -XX:MaxMetaspaceSize                           │
│ Pause target (G1)  │ -XX:MaxGCPauseMillis=200                       │
├────────────────────┼────────────────────────────────────────────────┤
│ Heap dump          │ jcmd <pid> GC.heap_dump file.hprof             │
│ GC logging         │ -Xlog:gc*:file=gc.log:time,level,tags          │
└────────────────────┴────────────────────────────────────────────────┘
```
