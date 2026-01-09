# Concurrency & Multithreading

## 1. Process vs Thread

- **Process**: Independent execution unit with own memory space. (e.g., Chrome Browser).
- **Thread**: Lightweight unit within a process. Shares memory (Heap) but has own Stack. (e.g., Chrome Tab).

## 2. Synchronization Issues

- **Race Condition**: Two threads modify shared data simultaneously, leading to corrupt state.
- **Deadlock**: Thread A holds Lock 1, wants Lock 2. Thread B holds Lock 2, wants Lock 1. Both wait forever.
  - **Prevention**: Acquire locks in same order.

## 3. Java Concurrency Tools

### A. `synchronized` Keyword

- Locks the object/method.
- Simple but performance heavy (blocking).

### B. `volatile`

- Ensures visibility. Changes to variable are instantly visible to other threads (flushes CPU cache to RAM).
- Does **NOT** guarantee atomicity (i+=1 is not safe).

### C. Locks (`ReentrantLock`)

- More flexible than `synchronized`.
- `tryLock()`: Try to get lock, if busy do something else (non-blocking).
- `ReadWriteLock`: Multiple readers allowed, single writer.

### D. ThreadPool (`ExecutorService`)

- Creating threads is expensive (OS overhead).
- **ThreadPool**: Reuses a pool of worker threads.
- `FixedThreadPool(10)`: Max 10 threads.
- `CachedThreadPool`: Creates threads as needed, kills idle ones.

## 4. Producer-Consumer Problem (Classic)

- **Queue**: Shared buffer.
- **Producer**: Adds to queue. Waits if Full.
- **Consumer**: Removes from queue. Waits if Empty.
- **Implementation**: `BlockingQueue` (ArrayBlockingQueue) handles all the wait/notify logic internally.

```java
BlockingQueue<Integer> q = new ArrayBlockingQueue<>(10);
// Producer
q.put(1); // Blocks if full
// Consumer
q.take(); // Blocks if empty
```
