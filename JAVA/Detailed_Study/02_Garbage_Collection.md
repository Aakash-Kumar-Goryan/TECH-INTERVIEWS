# Garbage Collection Deep Dive

GC knowledge is essential for production debugging and performance tuning.

## 1. Why Garbage Collection?

Manual memory management (C/C++) leads to:

- Memory leaks (forgot to free)
- Dangling pointers (freed too early)
- Double free bugs

GC automates memory reclamation for **unreachable** objects.

---

## 2. The Generational Hypothesis

> "Most objects die young."

**Observation**: In typical applications:

- ~95% of objects are short-lived (temporary variables, intermediate results)
- ~5% are long-lived (caches, singletons)

**Implication**: Optimize for cleaning short-lived objects quickly (Minor GC on Young Gen).

---

## 3. GC Roots & Reachability

An object is **reachable** if there's a path from a GC root to it.

### GC Roots Include

- Local variables in active threads
- Static variables
- JNI references
- Active Thread objects

### Mark and Sweep

1. **Mark Phase**: Traverse from GC roots, mark all reachable objects.
2. **Sweep Phase**: Reclaim memory from unmarked objects.
3. **Compact (optional)**: Defragment heap.

---

## 4. Heap Generations

```
┌──────────────────────────────────────────────────────────────┐
│                           HEAP                                │
│  ┌─────────────────────────────┐  ┌────────────────────────┐ │
│  │      Young Generation       │  │    Old Generation      │ │
│  │  ┌──────┐ ┌─────┐ ┌─────┐  │  │    (Tenured)           │ │
│  │  │ Eden │ │ S0  │ │ S1  │  │  │                        │ │
│  │  │      │ │     │ │     │  │  │   Long-lived Objects   │ │
│  │  └──────┘ └─────┘ └─────┘  │  │                        │ │
│  └─────────────────────────────┘  └────────────────────────┘ │
└──────────────────────────────────────────────────────────────┘
```

### Object Lifecycle

1. **New objects** → Eden
2. **Minor GC** (Eden full):
   - Live objects → Survivor (S0 or S1)
   - Dead objects reclaimed
   - Survivors swap between S0/S1 each cycle
3. **Age threshold** (default 15 cycles):
   - Survivors → Old Gen (promotion/tenuring)
4. **Major GC / Full GC** (Old Gen full):
   - Cleans entire heap
   - "Stop-the-world" pauses

---

## 5. GC Algorithms

### Serial GC (`-XX:+UseSerialGC`)

- Single-threaded
- Stop-the-world for all GC
- **Use case**: Small apps, single-core machines

### Parallel GC (`-XX:+UseParallelGC`)

- Multi-threaded for Young Gen
- Throughput-focused (max CPU for app)
- **Use case**: Batch processing, no latency requirements

### G1 GC (`-XX:+UseG1GC`) — Default since Java 9

**Garbage First** divides heap into equal-sized **regions** (not contiguous generations).

```
┌───┬───┬───┬───┬───┬───┬───┬───┐
│ E │ E │ S │ O │ O │ O │ H │   │
└───┴───┴───┴───┴───┴───┴───┴───┘
E = Eden, S = Survivor, O = Old, H = Humongous (large objects)
```

**How it works**:

1. Tracks garbage amount per region.
2. Collects regions with most garbage first.
3. Concurrent marking (reduced pause).

**Key flag**: `-XX:MaxGCPauseMillis=200` (target pause time)

### ZGC (`-XX:+UseZGC`) — Java 11+

- Ultra-low latency: **< 10ms pauses** (even with TB heaps)
- Concurrent compaction (no stop-the-world for moving objects)
- Uses colored pointers (metadata in pointer bits)
- **Use case**: Latency-critical apps (trading, real-time systems)

### Shenandoah (`-XX:+UseShenandoahGC`)

- Similar goals to ZGC
- Brooks pointers for concurrent compaction
- Available in OpenJDK (not Oracle JDK)

---

## 6. GC Tuning Flags

### Memory Sizing

| Flag | Description |
|------|-------------|
| `-Xms` | Initial heap size |
| `-Xmx` | Maximum heap size |
| `-Xmn` | Young generation size |
| `-XX:MetaspaceSize` | Initial metaspace |
| `-XX:MaxMetaspaceSize` | Max metaspace |

### GC Selection

| Flag | Collector |
|------|-----------|
| `-XX:+UseSerialGC` | Serial |
| `-XX:+UseParallelGC` | Parallel |
| `-XX:+UseG1GC` | G1 |
| `-XX:+UseZGC` | ZGC |
| `-XX:+UseShenandoahGC` | Shenandoah |

### GC Logging (Java 9+)

```bash
-Xlog:gc*:file=gc.log:time,level,tags
```

### Common Tuning

```bash
# Typical production settings
java -Xms4g -Xmx4g \            # Fixed heap (avoid resizing)
     -XX:+UseG1GC \             # Use G1
     -XX:MaxGCPauseMillis=200 \ # Target pause
     -XX:+HeapDumpOnOutOfMemoryError \
     -XX:HeapDumpPath=/dumps/   # Dump on OOM
```

---

## 7. Memory Leaks in Java

GC removes **unreachable** objects. A leak happens when you hold references to unneeded objects.

### Common Causes

| Cause | Example | Fix |
|-------|---------|-----|
| **Static Collections** | `static List<User> cache` grows forever | Use bounded cache (LRU) |
| **Unclosed Resources** | `InputStream` not closed | `try-with-resources` |
| **ThreadLocal** | Not removed in thread pools | Call `remove()` after use |
| **Listeners/Callbacks** | Register without deregister | Implement cleanup |
| **Inner Class Reference** | Non-static inner class holds outer ref | Use static inner class |

### Detection Tools

| Tool | Description |
|------|-------------|
| **jmap** | Heap dump: `jmap -dump:live,format=b,file=heap.hprof <pid>` |
| **jvisualvm** | GUI for heap analysis |
| **Eclipse MAT** | Memory Analyzer Tool |
| **jcmd** | `jcmd <pid> GC.heap_dump file.hprof` |

---

## 8. Diagnosing GC Issues

### High GC Frequency

**Symptoms**: Frequent Minor GCs, high CPU.
**Cause**: Object churn (creating many short-lived objects).
**Fix**: Object pooling, reduce allocations.

### Long GC Pauses

**Symptoms**: Application freezes.
**Cause**: Full GC on large Old Gen.
**Fix**:

- Increase heap or tune generations
- Switch to low-latency GC (G1, ZGC)
- Fix memory leaks causing Old Gen growth

### OutOfMemoryError

| Error | Cause |
|-------|-------|
| `Java heap space` | Heap exhausted |
| `Metaspace` | Too many classes loaded |
| `GC overhead limit exceeded` | GC running constantly with little recovery |

---

## 9. Interview Questions

**Q: What triggers Minor vs Major GC?**

- **Minor GC**: Eden space full.
- **Major/Full GC**: Old Gen full, or explicit `System.gc()`, or Metaspace pressure.

**Q: How does G1 achieve predictable pauses?**

- Divides heap into regions.
- Tracks garbage per region.
- Collects high-garbage regions first within pause target.
- Incremental collection.

**Q: Difference between `System.gc()` and `Runtime.gc()`?**

Both are hints. `System.gc()` calls `Runtime.getRuntime().gc()`. Neither guarantees GC.

**Q: What is a "Stop-the-World" pause?**

All application threads pause while GC runs. Necessary for consistent heap state during marking/compaction.
