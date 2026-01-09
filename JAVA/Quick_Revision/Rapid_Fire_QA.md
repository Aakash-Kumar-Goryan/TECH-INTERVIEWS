# Rapid Fire Java QA (FAANG Level)

*Deep questions on JVM Internals, Engineering, and Concurrency.*

## 1. JVM & Memory Internals

**Q: Can a Java application have a Memory Leak? Explain how.**

* **A:** Yes. GC removes *unreachable* objects. A leak happens when you hold references to objects you no longer need.
* **Common Causes:**
    1. **Static Collections:** `static List` grows forever.
    2. **Unclosed Resources:** Streams/Connections not closed in `finally/try-with-resources`.
    3. **ThreadLocal:** Not removed after thread execution (common in thread pools).
    4. **Listeners/Callbacks:** Registering without deregistering.

**Q: `StackOverflowError` vs. `OutOfMemoryError`?**

* **StackOverflow:** Stack depth exceeded. Infinite recursion or deep cyclic dependency.
* **OutOfMemory (OOM):** Heap is full, and GC cannot reclaim enough space. Can also happen in Metaspace (too many classes loaded).

**Q: How does String Constant Pool work? Why is String immutable?**

* **immutable:** Security (Class loading, connection, passwords), HashCode caching (Map keys), and Thread Safety.
* **Internals:** Strings created with `""` go to the Pool (Special area in Heap). `new String("")` forces a new object on Heap. `String.intern()` moves/finds it in the Pool.

## 2. Concurrency & Threads

**Q: What is the "Happens-Before" relationship in JMM?**

* **A:** It guarantees that memory writes by one specific statement are visible to another specific statement.
* **Rules:**
    1. Monitor Lock (unlock happens-before lock).
    2. Volatile Write happens-before Volatile Read.
    3. Thread Start happens-before any action in the started thread.

**Q: `synchronized` vs. `ReentrantLock`. Why use Lock?**

* **A:** `synchronized` is cleaner/safer (automatic release).
* **Use ReentrantLock when:**
    1. Need **Fairness** (`new ReentrantLock(true)` - longest waiting thread gets lock).
    2. Need `tryLock()` (don't want to block forever).
    3. Need multiple Conditions (`lock.newCondition()`) for finer wait/notify logic.
    4. Need `lockInterruptibly()`.

**Q: Explain "False Sharing" in multi-threading.**

* **A:** When two threads modify independent variables that happen to be on the **same Cache Line** (usually 64 bytes). The CPU cores invalidate each other's cache lines unnecessarily, killing performance.
* **Fix:** Padding (Adding unused variables) or `@Contended` (Java 8+).

**Q: What is a "ThreadLocal"?**

* **A:** Variables that are local to the thread (Map<Thread, Value>). Each thread has its own copy.
* **Use case:** TransactionContext, UserSession in web apps.
* **Risk:** Memory leak if using Thread Pools (Thread never dies, value stays). must `.remove()`.

## 3. Java Language Specifics

**Q: Why are Generics "Erased" at runtime? What is the limitation?**

* **A:** Backward compatibility with pre-Java 5 code.
* **Limitation:** You cannot do `new T()`, `new T[]`, or `instanceof T`. Metadata is lost. Runtime only sees `Object` (or bound).

**Q: What is the difference between `ClassNotFoundException` and `NoClassDefFoundError`?**

* **ClassNotFoundException:** Checked Exception. You tried `Class.forName()` or `loadClass()` and it wasn't there. (Dynamic loading fail).
* **NoClassDefFoundError:** Error. The class was present at compile time but missing at runtime (e.g., jar missing, static init failed).

**Q: `equals()` and `hashCode()` contract. What happens if you break it?**

* **Contract:** If `a.equals(b)`, then `a.hashCode() == b.hashCode()`.
* **Breakage:** If distinct, HashMaps fail. You put an object in, but can't retrieve it because the map looks in the wrong bucket (hash changed) or can't find equality (equals broken).

## 4. Compilation & Optimization

**Q: JIT (Just-In-Time) Compiler: What is C1 vs C2?**

* **A:** JVM interprets bytecode first, then compiles "hot" spots to native machine code.
* **C1 (Client Compiler):** Fast start-up, simple optimizations.
* **C2 (Server Compiler):** Slower start-up, aggressive (expensive) optimizations based on profiling.
* **Tiered Compilation:** Starts with C1, then promotes to C2 if method remains hot.

**Q: Does Java have "Tail Call Optimization"?**

* **A:** **No.** (Unlike Scala/Kotlin on JVM which can trick it, or functional languages).
* **Consequence:** Deep recursion will throw `StackOverflowError`. You must convert recursion to Iteration for deep stacks.

**Q: What is "Escape Analysis"?**

* **A:** Optimization technique where the JIT analyzes the scope of a new object.
* **Optimization:** If an object never "escapes" the method (not returned, not assigned to static), the JIT might allocate it on the **Stack** instead of the Heap (Scalar Replacement). This reduces GC pressure.

## 5. Advanced & Streams

**Q: How to debug a CPU spike in a Java App?**

1. Convert Thread ID to Hex. Search in dump. See exact method causing loop/work.

**Q: Parallel Streams pitfalls?**

* **A:** They use the **common** `ForkJoinPool`. Even one slow I/O task in a parallel stream can block ALL parallel streams in the entire JVM.
* **Rule:** Only use for CPU-intensive, non-blocking tasks. Better to use custom ThreadPool.

**Q: Fail-Fast vs. Fail-Safe Iterators?**

* **Fail-Fast:** `ArrayList`, `HashMap`. Throws `ConcurrentModificationException` if collection modified during iteration.
* **Fail-Safe:** `ConcurrentHashMap`, `CopyOnWriteArrayList`. Works on a clone or weakly consistent view. No exception, but might not see latest data.
