# Common Java Interview Problems

Coding problems that test Java-specific knowledge.

## 1. LRU Cache Implementation

Using `LinkedHashMap` with access order.

```java
public class LRUCache<K, V> extends LinkedHashMap<K, V> {
    private final int capacity;
    
    public LRUCache(int capacity) {
        super(capacity, 0.75f, true);  // true = access order
        this.capacity = capacity;
    }
    
    @Override
    protected boolean removeEldestEntry(Map.Entry<K, V> eldest) {
        return size() > capacity;
    }
}

// Usage
LRUCache<Integer, String> cache = new LRUCache<>(3);
cache.put(1, "one");
cache.put(2, "two");
cache.put(3, "three");
cache.get(1);         // Access 1, moves to end
cache.put(4, "four"); // Evicts 2 (least recently used)
```

### Custom Implementation (No LinkedHashMap)

```java
public class LRUCache<K, V> {
    private final int capacity;
    private final Map<K, Node<K, V>> map;
    private final DoublyLinkedList<K, V> list;
    
    public LRUCache(int capacity) {
        this.capacity = capacity;
        this.map = new HashMap<>();
        this.list = new DoublyLinkedList<>();
    }
    
    public V get(K key) {
        if (!map.containsKey(key)) return null;
        Node<K, V> node = map.get(key);
        list.moveToHead(node);
        return node.value;
    }
    
    public void put(K key, V value) {
        if (map.containsKey(key)) {
            Node<K, V> node = map.get(key);
            node.value = value;
            list.moveToHead(node);
        } else {
            if (map.size() >= capacity) {
                Node<K, V> tail = list.removeTail();
                map.remove(tail.key);
            }
            Node<K, V> node = new Node<>(key, value);
            list.addToHead(node);
            map.put(key, node);
        }
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
    
    private static class DoublyLinkedList<K, V> {
        private Node<K, V> head, tail;
        
        DoublyLinkedList() {
            head = new Node<>(null, null);
            tail = new Node<>(null, null);
            head.next = tail;
            tail.prev = head;
        }
        
        void addToHead(Node<K, V> node) {
            node.next = head.next;
            node.prev = head;
            head.next.prev = node;
            head.next = node;
        }
        
        void remove(Node<K, V> node) {
            node.prev.next = node.next;
            node.next.prev = node.prev;
        }
        
        void moveToHead(Node<K, V> node) {
            remove(node);
            addToHead(node);
        }
        
        Node<K, V> removeTail() {
            Node<K, V> node = tail.prev;
            remove(node);
            return node;
        }
    }
}
```

---

## 2. Producer-Consumer with BlockingQueue

```java
public class ProducerConsumer {
    private final BlockingQueue<Integer> queue;
    
    public ProducerConsumer(int capacity) {
        this.queue = new ArrayBlockingQueue<>(capacity);
    }
    
    public void produce(int item) throws InterruptedException {
        queue.put(item);  // Blocks if full
        System.out.println("Produced: " + item);
    }
    
    public int consume() throws InterruptedException {
        int item = queue.take();  // Blocks if empty
        System.out.println("Consumed: " + item);
        return item;
    }
}

// Usage
ProducerConsumer pc = new ProducerConsumer(10);

// Producer thread
new Thread(() -> {
    for (int i = 0; i < 100; i++) {
        try {
            pc.produce(i);
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
        }
    }
}).start();

// Consumer thread
new Thread(() -> {
    while (true) {
        try {
            pc.consume();
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
            break;
        }
    }
}).start();
```

### Using wait/notify (Manual)

```java
public class ProducerConsumer {
    private final Queue<Integer> queue = new LinkedList<>();
    private final int capacity;
    
    public ProducerConsumer(int capacity) {
        this.capacity = capacity;
    }
    
    public synchronized void produce(int item) throws InterruptedException {
        while (queue.size() == capacity) {
            wait();  // Wait until space available
        }
        queue.offer(item);
        notifyAll();  // Wake consumers
    }
    
    public synchronized int consume() throws InterruptedException {
        while (queue.isEmpty()) {
            wait();  // Wait until items available
        }
        int item = queue.poll();
        notifyAll();  // Wake producers
        return item;
    }
}
```

---

## 3. Custom HashMap Implementation

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
        if (size >= buckets.length * LOAD_FACTOR) {
            resize();
        }
        
        int index = getIndex(key);
        Node<K, V> node = buckets[index];
        
        while (node != null) {
            if (keyEquals(node.key, key)) {
                node.value = value;
                return;
            }
            node = node.next;
        }
        
        Node<K, V> newNode = new Node<>(key, value);
        newNode.next = buckets[index];
        buckets[index] = newNode;
        size++;
    }
    
    public V get(K key) {
        int index = getIndex(key);
        Node<K, V> node = buckets[index];
        
        while (node != null) {
            if (keyEquals(node.key, key)) {
                return node.value;
            }
            node = node.next;
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
        if (key == null) return 0;
        return Math.abs(key.hashCode() % buckets.length);
    }
    
    private boolean keyEquals(K k1, K k2) {
        if (k1 == null) return k2 == null;
        return k1.equals(k2);
    }
    
    @SuppressWarnings("unchecked")
    private void resize() {
        Node<K, V>[] oldBuckets = buckets;
        buckets = new Node[oldBuckets.length * 2];
        size = 0;
        
        for (Node<K, V> node : oldBuckets) {
            while (node != null) {
                put(node.key, node.value);
                node = node.next;
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

## 4. Thread-Safe Singleton Patterns

### Double-Checked Locking

```java
public class Singleton {
    private static volatile Singleton instance;
    
    private Singleton() { }
    
    public static Singleton getInstance() {
        if (instance == null) {                 // First check (no lock)
            synchronized (Singleton.class) {
                if (instance == null) {         // Second check (with lock)
                    instance = new Singleton();
                }
            }
        }
        return instance;
    }
}
```

**Why volatile?** Prevents instruction reordering during object creation.

### Bill Pugh (Initialize-On-Demand)

```java
public class Singleton {
    private Singleton() { }
    
    private static class Holder {
        private static final Singleton INSTANCE = new Singleton();
    }
    
    public static Singleton getInstance() {
        return Holder.INSTANCE;
    }
}
```

**Why it works**: Inner class loaded only when `getInstance()` called.

### Enum Singleton (Best)

```java
public enum Singleton {
    INSTANCE;
    
    private int value;
    
    public int getValue() { return value; }
    public void setValue(int value) { this.value = value; }
}

// Usage
Singleton.INSTANCE.setValue(42);
```

---

## 5. Immutable Class

```java
public final class ImmutablePerson {
    private final String name;
    private final int age;
    private final List<String> hobbies;
    
    public ImmutablePerson(String name, int age, List<String> hobbies) {
        this.name = name;
        this.age = age;
        this.hobbies = new ArrayList<>(hobbies);  // Defensive copy
    }
    
    public String getName() { return name; }
    public int getAge() { return age; }
    
    public List<String> getHobbies() {
        return Collections.unmodifiableList(hobbies);  // Return immutable
    }
}
```

**Rules for Immutability**:

1. Make class `final`
2. Make all fields `private final`
3. No setters
4. Defensive copy in constructor (for mutable objects)
5. Return immutable views of mutable fields

---

## 6. Deep Copy vs Shallow Copy

### Shallow Copy

```java
class Person implements Cloneable {
    String name;
    Address address;
    
    @Override
    protected Person clone() throws CloneNotSupportedException {
        return (Person) super.clone();  // Copies references, not objects
    }
}

Person p1 = new Person();
Person p2 = p1.clone();
p1.address == p2.address;  // true (same object)
```

### Deep Copy

```java
class Person implements Cloneable {
    String name;
    Address address;
    
    @Override
    protected Person clone() throws CloneNotSupportedException {
        Person cloned = (Person) super.clone();
        cloned.address = this.address.clone();  // Clone nested objects
        return cloned;
    }
}

Person p1 = new Person();
Person p2 = p1.clone();
p1.address == p2.address;  // false (different objects)
```

### Using Serialization

```java
public static <T extends Serializable> T deepCopy(T object) {
    try {
        ByteArrayOutputStream bos = new ByteArrayOutputStream();
        ObjectOutputStream oos = new ObjectOutputStream(bos);
        oos.writeObject(object);
        
        ByteArrayInputStream bis = new ByteArrayInputStream(bos.toByteArray());
        ObjectInputStream ois = new ObjectInputStream(bis);
        return (T) ois.readObject();
    } catch (Exception e) {
        throw new RuntimeException(e);
    }
}
```

---

## 7. equals() and hashCode() Contract

```java
public class Person {
    private String name;
    private int age;
    
    @Override
    public boolean equals(Object o) {
        if (this == o) return true;  // Same reference
        if (o == null || getClass() != o.getClass()) return false;
        Person person = (Person) o;
        return age == person.age && Objects.equals(name, person.name);
    }
    
    @Override
    public int hashCode() {
        return Objects.hash(name, age);  // Use same fields as equals
    }
}
```

**Contract Rules**:

1. If `a.equals(b)` then `a.hashCode() == b.hashCode()`
2. If hashCodes differ, objects are definitely not equal
3. Equal hashCodes don't guarantee equality (collision)

---

## 8. Deadlock Example and Prevention

### Creating Deadlock

```java
Object lock1 = new Object();
Object lock2 = new Object();

// Thread 1
new Thread(() -> {
    synchronized (lock1) {
        Thread.sleep(100);  // Ensure both threads lock first lock
        synchronized (lock2) {
            System.out.println("Thread 1");
        }
    }
}).start();

// Thread 2
new Thread(() -> {
    synchronized (lock2) {  // Opposite order!
        Thread.sleep(100);
        synchronized (lock1) {
            System.out.println("Thread 2");
        }
    }
}).start();
```

### Prevention: Consistent Lock Ordering

```java
Object lock1 = new Object();
Object lock2 = new Object();

// Both threads acquire locks in same order
void safeMethod() {
    synchronized (lock1) {
        synchronized (lock2) {
            // Work
        }
    }
}
```

### Prevention: Using tryLock

```java
ReentrantLock lock1 = new ReentrantLock();
ReentrantLock lock2 = new ReentrantLock();

void safeLock() {
    while (true) {
        if (lock1.tryLock()) {
            try {
                if (lock2.tryLock()) {
                    try {
                        // Work
                        return;
                    } finally {
                        lock2.unlock();
                    }
                }
            } finally {
                lock1.unlock();
            }
        }
        Thread.sleep(10);  // Back off, retry
    }
}
```

---

## 9. Print Numbers Using Multiple Threads

Print 1-10 using 3 threads (T1 prints 1,4,7,10; T2 prints 2,5,8; T3 prints 3,6,9).

```java
public class PrintNumbers {
    private int current = 1;
    private final int max = 10;
    private final Object lock = new Object();
    
    public void print(int threadId, int numThreads) {
        while (true) {
            synchronized (lock) {
                while (current <= max && current % numThreads != threadId % numThreads) {
                    try {
                        lock.wait();
                    } catch (InterruptedException e) {
                        return;
                    }
                }
                
                if (current > max) {
                    lock.notifyAll();
                    return;
                }
                
                System.out.println("Thread-" + threadId + ": " + current);
                current++;
                lock.notifyAll();
            }
        }
    }
    
    public static void main(String[] args) {
        PrintNumbers pn = new PrintNumbers();
        
        new Thread(() -> pn.print(1, 3)).start();
        new Thread(() -> pn.print(2, 3)).start();
        new Thread(() -> pn.print(3, 3)).start();
    }
}
```

---

## 10. Rate Limiter

Token Bucket implementation:

```java
public class RateLimiter {
    private final int maxTokens;
    private final long refillIntervalMs;
    private double tokens;
    private long lastRefillTime;
    
    public RateLimiter(int maxTokens, long refillIntervalMs) {
        this.maxTokens = maxTokens;
        this.refillIntervalMs = refillIntervalMs;
        this.tokens = maxTokens;
        this.lastRefillTime = System.currentTimeMillis();
    }
    
    public synchronized boolean tryAcquire() {
        refill();
        if (tokens >= 1) {
            tokens--;
            return true;
        }
        return false;
    }
    
    private void refill() {
        long now = System.currentTimeMillis();
        long elapsed = now - lastRefillTime;
        double tokensToAdd = elapsed * maxTokens / (double) refillIntervalMs;
        tokens = Math.min(maxTokens, tokens + tokensToAdd);
        lastRefillTime = now;
    }
}

// Usage: Allow 10 requests per second
RateLimiter limiter = new RateLimiter(10, 1000);
if (limiter.tryAcquire()) {
    processRequest();
} else {
    reject();
}
```
