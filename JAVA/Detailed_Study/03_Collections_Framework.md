# Collections Framework Deep Dive

> *Collections are the workhorses of every Java program. Let's explore them through stories of how data actually lives and moves...*

---

## 🎬 The Collection Kingdom

Imagine your data as citizens living in different neighborhoods. Each neighborhood (collection) has its own rules, architecture, and personality.

```
                        👑 Iterable<E>
                            │
                       Collection<E>
              ┌─────────────┼─────────────┐
              │             │             │
          📋 List<E>    🎯 Set<E>    📬 Queue<E>
         "Ordered,      "No dupes,   "First-in,
          indexed"       unique"      first-out"


                       🗺️ Map<K,V>
              ┌─────────────┼─────────────┐
              │             │             │
         HashMap       TreeMap      LinkedHashMap
        "Fast lookup"  "Sorted"     "Remembers order"
```

---

## 📖 Chapter 1: The Array Apartment (ArrayList)

**The Story**: ArrayList is like an apartment building. All residents live in numbered units. Easy to find anyone by their unit number, but moving in or out from the middle? Everyone has to shuffle!

### The Blueprint

```java
// What's really inside ArrayList
Object[] elementData = new Object[10];  // The apartment building
int size = 0;                            // Occupied units

// Adding "Alice" to position 0
elementData[0] = "Alice"; size++;        // Just assign, O(1)

// Adding "Carol" to position 1 (between Alice and Bob)
// Everyone from position 1 onwards must move! 😰
```

### Life in the Apartment

```
Before inserting "Carol" at index 1:
┌───────┬───────┬───────┬───────┬───────┐
│ Alice │  Bob  │ David │       │       │
│   0   │   1   │   2   │   3   │   4   │
└───────┴───────┴───────┴───────┴───────┘

Insert at index 1:
         💨 Everyone shifts right!
┌───────┬───────┬───────┬───────┬───────┐
│ Alice │ Carol │  Bob  │ David │       │
│   0   │   1   │   2   │   3   │   4   │
└───────┴───────┴───────┴───────┴───────┘
```

### When the Building Gets Full (Resizing)

```java
// Default capacity: 10 units
// When full: newCapacity = oldCapacity + (oldCapacity >> 1)
//           = 10 + 5 = 15 (grow by 50%)

// Behind the scenes:
Object[] newBuilding = new Object[15];
System.arraycopy(oldBuilding, 0, newBuilding, 0, size);
// Everyone moves to bigger building! 📦
```

### The Performance Card

| Operation | Time | What Happens |
|-----------|------|--------------|
| `get(index)` | O(1) | "Unit 42? Right here!" |
| `add(element)` | O(1)* | Add to end, unless building full |
| `add(index, e)` | O(n) | "Everyone move right!" |
| `remove(index)` | O(n) | "Everyone move left!" |
| `contains(e)` | O(n) | Check every unit door-to-door |

**Pro Tip**: Know your guest count? `new ArrayList<>(1000)` avoids moving day!

---

## 📖 Chapter 2: The Chain Gang (LinkedList)

**The Story**: LinkedList is like a train. Each car (node) only knows the car ahead and behind it. Great for adding cars anywhere, but finding car #500? Walk the whole train!

### The Blueprint

```java
class Node<E> {
    E item;           // The passenger
    Node<E> next;     // "Car ahead is..."
    Node<E> prev;     // "Car behind is..."
}

Node<E> first;  // Engine
Node<E> last;   // Caboose
```

### Visualizing the Chain

```
           ┌─────────────┐     ┌─────────────┐     ┌─────────────┐
  null ←── │ prev: null  │ ←── │ prev: ◀     │ ←── │ prev: ◀     │
           │ item: Alice │     │ item: Bob   │     │ item: Carol │
  first ─▶ │ next: ▶     │ ──▶ │ next: ▶     │ ──▶ │ next: null  │ ◀── last
           └─────────────┘     └─────────────┘     └─────────────┘
```

### Adding a New Car (O(1) at ends!)

```
Adding "New" after Alice (we have reference to Alice's node):

Before: Alice ←→ Bob ←→ Carol

1. Create new node
2. New.prev = Alice
3. New.next = Alice.next (Bob)
4. Alice.next = New
5. Bob.prev = New

After:  Alice ←→ New ←→ Bob ←→ Carol

Just 4 pointer updates! No shifting! 🎉
```

### The Bitter Truth

```java
// "Get me element 500" in LinkedList:
Node<E> current = first;
for (int i = 0; i < 500; i++) {
    current = current.next;  // Walk... walk... walk...
}
return current.item;  // O(n) 😢

// Same in ArrayList:
return elementData[500];  // O(1) 🚀
```

**When to use LinkedList?** Almost never. ArrayList's cache locality wins. Use `ArrayDeque` for queue/stack.

---

## 📖 Chapter 3: The Hash Embassy (HashMap)

**The Story**: HashMap is like an embassy with numbered counters. Your passport (key's hashCode) determines which counter you go to. But sometimes two people go to the same counter (collision!).

### The Embassy Floor Plan

```
table[] (capacity = 16)
┌───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┐
│ 0 │ 1 │ 2 │ 3 │ 4 │ 5 │ 6 │ 7 │ 8 │ 9 │10 │11 │12 │13 │14 │15 │
└───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┘
          │           │
          ▼           ▼
        [Node]      [Node]──▶[Node]──▶[Node]  ← Collision chain!
        Alice       Bob       Carol     Dave
```

### The put() Journey

```java
map.put("Alice", 100);

// Step 1: Compute hash
hash = "Alice".hashCode();                    // 92668751
hash = hash ^ (hash >>> 16);                  // Extra scrambling

// Step 2: Find bucket
index = hash & (capacity - 1);                // Fast modulo!
// (capacity is always power of 2, so this works)

// Step 3: Place in bucket
if (bucket[2] == null) {
    bucket[2] = new Node("Alice", 100);       // Empty? Just place!
} else {
    // Collision! Walk the chain...
    for (Node n = bucket[2]; n != null; n = n.next) {
        if (n.key.equals("Alice")) {
            n.value = 100; return;            // Update existing
        }
    }
    bucket[2].append(new Node("Alice", 100)); // Add to chain
}
```

### The Treeification Magic (Java 8+)

```
When a bucket has > 8 entries... 🌲

Before (O(n) lookup in chain):
[Node]──▶[Node]──▶[Node]──▶[Node]──▶[Node]──▶[Node]──▶[Node]──▶[Node]──▶[Node]

After treeification (O(log n) lookup):
                    [TreeNode root]
                   /               \
            [TreeNode]           [TreeNode]
            /       \            /       \
      [TreeNode] [TreeNode] [TreeNode] [TreeNode]

Threshold: > 8 entries → Tree
Untreeify: < 6 entries → Back to List
```

### Why Power of 2 Capacity?

```java
// Normal modulo (slow):
index = hash % 16;          // Division operation

// Bitwise AND (fast):
index = hash & (16 - 1);    // Same result when n is power of 2!
index = hash & 0x0F;        // Just mask lower bits!

// This is why HashMap always uses 16, 32, 64, 128...
```

### The Resize Earthquake

```
When size > capacity * loadFactor (default 0.75):
16 * 0.75 = 12 entries triggers resize

Before: 16 buckets   │  After: 32 buckets
────────────────────┼────────────────────
All entries must    │  Old bucket 5 splits into:
be REHASHED!        │    → New bucket 5
O(n) operation      │    → New bucket 21 (5 + 16)
```

---

## 📖 Chapter 4: The Concurrent Embassy (ConcurrentHashMap)

**The Story**: Multiple visitors can be served simultaneously, but each counter (bucket) has its own lock!

### Evolution of Thread-Safety

```
┌─────────────────────────────────────────────────────────────────────────┐
│ Hashtable (Legacy)                                                      │
│ synchronized void put(K k, V v) { ... }                                 │
│ 🔒 ONE big lock for entire table → Everyone waits!                      │
└─────────────────────────────────────────────────────────────────────────┘
                                    ↓
┌─────────────────────────────────────────────────────────────────────────┐
│ ConcurrentHashMap (Java 7): Segment Locks                               │
│ ┌──────────────────┐┌──────────────────┐┌──────────────────┐           │
│ │ 🔒 Segment 0     ││ 🔒 Segment 1     ││ 🔒 Segment 2     │           │
│ │ [bucket][bucket] ││ [bucket][bucket] ││ [bucket][bucket] │           │
│ └──────────────────┘└──────────────────┘└──────────────────┘           │
│ 16 segments = 16 concurrent writes possible!                            │
└─────────────────────────────────────────────────────────────────────────┘
                                    ↓
┌─────────────────────────────────────────────────────────────────────────┐
│ ConcurrentHashMap (Java 8+): Per-Bucket Locking                         │
│ ┌────┐┌────┐┌────┐┌────┐┌────┐┌────┐┌────┐┌────┐                       │
│ │🔒 ││🔒 ││🔒 ││🔒 ││🔒 ││🔒 ││🔒 ││🔒 │  Each bucket!             │
│ └────┘└────┘└────┘└────┘└────┘└────┘└────┘└────┘                       │
│ + CAS for empty buckets (no lock at all!)                               │
└─────────────────────────────────────────────────────────────────────────┘
```

### The Java 8+ put() Dance

```java
if (bucket[i] == null) {
    // Empty bucket? Use CAS (no lock!)
    if (CAS(bucket[i], null, newNode)) {
        return;  // Success without any locking! 🚀
    }
} else {
    // Bucket has data? Lock just this bucket
    synchronized (bucket[i].firstNode) {
        // Only threads hitting THIS bucket wait
        addToChain(bucket[i], newNode);
    }
}
```

### Critical Differences

| Feature | HashMap | ConcurrentHashMap |
|---------|---------|-------------------|
| Thread-safe | ❌ No | ✅ Yes |
| Null keys | ✅ Allowed | ❌ NOT allowed |
| Null values | ✅ Allowed | ❌ NOT allowed |
| Iteration | Fail-fast 💥 | Weakly consistent |
| Performance | 🚀 Single thread | 🚀 Multi-thread |

---

## 📖 Chapter 5: The Sorted Kingdom (TreeMap & TreeSet)

**The Story**: Everything is kept in order. The Red-Black Tree is the royal guard ensuring no side of the kingdom grows too powerful (unbalanced).

### The Red-Black Rules

```
Every citizen (node) follows these laws:
1. You are either RED 🔴 or BLACK ⚫
2. The King (root) is always BLACK ⚫
3. RED 🔴 citizens cannot have RED 🔴 children
4. Every path from King to a peasant (leaf) has same BLACK ⚫ count
```

### Why It Matters

```
Unbalanced BST (worst case):      Red-Black Tree (guaranteed):
        1                               4⚫
         \                            /    \
          2                         2🔴     6🔴
           \                       /  \    /  \
            3                    1⚫  3⚫ 5⚫  7⚫
             \
              4
               \
                5
                
Lookup: O(n) 😰                  Lookup: O(log n) 🎉
```

### TreeMap Powers

```java
TreeMap<Integer, String> map = new TreeMap<>();
map.put(5, "five");
map.put(2, "two");
map.put(8, "eight");
map.put(1, "one");

// Iteration is ALWAYS sorted!
for (var entry : map.entrySet()) {
    System.out.println(entry);  // 1, 2, 5, 8
}

// Range operations
map.subMap(2, 6);      // {2=two, 5=five}  [2, 6)
map.headMap(5);        // {1=one, 2=two}   < 5
map.tailMap(5);        // {5=five, 8=eight} >= 5
map.floorKey(4);       // 2 (largest key <= 4)
map.ceilingKey(4);     // 5 (smallest key >= 4)
```

---

## 📖 Chapter 6: The Memory Lane (LinkedHashMap)

**The Story**: Like HashMap, but with a memory. It remembers the order things happened.

### Dual Structure

```
HashMap buckets:                 + Linked List (maintains order):
┌───┬───┬───┬───┐
│ 0 │ 1 │ 2 │ 3 │               head ──▶ Alice ──▶ Bob ──▶ Carol ──▶ tail
└───┴───┴───┴───┘                       (insertion order preserved)
      │
      ▼
    [Node]
```

### LRU Cache in 10 Lines

```java
class LRUCache<K, V> extends LinkedHashMap<K, V> {
    private final int maxSize;
    
    LRUCache(int maxSize) {
        super(maxSize, 0.75f, true);  // true = ACCESS order!
        this.maxSize = maxSize;
    }
    
    @Override
    protected boolean removeEldestEntry(Map.Entry<K, V> eldest) {
        return size() > maxSize;  // Auto-evict oldest!
    }
}

// Usage:
LRUCache<String, User> cache = new LRUCache<>(100);
cache.put("user1", alice);   // [alice]
cache.put("user2", bob);     // [alice, bob]
cache.get("user1");          // Access moves to end: [bob, alice]
cache.put("user3", carol);   // If full, bob evicted: [alice, carol]
```

---

## 📖 Chapter 7: The Priority Waiting Room (PriorityQueue)

**The Story**: Not first-come-first-serve. The most urgent patient always goes next!

### Binary Heap Under the Hood

```
PriorityQueue<Integer> pq = new PriorityQueue<>();
pq.offer(5); pq.offer(2); pq.offer(8); pq.offer(1);

The array: [1, 2, 8, 5]      The logical tree:
                                    1       ← min at root
           Indexes:               /   \
           0: parent            2       8
           1, 2: children      /
           3, 4: grandkids    5

Parent of i: (i-1)/2
Children of i: 2i+1, 2i+2
```

### The Bubble Dance

```
offer(1) when heap is [2, 5, 8]:

Step 1: Add to end       Step 2: Bubble up!
      2                        1     ← 1 < 2, swap!
    /   \                    /   \
   5     8                  2     8
  /                        /
 1                        5

Array: [2,5,8,1] → [1,2,8,5]
```

---

## 📖 Chapter 8: Sorting Citizens (Comparable vs Comparator)

### Comparable: "I know my own worth"

```java
class Employee implements Comparable<Employee> {
    String name;
    int salary;
    
    @Override
    public int compareTo(Employee other) {
        return Integer.compare(this.salary, other.salary);
        // Negative: I'm smaller, Positive: I'm bigger, Zero: Equal
    }
}

Collections.sort(employees);  // Uses MY compareTo
```

### Comparator: "Let someone else decide"

```java
// Multiple ways to sort the same objects:
Comparator<Employee> byName = Comparator.comparing(Employee::getName);
Comparator<Employee> bySalary = Comparator.comparing(Employee::getSalary);
Comparator<Employee> bySalaryDesc = bySalary.reversed();

// Chain them!
Comparator<Employee> byDeptThenSalary = Comparator
    .comparing(Employee::getDepartment)
    .thenComparing(Employee::getSalary)
    .thenComparing(Employee::getName);

employees.sort(byDeptThenSalary);
```

---

## 📖 Chapter 9: The Modification Police (Fail-Fast vs Fail-Safe)

### Fail-Fast: "Stop right there!"

```java
List<String> list = new ArrayList<>(Arrays.asList("a", "b", "c"));

for (String s : list) {
    if (s.equals("b")) {
        list.remove(s);  // 💥 ConcurrentModificationException!
    }
}

// How it knows:
int expectedModCount = list.modCount;  // Saved when iterator created
// On every next(): if (modCount != expectedModCount) throw!
```

**The Safe Way:**

```java
// Use Iterator.remove()
Iterator<String> it = list.iterator();
while (it.hasNext()) {
    if (it.next().equals("b")) {
        it.remove();  // ✅ Safe!
    }
}

// Or removeIf() (Java 8+)
list.removeIf(s -> s.equals("b"));  // ✅ Clean!
```

### Fail-Safe: "You do you"

```java
CopyOnWriteArrayList<String> list = new CopyOnWriteArrayList<>();
list.add("a");

for (String s : list) {
    list.add("b");  // ✅ No exception!
    // Iterator works on a SNAPSHOT, doesn't see new "b"
}
// But writes are expensive (copies entire array)
```

---

## 🎯 The Decision Tree

```
┌─────────────────────────────────────────────────────────────────────────┐
│                     WHICH COLLECTION DO I USE?                          │
├─────────────────────────────────────────────────────────────────────────┤
│                                                                         │
│  Need key-value pairs?                                                  │
│     ├─ Yes → Need sorted keys? → Yes → TreeMap                          │
│     │                          → No  → Need thread-safe? → Yes → CHM    │
│     │                                                    → No  → HashMap│
│     └─ No ↓                                                             │
│                                                                         │
│  Need ordering?                                                         │
│     ├─ By index → ArrayList (99% of the time)                           │
│     ├─ By insertion → LinkedHashSet/Map                                 │
│     ├─ Sorted → TreeSet                                                 │
│     └─ By priority → PriorityQueue                                      │
│                                                                         │
│  Need uniqueness? → HashSet (unsorted) / TreeSet (sorted)               │
│                                                                         │
│  Need stack/queue? → ArrayDeque (NOT LinkedList!)                       │
│                                                                         │
└─────────────────────────────────────────────────────────────────────────┘
```

---

## 📖 Chapter 10: The Grand Quiz

### Q: HashMap vs Hashtable vs ConcurrentHashMap?

| Feature | HashMap | Hashtable | ConcurrentHashMap |
|---------|---------|-----------|-------------------|
| Thread-safe | ❌ | ✅ (but slow) | ✅ (fast) |
| Null key/value | ✅ / ✅ | ❌ / ❌ | ❌ / ❌ |
| Lock granularity | N/A | Entire map | Per-bucket |
| Modern? | ✅ | ❌ Legacy | ✅ |

### Q: Why is capacity always power of 2?

```java
// Allows this optimization:
index = hash & (capacity - 1);  // Bitwise AND

// Instead of:
index = hash % capacity;        // Modulo (slower)

// Example: capacity = 16
hash & 15  ==  hash & 0b1111  ==  hash % 16
```

### Q: What if equals() but not hashCode() is overridden?

```java
class BadKey {
    int id;
    @Override
    public boolean equals(Object o) {
        return o instanceof BadKey && ((BadKey)o).id == this.id;
    }
    // hashCode() NOT overridden!
}

HashMap<BadKey, String> map = new HashMap<>();
map.put(new BadKey(1), "one");
map.get(new BadKey(1));  // null! 💥

// Why? new BadKey(1) has DIFFERENT hashCode → different bucket!
```

**Rule**: If you override `equals()`, you MUST override `hashCode()`!
