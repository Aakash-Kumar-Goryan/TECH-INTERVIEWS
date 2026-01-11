# Collections Framework Deep Dive

Collections internals are a FAANG staple. Understanding how they work enables optimal data structure choices.

## 1. Collection Hierarchy

```
                    Iterable<E>
                        │
                   Collection<E>
          ┌─────────────┼─────────────┐
          │             │             │
       List<E>       Set<E>       Queue<E>
          │             │             │
    ┌─────┴─────┐  ┌────┴────┐   ┌────┴────┐
ArrayList  LinkedList  HashSet  TreeSet  PriorityQueue
                           │
                      LinkedHashSet


                      Map<K,V>
          ┌─────────────┼─────────────┐
      HashMap       TreeMap      LinkedHashMap
          │
  ConcurrentHashMap
```

---

## 2. List Implementations

### ArrayList

**Backed by**: Dynamic array (`Object[] elementData`)

```java
// Internal structure
transient Object[] elementData;
private int size;
```

**Operations**:

| Operation | Time | Notes |
|-----------|------|-------|
| `get(i)` | O(1) | Direct array access |
| `add(e)` | O(1)* | Amortized (resizing is O(n)) |
| `add(i, e)` | O(n) | Shifts elements right |
| `remove(i)` | O(n) | Shifts elements left |
| `contains(e)` | O(n) | Linear search |

**Resizing**:

- Default capacity: 10
- Grows by 50% when full: `newCapacity = oldCapacity + (oldCapacity >> 1)`
- `Arrays.copyOf()` creates new array

**Best Practice**: If size is known, use `new ArrayList<>(expectedSize)` to avoid resizing.

### LinkedList

**Backed by**: Doubly-linked list

```java
// Internal structure
private static class Node<E> {
    E item;
    Node<E> next;
    Node<E> prev;
}
transient Node<E> first;
transient Node<E> last;
```

**Operations**:

| Operation | Time | Notes |
|-----------|------|-------|
| `get(i)` | O(n) | Traversal required |
| `add(e)` | O(1) | Add to end (maintain `last`) |
| `addFirst/Last` | O(1) | Direct pointer update |
| `remove(i)` | O(n) | Find + O(1) unlink |
| `contains(e)` | O(n) | Linear search |

**When to use LinkedList**:

- Frequent add/remove at beginning
- Implementing Queue/Deque (but `ArrayDeque` is usually faster)

**Rarely use**: `ArrayList` has better cache locality and is faster in practice for most use cases.

---

## 3. HashMap Internals (Critical for FAANG)

### Structure

```java
transient Node<K,V>[] table;  // Array of buckets
transient int size;
final float loadFactor;       // Default 0.75
int threshold;                // size at which to resize
```

### How `put()` Works

1. Compute hash: `hash = key.hashCode() ^ (key.hashCode() >>> 16)`
2. Find bucket: `index = hash & (n - 1)` (n is power of 2)
3. If bucket empty: create new Node
4. If bucket occupied:
   - If key exists: update value
   - Else: add to chain (linked list or tree)

### Treeification (Java 8+)

When a bucket has **> 8 entries**, the linked list converts to a **Red-Black Tree**.

```
Bucket (before treeify):      Bucket (after treeify):
   ↓                               ↓
[Node]→[Node]→[Node]→[Node]    [TreeNode (root)]
                                  /       \
                           [TreeNode]  [TreeNode]
```

**Why?**: Linked list is O(n) for lookup. Tree is O(log n).

**Untreeify**: When bucket drops to **< 6 entries**, converts back to list.

### Resizing (Rehashing)

Triggered when `size > threshold (capacity * loadFactor)`.

- New capacity = old capacity * 2
- All entries rehashed to new positions
- **Expensive**: O(n) operation

### ConcurrentHashMap (Thread-Safe)

**Java 7**: Segment-based locking (16 segments by default).

**Java 8+**: CAS (Compare-And-Swap) + synchronized blocks per bucket.

```java
// Java 8+ put operation (simplified)
if (casTabAt(tab, i, null, new Node<>(hash, key, value))) {
    // CAS succeeded, inserted with no lock
} else {
    // Bucket exists, use synchronized on first node in bucket
    synchronized (f) {
        // Add to chain or tree
    }
}
```

**Key Differences from HashMap**:

| Feature | HashMap | ConcurrentHashMap |
|---------|---------|-------------------|
| Thread-safe | No | Yes |
| Null keys/values | Allowed | NOT allowed |
| Iteration | Fail-fast | Weakly consistent |

---

## 4. TreeMap & TreeSet

**Backed by**: Red-Black Tree (self-balancing BST)

### Red-Black Tree Properties

1. Every node is red or black
2. Root is black
3. Red nodes have black children
4. All paths from root to leaves have same number of black nodes

### Operations

| Operation | Time |
|-----------|------|
| `get(k)` | O(log n) |
| `put(k, v)` | O(log n) |
| `remove(k)` | O(log n) |
| `firstKey()`, `lastKey()` | O(log n) |

### Use Cases

- Sorted iteration required
- Range queries (`subMap`, `headMap`, `tailMap`)
- Finding floor/ceiling keys

```java
TreeMap<Integer, String> map = new TreeMap<>();
map.put(1, "one");
map.put(5, "five");
map.put(3, "three");

map.floorKey(4);   // 3 (largest key <= 4)
map.ceilingKey(2); // 3 (smallest key >= 2)
map.subMap(1, 5);  // {1=one, 3=three} (keys in [1, 5))
```

---

## 5. LinkedHashMap

**Backed by**: HashMap + Doubly-linked list

Maintains **insertion order** (or access order with flag).

### Use Case: LRU Cache

```java
class LRUCache<K, V> extends LinkedHashMap<K, V> {
    private final int capacity;
    
    public LRUCache(int capacity) {
        super(capacity, 0.75f, true); // true = access order
        this.capacity = capacity;
    }
    
    @Override
    protected boolean removeEldestEntry(Map.Entry<K, V> eldest) {
        return size() > capacity;
    }
}
```

---

## 6. Queue & Deque

### PriorityQueue

**Backed by**: Binary Min-Heap (array-based)

```java
// Parent: (i-1)/2, Left child: 2i+1, Right child: 2i+2
transient Object[] queue;
```

| Operation | Time |
|-----------|------|
| `offer(e)` | O(log n) |
| `poll()` | O(log n) |
| `peek()` | O(1) |

**Not thread-safe**. Use `PriorityBlockingQueue` for concurrency.

### ArrayDeque

- Resizable circular array
- Faster than `LinkedList` for stack/queue operations
- No capacity limits

```java
Deque<Integer> stack = new ArrayDeque<>();
stack.push(1);
stack.pop();

Deque<Integer> queue = new ArrayDeque<>();
queue.offer(1);
queue.poll();
```

---

## 7. Comparable vs Comparator

### Comparable (Natural Ordering)

```java
public class Employee implements Comparable<Employee> {
    private int id;
    
    @Override
    public int compareTo(Employee other) {
        return Integer.compare(this.id, other.id);
    }
}

Collections.sort(employees); // Uses natural ordering
```

### Comparator (Custom Ordering)

```java
// Lambda style
Comparator<Employee> byName = (e1, e2) -> e1.getName().compareTo(e2.getName());

// Method reference
Comparator<Employee> byId = Comparator.comparing(Employee::getId);

// Chaining
Comparator<Employee> byNameThenId = Comparator
    .comparing(Employee::getName)
    .thenComparing(Employee::getId);

Collections.sort(employees, byName);
```

---

## 8. Fail-Fast vs Fail-Safe

### Fail-Fast (ArrayList, HashMap)

```java
List<String> list = new ArrayList<>(Arrays.asList("a", "b", "c"));
for (String s : list) {
    list.remove(s); // ConcurrentModificationException!
}
```

**Mechanism**: Maintains `modCount`. Iterator checks if modified.

### Fail-Safe (ConcurrentHashMap, CopyOnWriteArrayList)

```java
CopyOnWriteArrayList<String> list = new CopyOnWriteArrayList<>();
list.add("a");
for (String s : list) {
    list.add("b"); // No exception, but iterator may not see "b"
}
```

---

## 9. Performance Comparison

| Collection | Add | Contains/Get | Remove | Notes |
|------------|-----|--------------|--------|-------|
| ArrayList | O(1)* | O(n) / O(1) | O(n) | Cache-friendly |
| LinkedList | O(1) | O(n) | O(n) | Extra memory per node |
| HashSet | O(1) | O(1) | O(1) | No duplicates |
| TreeSet | O(log n) | O(log n) | O(log n) | Sorted |
| HashMap | O(1) | O(1) | O(1) | Key-value |
| TreeMap | O(log n) | O(log n) | O(log n) | Sorted keys |
| PriorityQueue | O(log n) | O(1) peek | O(log n) | Heap |

*Amortized

---

## 10. Interview Questions

**Q: HashMap vs Hashtable vs ConcurrentHashMap?**

| Feature | HashMap | Hashtable | ConcurrentHashMap |
|---------|---------|-----------|-------------------|
| Thread-safe | No | Yes (synchronized methods) | Yes (fine-grained) |
| Null keys/values | Yes | No | No |
| Performance | Best (single-thread) | Poor (full lock) | Good (concurrent) |

**Q: Why is initial capacity power of 2 in HashMap?**

Allows using `hash & (n-1)` instead of `hash % n`. Bitwise AND is faster.

**Q: What happens if `equals()` but not `hashCode()` is overridden?**

Objects may be equal but placed in different buckets. `contains()` and `get()` fail.
