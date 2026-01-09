# Advanced Concurrency Patterns

## 1. Thread-Safe Singleton (Double Checked Locking)

```java
public class Singleton {
    private static volatile Singleton instance; // volatile is key
    
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
```

## 2. Producer - Consumer Pattern

Using `wait()` and `notify()`. This is arguably the most asked concurrency question.

```java
public class SharedBuffer {
    Queue<Integer> q = new LinkedList<>();
    int capacity = 5;
    
    public void produce(int item) throws InterruptedException {
        synchronized(this) {
            while (q.size() == capacity) {
                wait(); // Release lock and wait
            }
            q.add(item);
            System.out.println("Produced: " + item);
            notifyAll(); // Wake up consumers
        }
    }
    
    public void consume() throws InterruptedException {
        synchronized(this) {
            while (q.isEmpty()) {
                wait(); // Wait for data
            }
            int val = q.poll();
            System.out.println("Consumed: " + val);
            notifyAll(); // Wake up producers
        }
    }
}
```

## 3. ReadWriteLock

Scenario: 99 Readers, 1 Writer. `synchronized` blocks everyone. We want Readers to access parallelly, but Writer to block everyone.

```java
ReadWriteLock lock = new ReentrantReadWriteLock();

public void read() {
    lock.readLock().lock();
    // read data... (Multiple threads can be here)
    lock.readLock().unlock();
}

public void write() {
    lock.writeLock().lock();
    // write data... (Only 1 thread can be here, and 0 readers)
    lock.writeLock().unlock();
}
```

## 4. CompletableFuture (Async Programming)

Modern Java (and JS) uses Futures/Promises instead of raw threads.

```java
CompletableFuture.supplyAsync(() -> fetchOrder())
    .thenApply(order -> enrichOrder(order))
    .thenAccept(order -> saveToDB(order));
// Non-blocking chain execution
```
