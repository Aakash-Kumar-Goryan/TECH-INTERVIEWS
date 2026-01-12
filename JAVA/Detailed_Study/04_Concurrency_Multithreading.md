# Concurrency & Multi-threading

> *Concurrency is the art of juggling multiple tasks. Let's explore threads through the story of workers in a factory...*

---

## 🎬 The Factory Floor

Imagine a factory where each **thread** is a worker. They share the same workspace (memory) and tools (resources). The challenge? Coordination!

```
┌─────────────────────────────────────────────────────────────────────────┐
│                        🏭 THE JVM FACTORY                               │
│                                                                         │
│  Worker 1 (Thread)    Worker 2 (Thread)    Worker 3 (Thread)           │
│      👷                    👷                    👷                     │
│       │                     │                     │                     │
│       ▼                     ▼                     ▼                     │
│  ┌─────────────────────────────────────────────────────────────────┐   │
│  │                    SHARED WORKSPACE (Heap)                       │   │
│  │                                                                  │   │
│  │   [Counter: 42]    [UserList]    [Cache]    [DatabaseConnection] │   │
│  │                                                                  │   │
│  └─────────────────────────────────────────────────────────────────┘   │
│                                                                         │
│  ⚠️ Problem: Two workers grabbing the same tool at once = CHAOS!      │
└─────────────────────────────────────────────────────────────────────────┘
```

---

## 📖 Chapter 1: Hiring Workers (Creating Threads)

### Three Ways to Hire

```java
// Method 1: The Full-Timer (extends Thread)
class FullTimeWorker extends Thread {
    @Override
    public void run() {
        System.out.println("Working hard!");
    }
}
new FullTimeWorker().start();

// Method 2: The Contractor (Runnable) - PREFERRED
Runnable task = () -> System.out.println("Got the job done!");
new Thread(task).start();

// Method 3: The Consultant (Callable) - Returns a result
Callable<Integer> billableWork = () -> {
    // Complex calculation
    return 42;
};
```

### A Worker's Life Cycle

```
┌─────────┐   start()   ┌───────────┐
│   NEW   │ ──────────▶ │ RUNNABLE  │ ◀────────────────────────┐
│(Created)│             │(Ready/Run)│                          │
└─────────┘             └─────┬─────┘                          │
                              │                                │
          ┌───────────────────┼───────────────────┐            │
          ▼                   ▼                   ▼            │
   ┌──────────────┐   ┌──────────────┐   ┌──────────────────┐  │
   │   BLOCKED    │   │   WAITING    │   │  TIMED_WAITING   │  │
   │ (wants lock) │   │ (wait/join)  │   │  (sleep/timeout) │  │
   └──────┬───────┘   └──────┬───────┘   └────────┬─────────┘  │
          │                  │                    │            │
          └──────────────────┴────────────────────┴────────────┘
                              │
                              ▼
                      ┌──────────────┐
                      │  TERMINATED  │
                      │  (finished)  │
                      └──────────────┘
```

| State | What's Happening | Example |
|-------|-----------------|---------|
| **NEW** | Born, not started | `new Thread()` |
| **RUNNABLE** | Running or in queue | After `start()` |
| **BLOCKED** | Wants lock someone else has | Waiting for `synchronized` |
| **WAITING** | Waiting indefinitely | `wait()`, `join()` |
| **TIMED_WAITING** | Waiting with timeout | `sleep(1000)` |
| **TERMINATED** | Work complete or crashed | After `run()` returns |

---

## 📖 Chapter 2: The Bathroom Problem (Synchronization)

**The Story**: Only one person can use the bathroom at a time. The lock on the door ensures this.

### The synchronized Lock

```java
public class Bathroom {
    // Method 1: Lock the entire bathroom (method-level)
    public synchronized void use() {
        // Only one thread at a time!
        System.out.println("Using bathroom...");
    }
    
    // Method 2: Lock just the toilet (block-level)
    private final Object toiletLock = new Object();
    
    public void useToilet() {
        synchronized (toiletLock) {
            // Only this part is locked
        }
        // Other code runs freely
    }
}
```

### What synchronized Actually Does

```
Thread 1:                         Thread 2:
─────────                         ─────────

synchronized (lock) {             synchronized (lock) {
    // I got the lock! 🔑             // Is lock free?
    count++;                          // No! 😤 Block...
}                                     // Still waiting...
// Released lock! 🔓                  // ...
                                      // Lock free! 🔑 My turn!
                                      count++;
                                  }
```

### The infamous count++ Problem

```java
// This looks atomic, but it's NOT!
count++;

// What really happens:
// 1. READ count (= 5)
// 2. ADD 1 (= 6)
// 3. WRITE count (= 6)

// If two threads do this simultaneously:
Thread 1: READ 5 → ADD → 6 → WRITE 6
Thread 2: READ 5 → ADD → 6 → WRITE 6

// Expected: 7, Actual: 6! 💥 Lost update!
```

### volatile: The Visibility Guarantee

```java
volatile boolean running = true;

// Thread 1 (Worker)
while (running) {  // Always reads fresh value from main memory
    doWork();
}

// Thread 2 (Controller)
running = false;   // Immediately visible to Thread 1

// ✅ This works! Thread 1 stops.
// Without volatile, Thread 1 might read cached 'true' forever!
```

**volatile Provides**:

- ✅ Visibility (writes immediately visible to all threads)
- ✅ Ordering (prevents reordering around volatile)
- ❌ Atomicity (`count++` still broken even with volatile!)

---

## 📖 Chapter 3: Better Locks (java.util.concurrent.locks)

**The Story**: The bathroom lock is simple but limited. What if you want to knock and leave if occupied? Or wait only 5 seconds?

### ReentrantLock: The Smart Lock

```java
ReentrantLock lock = new ReentrantLock();

// Basic usage (ALWAYS unlock in finally!)
lock.lock();
try {
    // Critical section
} finally {
    lock.unlock();  // ← NEVER forget this!
}

// Try to get lock, don't block
if (lock.tryLock()) {
    try {
        // Got it!
    } finally {
        lock.unlock();
    }
} else {
    // Occupied, do something else
}

// Wait with timeout
if (lock.tryLock(5, TimeUnit.SECONDS)) {
    // Got lock within 5 seconds
}
```

### synchronized vs ReentrantLock

```
┌────────────────────────┬───────────────────────────────────────────────┐
│    synchronized        │         ReentrantLock                         │
├────────────────────────┼───────────────────────────────────────────────┤
│ Auto unlock (even      │ Must call unlock() in finally                 │
│ on exception)          │ (you can forget = disaster!)                  │
├────────────────────────┼───────────────────────────────────────────────┤
│ No timeout             │ tryLock(timeout) ✅                           │
├────────────────────────┼───────────────────────────────────────────────┤
│ No interruptible wait  │ lockInterruptibly() ✅                        │
├────────────────────────┼───────────────────────────────────────────────┤
│ One wait set           │ Multiple Conditions ✅                        │
├────────────────────────┼───────────────────────────────────────────────┤
│ Unfair by default      │ Fair option: new ReentrantLock(true) ✅       │
└────────────────────────┴───────────────────────────────────────────────┘
```

### ReadWriteLock: The Library Card

```
Scenario: Many readers, rare writers

┌─────────────────────────────────────────────────────────────────────────┐
│                      📚 THE LIBRARY                                     │
│                                                                         │
│   Regular Lock:                ReadWriteLock:                          │
│   ┌─────────────┐              ┌─────────────┐                         │
│   │  🔒 LOCKED  │              │ 📖 READERS  │ (many can read)         │
│   │ One person! │              │  👤👤👤👤   │                         │
│   └─────────────┘              └─────────────┘                         │
│                                ┌─────────────┐                         │
│                                │ ✏️ WRITER   │ (only one, blocks all) │
│                                │     👤      │                         │
│                                └─────────────┘                         │
└─────────────────────────────────────────────────────────────────────────┘
```

```java
ReadWriteLock rwLock = new ReentrantReadWriteLock();

// Reading (many can do this simultaneously)
rwLock.readLock().lock();
try {
    return data.get(key);
} finally {
    rwLock.readLock().unlock();
}

// Writing (exclusive access)
rwLock.writeLock().lock();
try {
    data.put(key, value);
} finally {
    rwLock.writeLock().unlock();
}
```

### StampedLock: The Optimistic Reader (Java 8+)

```java
StampedLock lock = new StampedLock();

// Optimistic read: "I'll read without locking, then verify"
long stamp = lock.tryOptimisticRead();  // No actual lock!
int x = this.x;
int y = this.y;

// Did anyone write while I was reading?
if (!lock.validate(stamp)) {
    // Oops! Someone wrote. Get a real lock.
    stamp = lock.readLock();
    try {
        x = this.x;
        y = this.y;
    } finally {
        lock.unlockRead(stamp);
    }
}
// Most of the time, validation passes = FREE performance!
```

---

## 📖 Chapter 4: The Waiting Room (Wait/Notify & Conditions)

**The Story**: Workers sometimes need to wait for conditions. "Wait here until the parts arrive!"

### Old School: wait() / notify()

```java
class Warehouse {
    private Queue<Item> items = new LinkedList<>();
    private final Object lock = new Object();
    
    // Producer
    public void produce(Item item) {
        synchronized (lock) {
            items.add(item);
            lock.notifyAll();  // "Hey waiters, something's here!"
        }
    }
    
    // Consumer
    public Item consume() throws InterruptedException {
        synchronized (lock) {
            while (items.isEmpty()) {  // ALWAYS use while, not if!
                lock.wait();  // "I'll wait here... 😴"
            }
            return items.remove();
        }
    }
}
```

### Modern: Condition Objects

```java
class BetterWarehouse {
    private Queue<Item> items = new LinkedList<>();
    private ReentrantLock lock = new ReentrantLock();
    private Condition notEmpty = lock.newCondition();  // Separate waiting room!
    private Condition notFull = lock.newCondition();   // Another waiting room!
    
    public void produce(Item item) throws InterruptedException {
        lock.lock();
        try {
            while (isFull()) {
                notFull.await();  // Wait in "notFull" room
            }
            items.add(item);
            notEmpty.signal();  // Wake someone in "notEmpty" room
        } finally {
            lock.unlock();
        }
    }
}
```

---

## 📖 Chapter 5: The Worker Pool (Executors)

**The Story**: Hiring a new worker for every task is expensive. Keep a pool of workers ready!

### Why Thread Pools?

```
Without Pool:                    With Pool:
─────────────                    ──────────

Task 1 → Create Thread 💰        Task 1 ─┐
Task 2 → Create Thread 💰        Task 2 ─┼──▶  [Worker Pool]  ──▶ Results
Task 3 → Create Thread 💰        Task 3 ─┘        👷👷👷👷
   ...                                      (reused workers!)

1000 tasks = 1000 threads! 😱    1000 tasks = 10 threads 🎉
```

### The Pool Types

```java
// Fixed Pool: Exactly N workers
ExecutorService fixed = Executors.newFixedThreadPool(10);
// Use when: You know the optimal thread count

// Cached Pool: Grows as needed, shrinks when idle
ExecutorService cached = Executors.newCachedThreadPool();
// Use when: Many short tasks, bursty load

// Single Thread: One worker, tasks run in order
ExecutorService single = Executors.newSingleThreadExecutor();
// Use when: Tasks must run sequentially

// Scheduled: For delayed or periodic tasks
ScheduledExecutorService scheduled = Executors.newScheduledThreadPool(5);
scheduled.scheduleAtFixedRate(task, 0, 1, TimeUnit.SECONDS);
```

### The Pool Internals

```
ThreadPoolExecutor(
    corePoolSize,     // Always keep this many workers
    maxPoolSize,      // Can grow to this many
    keepAliveTime,    // Idle workers go home after this
    TimeUnit,
    workQueue         // Where tasks wait
)

Task Submitted:
       │
       ▼
┌──────────────────┐
│ Core workers     │ ← If available, use immediately
│ busy?            │
└────────┬─────────┘
         │ Yes
         ▼
┌──────────────────┐
│ Queue has        │ ← If queue has space, enqueue
│ space?           │
└────────┬─────────┘
         │ No
         ▼
┌──────────────────┐
│ Under maxPool?   │ ← If under max, hire temporary worker
└────────┬─────────┘
         │ No
         ▼
┌──────────────────┐
│ Rejection Policy │ ← Throw exception / caller runs / discard
└──────────────────┘
```

### When the Queue is Full: Rejection Policies

| Policy | What Happens |
|--------|--------------|
| `AbortPolicy` | 💥 Throw `RejectedExecutionException` |
| `CallerRunsPolicy` | 🔄 Submitter runs the task themselves |
| `DiscardPolicy` | 🗑️ Silently drop the task |
| `DiscardOldestPolicy` | 🔄 Drop oldest, retry new |

---

## 📖 Chapter 6: The Promise (CompletableFuture)

**The Story**: "Go do this in the background. When done, do that. If it fails, handle it."

### The Old Way (Blocking)

```java
// Thread waits doing nothing! 😴
Future<User> future = executor.submit(() -> fetchUser());
User user = future.get();  // BLOCKED until complete!
```

### The New Way (Non-Blocking)

```java
CompletableFuture.supplyAsync(() -> fetchUser())
    .thenApply(user -> fetchOrders(user))       // When done, do this
    .thenApply(orders -> calculateTotal(orders))
    .thenAccept(total -> display(total))         // Final step
    .exceptionally(ex -> {                       // If anything fails
        log.error("Oops!", ex);
        return null;
    });

// Main thread continues immediately! 🏃
```

### The Pipeline Visualized

```
fetchUser()  ───▶  fetchOrders()  ───▶  calculateTotal()  ───▶  display()
    │                   │                      │                    │
    └──────────────────┴──────────────────────┴────────────────────┘
                         Non-blocking pipeline!
                         
                         Error at any step?
                              │
                              ▼
                         exceptionally() handles it
```

### Combining Futures

```java
// Wait for BOTH to complete, combine results
CompletableFuture<User> userFuture = fetchUserAsync();
CompletableFuture<List<Order>> ordersFuture = fetchOrdersAsync();

userFuture.thenCombine(ordersFuture, (user, orders) -> {
    return new Dashboard(user, orders);
});

// Wait for ALL to complete
CompletableFuture.allOf(f1, f2, f3).thenRun(() -> {
    System.out.println("All done!");
});

// Wait for FIRST to complete (race)
CompletableFuture.anyOf(f1, f2, f3).thenAccept(first -> {
    System.out.println("Got first result: " + first);
});
```

---

## 📖 Chapter 7: Lock-Free Magic (Atomic Classes)

**The Story**: What if we could update values without locks? Enter CAS!

### Compare-And-Swap (CAS)

```
"Change the value ONLY if it's still what I expect"

Thread 1: "If counter is 5, set it to 6"
Thread 2: "If counter is 5, set it to 6"

CPU executes atomically:
┌─────────────────────────────────────────────────────────────────────────┐
│  Thread 1: CAS(expected=5, new=6) → Success! ✅ Counter is now 6       │
│  Thread 2: CAS(expected=5, new=6) → Fail! ❌ Counter is 6, not 5       │
│            (Thread 2 retries with expected=6)                           │
└─────────────────────────────────────────────────────────────────────────┘
```

### AtomicInteger in Action

```java
AtomicInteger counter = new AtomicInteger(0);

// Thread-safe increment (no locks!)
counter.incrementAndGet();  // ++counter
counter.getAndIncrement();  // counter++

// Atomic update with function
counter.updateAndGet(x -> x * 2);

// CAS directly
boolean success = counter.compareAndSet(5, 6);
```

### When to Use What

```
┌──────────────────────────────────────────────────────────────────────┐
│  Contention Level:   LOW              MEDIUM              HIGH       │
├──────────────────────────────────────────────────────────────────────┤
│                        │                 │                 │        │
│  AtomicInteger ◀───────┴─────────────────┘                 │        │
│  (works great for low-medium contention)                   │        │
│                                                            ▼        │
│                                                      LongAdder     │
│                                        (shards counter, then sums) │
└──────────────────────────────────────────────────────────────────────┘
```

### VarHandle: The Modern Primitive (Java 9+)

```java
// Replaces sun.misc.Unsafe
private static final VarHandle COUNT;
static {
    COUNT = MethodHandles.lookup()
        .findVarHandle(Counter.class, "count", int.class);
}

// Multiple memory access modes
COUNT.get(this);              // Plain read
COUNT.getVolatile(this);      // Volatile read (visibility)
COUNT.getAcquire(this);       // Acquire semantics (release-acquire)
COUNT.compareAndSet(this, expected, newValue);  // CAS
```

---

## 📖 Chapter 8: Thread-Safe Collections

### The Evolution

```
┌────────────────────────────────────────────────────────────────────────┐
│  Era 1: Vector, Hashtable (synchronized everything)                   │
│  └── Problem: One big lock, poor concurrency                          │
│                                                                        │
│  Era 2: Collections.synchronizedXxx()                                  │
│  └── Problem: Still one big lock                                       │
│                                                                        │
│  Era 3: java.util.concurrent ✨                                        │
│      ConcurrentHashMap       - Fine-grained locks, CAS                 │
│      CopyOnWriteArrayList    - Copy on write, read-heavy               │
│      BlockingQueue           - Producer-consumer built-in              │
│      ConcurrentLinkedQueue   - Lock-free queue                         │
└────────────────────────────────────────────────────────────────────────┘
```

### BlockingQueue: Producer-Consumer Made Easy

```java
BlockingQueue<Task> queue = new ArrayBlockingQueue<>(100);

// Producer
queue.put(task);  // Blocks if queue full! No busy-wait!

// Consumer
Task task = queue.take();  // Blocks if queue empty!

// Timeline:
Producer: put──put──put──[WAIT: queue full]──put──put
Consumer: [WAIT: queue empty]──take──take──take──take
```

---

## 📖 Chapter 9: The Three Nightmares (Concurrency Problems)

### Nightmare 1: Deadlock 🔒💀🔒

```java
// Thread 1                    Thread 2
synchronized(lockA) {          synchronized(lockB) {
    // Got A!                      // Got B!
    synchronized(lockB) {          synchronized(lockA) {
        // Waiting for B...            // Waiting for A...
        // Forever! 💀                  // Forever! 💀
    }                              }
}                              }
```

**Prevention**:

- Always acquire locks in the same order
- Use `tryLock` with timeout
- Use lock hierarchies

### Nightmare 2: Livelock 🔄

```java
// Two polite people in a hallway
while (true) {
    if (otherPersonMoving) {
        step aside;  // Both keep stepping aside forever!
    }
}
```

**Fix**: Add randomness to retry timing.

### Nightmare 3: Starvation 😢

```
High priority threads keep getting the lock.
Low priority thread waits... waits... waits...

Use fair locks: new ReentrantLock(true)
```

### Bonus Nightmare: False Sharing

```java
// These variables might be on the same CPU cache line (64 bytes)
class Counters {
    volatile long counter1;  // Thread 1 writes
    volatile long counter2;  // Thread 2 writes
}

// Every write invalidates the ENTIRE cache line!
// Both CPUs constantly invalidating each other's cache!
```

**Fix**: Padding or `@Contended` annotation

---

## 🎯 Quick Reference Card

```
┌─────────────────────────────────────────────────────────────────────┐
│                   CONCURRENCY QUICK REFERENCE                       │
├─────────────────────┬───────────────────────────────────────────────┤
│ Visibility          │ volatile                                      │
│ Mutual exclusion    │ synchronized, Lock                            │
│ Atomic updates      │ AtomicXxx, VarHandle                          │
│ Thread pools        │ ExecutorService                               │
│ Async pipelines     │ CompletableFuture                             │
│ Producer-consumer   │ BlockingQueue                                 │
│ Lock-free queue     │ ConcurrentLinkedQueue                         │
│ Concurrent map      │ ConcurrentHashMap                             │
├─────────────────────┼───────────────────────────────────────────────┤
│ DEADLOCK            │ Consistent lock order, timeout                │
│ RACE CONDITION      │ Proper synchronization                        │
│ VISIBILITY          │ volatile, synchronized, Atomic                │
└─────────────────────┴───────────────────────────────────────────────┘
```
