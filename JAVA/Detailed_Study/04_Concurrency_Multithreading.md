# Concurrency & Multi-threading

Concurrency is tested in almost every senior Java interview. Master these concepts.

## 1. Thread Basics

### Creating Threads

```java
// Method 1: Extend Thread
class MyThread extends Thread {
    public void run() { /* work */ }
}

// Method 2: Implement Runnable (preferred)
Runnable task = () -> { /* work */ };
new Thread(task).start();

// Method 3: Callable (returns result)
Callable<Integer> task = () -> 42;
```

### Thread Lifecycle

```
NEW → RUNNABLE ↔ BLOCKED/WAITING/TIMED_WAITING → TERMINATED
         ↑__________________|
```

| State | Description |
|-------|-------------|
| **NEW** | Created, not started |
| **RUNNABLE** | Running or ready to run |
| **BLOCKED** | Waiting for monitor lock |
| **WAITING** | `wait()`, `join()`, `park()` |
| **TIMED_WAITING** | `sleep()`, `wait(timeout)` |
| **TERMINATED** | Completed execution |

---

## 2. Synchronization Primitives

### `synchronized` Keyword

**Object-level lock** (intrinsic monitor):

```java
public class Counter {
    private int count = 0;
    
    // Synchronized method (locks on 'this')
    public synchronized void increment() {
        count++;
    }
    
    // Synchronized block (explicit lock object)
    public void decrement() {
        synchronized (this) {
            count--;
        }
    }
    
    // Class-level lock (for static methods)
    public static synchronized void staticMethod() { }
}
```

**Properties**:

- Reentrant (same thread can acquire same lock multiple times)
- Automatic release (even on exception)

### `volatile` Keyword

Ensures **visibility** but NOT atomicity.

```java
volatile boolean running = true;

// Thread 1
while (running) {
    // Work
}

// Thread 2
running = false; // Thread 1 will see this immediately
```

**Use cases**: Flags, single-writer scenarios.

**NOT sufficient for**: `count++` (read-modify-write is not atomic).

---

## 3. Locks (java.util.concurrent.locks)

### ReentrantLock

```java
ReentrantLock lock = new ReentrantLock();

lock.lock();
try {
    // Critical section
} finally {
    lock.unlock(); // ALWAYS in finally!
}
```

### ReentrantLock Features

| Feature | Description |
|---------|-------------|
| `tryLock()` | Non-blocking, returns immediately |
| `tryLock(timeout)` | Wait with timeout |
| `lockInterruptibly()` | Responds to interrupt |
| `newCondition()` | Multiple wait sets |
| Fairness | `new ReentrantLock(true)` - longest waiting thread wins |

### ReadWriteLock

```java
ReadWriteLock rwLock = new ReentrantReadWriteLock();
Lock readLock = rwLock.readLock();
Lock writeLock = rwLock.writeLock();

// Multiple readers allowed
readLock.lock();
try {
    // Read data
} finally {
    readLock.unlock();
}

// Only one writer, blocks all readers
writeLock.lock();
try {
    // Write data
} finally {
    writeLock.unlock();
}
```

### StampedLock (Java 8+)

Optimistic reading for better performance:

```java
StampedLock lock = new StampedLock();

// Optimistic read (no locking!)
long stamp = lock.tryOptimisticRead();
int currentX = x;
int currentY = y;
if (!lock.validate(stamp)) {
    // Data may have changed, fall back to read lock
    stamp = lock.readLock();
    try {
        currentX = x;
        currentY = y;
    } finally {
        lock.unlockRead(stamp);
    }
}
```

---

## 4. Wait/Notify vs Conditions

### Object.wait() / notify()

```java
synchronized (lock) {
    while (!condition) {
        lock.wait();  // Releases lock, waits for notify
    }
    // Process when condition is true
}

synchronized (lock) {
    condition = true;
    lock.notifyAll();  // Wake all waiting threads
}
```

### Condition (with Lock)

```java
Lock lock = new ReentrantLock();
Condition notEmpty = lock.newCondition();
Condition notFull = lock.newCondition();

// Consumer
lock.lock();
try {
    while (queue.isEmpty()) {
        notEmpty.await();
    }
    item = queue.remove();
    notFull.signal();
} finally {
    lock.unlock();
}
```

**Advantage**: Multiple conditions per lock.

---

## 5. Executors & Thread Pools

### Why Thread Pools?

- Reuse threads (creation is expensive)
- Control max concurrency
- Manage task queue

### ExecutorService Types

```java
// Fixed thread pool
ExecutorService fixed = Executors.newFixedThreadPool(10);

// Cached (creates threads as needed, reuses idle)
ExecutorService cached = Executors.newCachedThreadPool();

// Single thread (ordering guaranteed)
ExecutorService single = Executors.newSingleThreadExecutor();

// Scheduled
ScheduledExecutorService scheduled = Executors.newScheduledThreadPool(5);
```

### ThreadPoolExecutor Parameters

```java
new ThreadPoolExecutor(
    corePoolSize,     // Threads to keep alive
    maxPoolSize,      // Max threads
    keepAliveTime,    // Idle thread timeout
    TimeUnit.SECONDS,
    workQueue,        // Task queue (LinkedBlockingQueue, ArrayBlockingQueue)
    threadFactory,    // Custom thread creation
    rejectionHandler  // What to do when queue full
);
```

### Rejection Policies

| Policy | Behavior |
|--------|----------|
| `AbortPolicy` | Throw `RejectedExecutionException` |
| `CallerRunsPolicy` | Caller thread runs the task |
| `DiscardPolicy` | Silently discard |
| `DiscardOldestPolicy` | Discard oldest, retry |

### ForkJoinPool

For recursive divide-and-conquer tasks.

```java
class SumTask extends RecursiveTask<Long> {
    protected Long compute() {
        if (small enough) {
            return directSum();
        }
        SumTask left = new SumTask(leftHalf);
        SumTask right = new SumTask(rightHalf);
        left.fork();
        return right.compute() + left.join();
    }
}

ForkJoinPool pool = new ForkJoinPool();
Long result = pool.invoke(new SumTask(data));
```

**Note**: Parallel streams use the common ForkJoinPool.

---

## 6. CompletableFuture

Non-blocking, composable async operations.

### Basic Usage

```java
CompletableFuture<String> future = CompletableFuture.supplyAsync(() -> {
    return fetchData();
});

// Non-blocking callback
future.thenApply(data -> process(data))
      .thenAccept(result -> System.out.println(result));

// Blocking (if needed)
String result = future.get();
```

### Composition

```java
// Chain operations
CompletableFuture.supplyAsync(() -> fetchUser())
    .thenApply(user -> fetchOrders(user))
    .thenApply(orders -> calculateTotal(orders))
    .thenAccept(total -> display(total));

// Combine two futures
CompletableFuture<String> userFuture = fetchUserAsync();
CompletableFuture<List<Order>> ordersFuture = fetchOrdersAsync();

userFuture.thenCombine(ordersFuture, (user, orders) -> {
    return new UserOrders(user, orders);
});

// Wait for all
CompletableFuture.allOf(future1, future2, future3).join();

// Wait for any
CompletableFuture.anyOf(future1, future2).thenAccept(first -> {});
```

### Exception Handling

```java
future.exceptionally(ex -> {
    log.error("Failed", ex);
    return defaultValue;
});

future.handle((result, ex) -> {
    if (ex != null) return defaultValue;
    return result;
});
```

---

## 7. Atomic Classes

Lock-free thread-safe operations using CAS (Compare-And-Swap).

### AtomicInteger

```java
AtomicInteger counter = new AtomicInteger(0);

counter.incrementAndGet();  // ++counter
counter.getAndIncrement();  // counter++
counter.compareAndSet(expected, newValue);  // CAS
counter.updateAndGet(x -> x * 2);  // Lambda update
```

### Other Atomics

| Class | Use Case |
|-------|----------|
| `AtomicLong` | 64-bit counter |
| `AtomicBoolean` | Flags |
| `AtomicReference<V>` | Reference updates |
| `AtomicIntegerArray` | Array of atomic ints |
| `LongAdder` | High-contention counters (faster than AtomicLong) |

### CAS (Compare-And-Swap)

```java
// Pseudo-implementation
boolean compareAndSet(int expected, int newValue) {
    synchronized (this) {
        if (current == expected) {
            current = newValue;
            return true;
        }
        return false;
    }
}
```

CPU-level atomic instruction. No locking, but may need retry loop.

---

## 8. Thread-Safe Collections

| Collection | Description |
|------------|-------------|
| `ConcurrentHashMap` | Fine-grained locking (CAS in Java 8+) |
| `CopyOnWriteArrayList` | Copy on write, great for read-heavy |
| `ConcurrentLinkedQueue` | Lock-free queue |
| `BlockingQueue` | Producer-consumer (put blocks if full) |
| `ConcurrentSkipListMap` | Concurrent sorted map |

### BlockingQueue Implementations

```java
BlockingQueue<Task> queue = new ArrayBlockingQueue<>(100);

// Producer
queue.put(task);  // Blocks if full

// Consumer
Task task = queue.take();  // Blocks if empty
```

---

## 9. Common Concurrency Problems

### Deadlock

Two threads waiting for each other's locks.

```java
// Thread 1: lock A, then lock B
// Thread 2: lock B, then lock A
// → Deadlock!
```

**Prevention**: Always acquire locks in consistent order.

### Livelock

Threads keep responding to each other, never progressing.

### Starvation

A thread never gets CPU time (low priority or always losing lock).

### False Sharing

Different threads modify variables on same cache line.

```java
// These might be on same cache line
long a;  // Thread 1 writes
long b;  // Thread 2 writes
// → Cache invalidation ping-pong
```

**Fix**: Padding or `@Contended` annotation.

---

## 10. Interview Questions

**Q: `synchronized` vs `ReentrantLock`?**

| synchronized | ReentrantLock |
|--------------|---------------|
| Implicit lock/unlock | Explicit (must call unlock) |
| Built-in | More features (tryLock, fairness) |
| No timeout | Timeout support |
| No interruptible | lockInterruptibly() |

**Q: How to create a thread-safe Singleton?**

```java
// Double-checked locking
public class Singleton {
    private static volatile Singleton instance;
    
    public static Singleton getInstance() {
        if (instance == null) {
            synchronized (Singleton.class) {
                if (instance == null) {
                    instance = new Singleton();
                }
            }
        }
        return instance;
    }
}

// Better: Enum singleton
public enum Singleton {
    INSTANCE;
}
```

**Q: Why use `volatile` with double-checked locking?**

Without it, another thread might see partially constructed object due to instruction reordering.
