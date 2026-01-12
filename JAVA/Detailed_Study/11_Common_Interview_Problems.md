# Common Java Interview Problems

> *These are the classic Java coding problems that test your language mastery. Let's solve them step by step...*

---

## 🎬 The Problem-Solving Journey

These aren't just coding problems - they're opportunities to show you understand Java deeply. Let's tackle them!

---

## 📖 Problem 1: LRU Cache

**The Interview Question**: "Implement a cache that evicts the least recently used item when full."

### The Quick Solution (LinkedHashMap)

```java
public class LRUCache<K, V> extends LinkedHashMap<K, V> {
    private final int capacity;
    
    public LRUCache(int capacity) {
        // true = access order (not insertion order!)
        super(capacity, 0.75f, true);
        this.capacity = capacity;
    }
    
    @Override
    protected boolean removeEldestEntry(Map.Entry<K, V> eldest) {
        return size() > capacity;  // Auto-evict when over capacity!
    }
}

// Usage
LRUCache<Integer, String> cache = new LRUCache<>(3);
cache.put(1, "one");   // [1]
cache.put(2, "two");   // [1, 2]
cache.put(3, "three"); // [1, 2, 3]
cache.get(1);          // Access 1, moves to end: [2, 3, 1]
cache.put(4, "four");  // Over capacity! Evict 2: [3, 1, 4]
```

### The "Show Me You Understand" Solution (Custom)

```java
public class LRUCache<K, V> {
    private final int capacity;
    private final Map<K, Node<K, V>> map = new HashMap<>();
    private final Node<K, V> head = new Node<>(null, null);  // Dummy head
    private final Node<K, V> tail = new Node<>(null, null);  // Dummy tail
    
    public LRUCache(int capacity) {
        this.capacity = capacity;
        head.next = tail;
        tail.prev = head;
    }
    
    public V get(K key) {
        Node<K, V> node = map.get(key);
        if (node == null) return null;
        
        moveToHead(node);  // Mark as recently used
        return node.value;
    }
    
    public void put(K key, V value) {
        Node<K, V> node = map.get(key);
        
        if (node != null) {
            node.value = value;
            moveToHead(node);
        } else {
            if (map.size() >= capacity) {
                Node<K, V> lru = tail.prev;  // LRU is before tail
                remove(lru);
                map.remove(lru.key);
            }
            Node<K, V> newNode = new Node<>(key, value);
            addToHead(newNode);
            map.put(key, newNode);
        }
    }
    
    private void moveToHead(Node<K, V> node) {
        remove(node);
        addToHead(node);
    }
    
    private void remove(Node<K, V> node) {
        node.prev.next = node.next;
        node.next.prev = node.prev;
    }
    
    private void addToHead(Node<K, V> node) {
        node.next = head.next;
        node.prev = head;
        head.next.prev = node;
        head.next = node;
    }
    
    private static class Node<K, V> {
        K key;
        V value;
        Node<K, V> prev, next;
        
        Node(K key, V value) {
            this.key = key;
            this.value = value;
        }
    }
}
```

**Why HashMap + Doubly Linked List?**

```
HashMap: O(1) lookup by key
DLL: O(1) move to head / remove from tail

Together: O(1) get and put! 🚀
```

---

## 📖 Problem 2: Producer-Consumer

**The Interview Question**: "Implement thread-safe producer-consumer."

### The Easy Way (BlockingQueue)

```java
public class ProducerConsumer<T> {
    private final BlockingQueue<T> queue;
    
    public ProducerConsumer(int capacity) {
        this.queue = new ArrayBlockingQueue<>(capacity);
    }
    
    public void produce(T item) throws InterruptedException {
        queue.put(item);  // Blocks if full! ✋
        System.out.println("Produced: " + item);
    }
    
    public T consume() throws InterruptedException {
        T item = queue.take();  // Blocks if empty! ✋
        System.out.println("Consumed: " + item);
        return item;
    }
}

// Usage
ProducerConsumer<Integer> pc = new ProducerConsumer<>(10);

Thread producer = new Thread(() -> {
    for (int i = 0; i < 100; i++) {
        try { pc.produce(i); } catch (InterruptedException e) { break; }
    }
});

Thread consumer = new Thread(() -> {
    while (true) {
        try { pc.consume(); } catch (InterruptedException e) { break; }
    }
});

producer.start();
consumer.start();
```

### The "Prove You Know wait/notify" Way

```java
public class ProducerConsumer<T> {
    private final Queue<T> queue = new LinkedList<>();
    private final int capacity;
    
    public ProducerConsumer(int capacity) {
        this.capacity = capacity;
    }
    
    public synchronized void produce(T item) throws InterruptedException {
        while (queue.size() == capacity) {
            wait();  // Full! Wait for consumer
        }
        queue.offer(item);
        notifyAll();  // Wake waiting consumers!
    }
    
    public synchronized T consume() throws InterruptedException {
        while (queue.isEmpty()) {
            wait();  // Empty! Wait for producer
        }
        T item = queue.poll();
        notifyAll();  // Wake waiting producers!
        return item;
    }
}
```

**Why `while` not `if`?**

```
Spurious wakeups! Thread can wake up without notify.
Always re-check the condition after waking.
```

---

## 📖 Problem 3: Custom HashMap

**The Interview Question**: "Implement HashMap from scratch."

```java
public class MyHashMap<K, V> {
    private static final int DEFAULT_CAPACITY = 16;
    private static final float LOAD_FACTOR = 0.75f;
    
    private Node<K, V>[] buckets;
    private int size;
    
    @SuppressWarnings("unchecked")
    public MyHashMap() {
        buckets = new Node[DEFAULT_CAPACITY];
    }
    
    public void put(K key, V value) {
        // Resize if needed
        if (size >= buckets.length * LOAD_FACTOR) {
            resize();
        }
        
        int index = getIndex(key);
        
        // Check if key exists
        for (Node<K, V> node = buckets[index]; node != null; node = node.next) {
            if (keyEquals(node.key, key)) {
                node.value = value;  // Update existing
                return;
            }
        }
        
        // Add new node at head of chain
        Node<K, V> newNode = new Node<>(key, value);
        newNode.next = buckets[index];
        buckets[index] = newNode;
        size++;
    }
    
    public V get(K key) {
        int index = getIndex(key);
        for (Node<K, V> node = buckets[index]; node != null; node = node.next) {
            if (keyEquals(node.key, key)) {
                return node.value;
            }
        }
        return null;
    }
    
    public V remove(K key) {
        int index = getIndex(key);
        Node<K, V> prev = null;
        Node<K, V> curr = buckets[index];
        
        while (curr != null) {
            if (keyEquals(curr.key, key)) {
                if (prev == null) {
                    buckets[index] = curr.next;
                } else {
                    prev.next = curr.next;
                }
                size--;
                return curr.value;
            }
            prev = curr;
            curr = curr.next;
        }
        return null;
    }
    
    private int getIndex(K key) {
        return key == null ? 0 : Math.abs(key.hashCode() % buckets.length);
    }
    
    private boolean keyEquals(K k1, K k2) {
        return k1 == null ? k2 == null : k1.equals(k2);
    }
    
    @SuppressWarnings("unchecked")
    private void resize() {
        Node<K, V>[] oldBuckets = buckets;
        buckets = new Node[oldBuckets.length * 2];
        size = 0;
        
        for (Node<K, V> bucket : oldBuckets) {
            for (Node<K, V> node = bucket; node != null; node = node.next) {
                put(node.key, node.value);
            }
        }
    }
    
    private static class Node<K, V> {
        K key;
        V value;
        Node<K, V> next;
        
        Node(K key, V value) {
            this.key = key;
            this.value = value;
        }
    }
}
```

---

## 📖 Problem 4: Thread-Safe Singleton

**The Interview Question**: "Implement a thread-safe singleton, explain the approaches."

### Approach 1: Double-Checked Locking

```java
public class Singleton {
    private static volatile Singleton instance;  // volatile is CRITICAL!
    
    private Singleton() { }
    
    public static Singleton getInstance() {
        if (instance == null) {                    // 1st check: avoid locking if exists
            synchronized (Singleton.class) {
                if (instance == null) {            // 2nd check: after acquiring lock
                    instance = new Singleton();
                }
            }
        }
        return instance;
    }
}

// Why volatile?
// Without it, another thread might see partially constructed object!
// instance = new Singleton() is actually:
//   1. Allocate memory
//   2. Initialize object
//   3. Assign to variable
// CPU might reorder 2 and 3! 💥
```

### Approach 2: Bill Pugh (Recommended)

```java
public class Singleton {
    private Singleton() { }
    
    private static class Holder {
        static final Singleton INSTANCE = new Singleton();
    }
    
    public static Singleton getInstance() {
        return Holder.INSTANCE;
    }
}

// Why it works:
// Inner class is loaded ONLY when getInstance() is called
// Class loading is thread-safe by JVM spec!
```

### Approach 3: Enum (Best!)

```java
public enum Singleton {
    INSTANCE;
    
    // Add your methods
    public void doSomething() { }
}

// Usage
Singleton.INSTANCE.doSomething();

// Why best:
// ✅ Thread-safe (JVM guarantees)
// ✅ Serialization-safe (can't create via deserialization)
// ✅ Reflection-safe (can't create via reflection)
```

---

## 📖 Problem 5: Immutable Class

**The Interview Question**: "Make this class immutable."

```java
public final class ImmutablePerson {            // 1. Class is final
    private final String name;                   // 2. Fields are final
    private final Date birthDate;                // 3. Handle mutable fields!
    private final List<String> hobbies;
    
    public ImmutablePerson(String name, Date birthDate, List<String> hobbies) {
        this.name = name;
        this.birthDate = new Date(birthDate.getTime());     // 4. Defensive copy IN
        this.hobbies = new ArrayList<>(hobbies);            // 4. Defensive copy IN
    }
    
    public String getName() {
        return name;  // String is immutable, safe
    }
    
    public Date getBirthDate() {
        return new Date(birthDate.getTime());               // 5. Defensive copy OUT
    }
    
    public List<String> getHobbies() {
        return Collections.unmodifiableList(hobbies);       // 5. Unmodifiable view
    }
    
    // 6. No setters!
}

// The Rules:
// 1. Make class final (prevent subclass from adding mutability)
// 2. Make all fields private final
// 3. No setters
// 4. Defensive copy mutable objects IN (constructor)
// 5. Defensive copy or unmodifiable view OUT (getters)
// 6. Don't leak 'this' during construction
```

---

## 📖 Problem 6: Deadlock Example & Prevention

**The Interview Question**: "Show me a deadlock, then fix it."

### Creating a Deadlock

```java
Object lockA = new Object();
Object lockB = new Object();

// Thread 1: Locks A, then wants B
Thread t1 = new Thread(() -> {
    synchronized (lockA) {
        System.out.println("T1: Got A");
        sleep(100);  // Give T2 time to lock B
        synchronized (lockB) {
            System.out.println("T1: Got B");  // Never happens!
        }
    }
});

// Thread 2: Locks B, then wants A
Thread t2 = new Thread(() -> {
    synchronized (lockB) {
        System.out.println("T2: Got B");
        sleep(100);  // Give T1 time to lock A
        synchronized (lockA) {
            System.out.println("T2: Got A");  // Never happens!
        }
    }
});

// Result: DEADLOCK!
// T1: Got A
// T2: Got B
// ... forever waiting ...
```

### Prevention: Consistent Lock Ordering

```java
// ALWAYS acquire locks in same order!
Thread t1 = new Thread(() -> {
    synchronized (lockA) {       // Always A first
        synchronized (lockB) {   // Then B
            // Work
        }
    }
});

Thread t2 = new Thread(() -> {
    synchronized (lockA) {       // Always A first (same as T1!)
        synchronized (lockB) {   // Then B
            // Work
        }
    }
});
```

### Prevention: tryLock with Timeout

```java
ReentrantLock lockA = new ReentrantLock();
ReentrantLock lockB = new ReentrantLock();

void safeWork() throws InterruptedException {
    while (true) {
        if (lockA.tryLock(50, TimeUnit.MILLISECONDS)) {
            try {
                if (lockB.tryLock(50, TimeUnit.MILLISECONDS)) {
                    try {
                        // Work with both locks
                        return;
                    } finally {
                        lockB.unlock();
                    }
                }
            } finally {
                lockA.unlock();
            }
        }
        // Failed to get both, back off and retry
        Thread.sleep(10);
    }
}
```

---

## 📖 Problem 7: equals() and hashCode() Contract

**The Interview Question**: "Implement equals and hashCode correctly."

```java
public class Person {
    private String name;
    private int age;
    private String email;
    
    @Override
    public boolean equals(Object o) {
        // 1. Same reference?
        if (this == o) return true;
        
        // 2. Null or different class?
        if (o == null || getClass() != o.getClass()) return false;
        
        // 3. Compare fields
        Person person = (Person) o;
        return age == person.age 
            && Objects.equals(name, person.name)
            && Objects.equals(email, person.email);
    }
    
    @Override
    public int hashCode() {
        // Use SAME fields as equals!
        return Objects.hash(name, age, email);
    }
}
```

**The Contract:**

```
1. If a.equals(b) == true  →  a.hashCode() == b.hashCode() (MUST)
2. If hashCode differs     →  Objects are definitely not equal
3. If hashCode same        →  Objects MIGHT be equal (check with equals)

Breaking the contract breaks HashMap!
```

---

## 📖 Problem 8: Rate Limiter (Token Bucket)

**The Interview Question**: "Implement a rate limiter that allows N requests per second."

```java
public class RateLimiter {
    private final int maxTokens;         // Max requests
    private final long refillPeriodMs;   // Period to refill all tokens
    private double availableTokens;
    private long lastRefillTime;
    
    public RateLimiter(int maxTokens, long refillPeriodMs) {
        this.maxTokens = maxTokens;
        this.refillPeriodMs = refillPeriodMs;
        this.availableTokens = maxTokens;
        this.lastRefillTime = System.currentTimeMillis();
    }
    
    public synchronized boolean tryAcquire() {
        refill();
        
        if (availableTokens >= 1) {
            availableTokens--;
            return true;   // Request allowed ✅
        }
        return false;      // Rate limited ❌
    }
    
    private void refill() {
        long now = System.currentTimeMillis();
        long elapsed = now - lastRefillTime;
        
        // Add tokens proportional to time elapsed
        double tokensToAdd = (elapsed * maxTokens) / (double) refillPeriodMs;
        availableTokens = Math.min(maxTokens, availableTokens + tokensToAdd);
        lastRefillTime = now;
    }
}

// Usage: 10 requests per second
RateLimiter limiter = new RateLimiter(10, 1000);

void handleRequest(Request req) {
    if (limiter.tryAcquire()) {
        process(req);
    } else {
        reject(req, "Rate limited!");
    }
}
```

---

## 🎯 Quick Reference: Problem → Data Structure

```
┌─────────────────────────────────────────────────────────────────────────┐
│  Problem                    │  Data Structure / Technique               │
├─────────────────────────────────────────────────────────────────────────┤
│  LRU Cache                  │  HashMap + Doubly Linked List             │
│  Producer-Consumer          │  BlockingQueue or wait/notify             │
│  Custom HashMap             │  Array + Linked List chains               │
│  Thread-safe Singleton      │  Enum or Double-check or Bill Pugh        │
│  Immutable Class            │  final, defensive copies, no setters      │
│  Deadlock                   │  Consistent lock ordering, tryLock        │
│  Rate Limiter               │  Token Bucket algorithm                   │
│  Print 1-N with K threads   │  wait/notify with modulo condition        │
└─────────────────────────────────────────────────────────────────────────┘
```
