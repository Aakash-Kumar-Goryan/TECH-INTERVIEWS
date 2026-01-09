# Rapid Fire LLD & OOP QA (FAANG Level)

*Questions focusing on Concurrency, Design Pattern pitfalls, and Language Internals.*

## 1. Concurrency & Thread Safety

**Q: Singleton: Is it thread-safe? How to make it distinctively thread-safe?**

* **A:** Naive implementation is NOT.
* **Double-Checked Locking:** Check `instance == null` twice. Once before locking, once inside the lock.
* **Important:** The instance variable must be `volatile` (Java/C++) to prevent instruction reordering, ensuring the fully initialized object is visible to other threads.

**Q: Optimistic vs. Pessimistic Locking?**

* **Pessimistic:** "Lock the record before updating". High contention safety, lower performance. (`SELECT FOR UPDATE`).
* **Optimistic:** "Read, check version/timestamp on Write". If version changed, retry/fail. High performance in low contention. (Compare-And-Swap / CAS operations).

**Q: What is the "ABA Problem" in concurrency?**

* **A:** In CAS (Compare-And-Swap), a value changes from A -> B -> A. A thread checks if it's A and assumes nothing changed, but it *did*.
* **Fix:** Versioned pointers (stamped references) or AtomicStampedReference.

## 2. Deep OOP & Internals

**Q: Composition over Inheritance. Why? Give a specific example where Inheritance fails.**

* **The Fragile Base Class Problem:** If the parent class changes its internal implementation (even private methods potentially affecting side effects), all subclasses break.
* **Example:** Making a `CustomHashSet` by extending `HashSet` to count added elements. If you override `addAll()` to increment count and call `super.addAll()`, and `super.addAll()` internally calls `add()`, you might double-count elements if you also overrode `add()`. Composition wraps the Set and avoids this internal coupling.

**Q: Explain Covariance vs. Contravariance.**

* **Covariance:** "Producer extends" (Java `? extends T`). You can read `T` from it, but can't write (might be a specific subclass).
* **Contravariance:** "Consumer super" (Java `? super T`). You can write `T` to it, but you read `Object`.
* **Rule:** PECS (Producer Extends, Consumer Super).

**Q: What happens internally when `HashMap` reaches a collision?**

* **A:** It uses Chaining (linked list) at the bucket index.
* **Optimization (Java 8+):** If the list grows beyond a threshold (8 nodes), it converts to a **Red-Black Tree** to improve search from O(n) to O(log n).

## 3. Design Patterns in the Wild

**Q: Strategy Pattern vs. State Pattern. They look the same (Interface + Concrete Classes). What's the semantic difference?**

* **Strategy:** "How" the object performs a task. The Client usually chooses the strategy. The strategy rarely changes the context's state. (e.g., Sorting algorithm).
* **State:** "What" state the object is in. The State objects often trigger transitions to *other* States. The transition logic is hidden from the client. (e.g., Connection: Open -> Closed).

**Q: Singleton is often called an "Anti-Pattern". Why?**

* **A:**
* **A:**
    1. **Hidden Dependencies:** It acts as global state, making data flow hard to track.
    2. **Hard to Test:** Hard to mock in unit tests (state persists between tests).
    3. **Concurrency Bottleneck:** Global synchronization point.
* **Alternative:** Dependency Injection (pass the instance down).

**Q: Decorator Pattern vs. Proxy Pattern?**

* **Decorator:** Adds **behavior/responsibilities** to the object (Enhancement). (e.g., CompressedStream(FileStream)).
* **Proxy:** Controls **access** to the object. (e.g., Lazy loading, Permissions check, Logging). usually keeps the same interface without adding new functional methods.

## 4. Complex Patterns & OS Concepts

**Q: Visitor Pattern: What is it, and when to use it?**

* **Problem:** You have a stable hierarchy of Element classes (e.g., FileSystem Nodes: File, Directory), but you want to define new operations (SizeCalculation, JSONExport) without changing the classes.
* **Solution:** Double Dispatch. `Element.accept(Visitor v) -> v.visit(this);`.
* **Pro:** Adding **new operations** is easy (new Visitor).
* **Con:** Adding **new Elements** is hard (must change Visitor interface and all implementations).

**Q: Command Pattern: Why use it over direct method calls?**

* **A:** Turns a request into a stand-alone object.
* **Benefits:** Decouples Invoker from Receiver. Allows **Undo/Redo** functionality (by storing history of commands), **Queueing** commands, and **Macro** recording.

**Q: Deadlock vs. Livelock vs. Starvation?**

* **Deadlock:** Threads waiting on each other. Cyclic dependency. (0 CPU usage).
* **Livelock:** Threads changing state in response to each other but making no progress (like two people trying to pass in a hallway). (100% CPU usage).
* **Starvation:** Low priority threads never get CPU because High priority threads hog it. (Fix: Priority Aging).

**Q: Mutex vs. Semaphore?**

* **Mutex (Lock):** **Ownership** concept. Only the thread that locked it can unlock it. (Binary).
* **Semaphore:** **Signaling** concept. A counter. Thread A can increment, Thread B can decrement. No ownership. (Can be used for Rate Limiting / Pools).

## 5. System Design -> LLD Mapping

**Q: How do you implement a Rate Limiter class? (LLD focus)**

* **Design:**
  * Need a Thread-Safe Map: `ConcurrentHashMap<String, Bucket>`.
  * **Bucket Implementation:**
    * *Lazy Refill:* Store `lastRefillTimestamp` and `tokens`. On request, calculate `(now - lastRefill) * refillRate`, add to tokens, then try to subtract 1. (No background thread needed).
    * Must use **Atomic** operations or **Synchronized** blocks for calculation to avoid race conditions.

**Q: Design an LRU Cache. (Data Structures)**

* **Components:** `HashMap<Key, Node>` + `DoublyLinkedList<Node>`.
* **Why map?** O(1) access to see if key exists.
* **Why Doubly Linked List?** O(1) removal of specific node (move to head on access, remove tail on eviction).
* **Concurrency:** To make it thread-safe, you need a Read-Write Lock (ReentrantReadWriteLock), because moving nodes affects the list structure.
