# JAVA Quick Revision Cheat Sheet (FAANG Level)

*Core internals, JVM architecture, and performance-critical concepts.*

## 1. JVM Architecture

### Memory Model (Runtime Data Areas)

| Area | Shared? | Description |
|------|---------|-------------|
| **Heap** | **Yes** | Objects, Class Instances, Arrays. Managed by GC. Split into Young Gen (Eden, S0, S1) & Old Gen. |
| **Metaspace** | **Yes** | Class metadata, static variables, constants. Replaced PermGen (Java 8). Native Memory. |
| **Stack** | No | Per-thread. Stores Frames (Local vars, partial results, method references). |
| **PC Register** | No | Current instruction address being executed. |
| **Native Stack** | No | For Native methods (C++ JNI). |

### Class Loaders

1. **Bootstrap:** Loads Core (rt.jar, java.lang.*). Native code.
2. **Platform/Extension:** Loads `lib/ext`.
3. **Application/System:** Loads classpath (your code).

## 2. Garbage Collection (GC)

### Key Algorithms

* **Serial GC:** Single-threaded. Small apps.
* **Parallel GC:** Multi-threaded throughput collector.
* **G1 GC (Garbage First):** Splits heap into regions. Prioritizes regions with most garbage. Low latency + High throughput. (Default Java 9+).
* **ZGC / Shenandoah:** Ultra-low latency (<10ms pauses). Concurrent compaction.

### The Cycle

1. **Eden:** New objects/Short-lived.
2. **Minor GC:** Cleans Eden. Survivors -> S0/S1.
3. **Tenured:** Survivors > Threshold (e.g., 15 cycles) move to Old Gen.
4. **Major/Full GC:** Cleans Old Gen. "Stop-the-world" pauses usually happen here.

## 3. Concurrency & Multi-threading

### Core Interfaces

* `Runnable`: `void run()`. No return, no checked exceptions.
* `Callable<V>`: `V call()`. Returns result, throws Exception.
* `Future<V>`: Holder for pending result. (`get()` is blocking).
* `CompletableFuture`: Non-blocking, functional composition (`thenApply`, `thenCompose`).

### Executors

* `FixedThreadPool(n)`: Fixed threads, unbounded queue.
* `CachedThreadPool()`: Creates threads as needed, kills idle (60s). Danger: Unbounded growth.
* `ForkJoinPool`: Work-stealing algorithm. Great for recursive tasks (Parallel Streams).

### Synchronization

* `synchronized`: Intrinsic lock on object/class. Reentrant.
* `ReentrantLock`: Explicit lock. `tryLock()`, `lockInterruptibly()`.
* `ReadWriteLock`: Many readers, one writer.
* `volatile`: Visibility only. No atomicity. Next read sees last write.

## 4. Java 8+ Features

### Functional Interfaces

* `Predicate<T>`: `T -> boolean`
* `Function<T,R>`: `T -> R`
* `Consumer<T>`: `T -> void`
* `Supplier<T>`: `() -> T`

### Stream API Pipeline

1. **Source:** Collection, Array, I/O.
2. **Intermediate (Lazy):** `filter`, `map`, `flatMap`, `distinct`, `sorted`.
3. **Terminal (Eager):** `collect`, `forEach`, `reduce`, `count`, `findFirst`.

## 5. Collections Performance (Big O)

| Collection | Add | Contains | Next | Remove | Notes |
|------------|-----|----------|------|--------|-------|
| ArrayList | O(1)* | O(n) | O(1) | O(n) | Resizing is O(n). Good cache locality. |
| LinkedList | O(1) | O(n) | O(1) | O(1) | Memory overhead (Node pointers). Bad cache. |
| HashSet | O(1) | O(1) | O(h/n) | O(1) | Collision may degrade to O(n) or O(log n). |
| TreeSet | O(log n) | O(log n) | O(log n) | O(log n) | Sorted (Red-Black Tree). |
| PriorityQueue | O(log n) | O(1) (peek) | - | O(log n) | Binary Heap. Not thread-safe. |

*Amortized.
