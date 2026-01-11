# JVM Architecture & Memory Model

Understanding JVM internals is critical for debugging performance issues and answering FAANG-level questions.

## 1. JVM Architecture Overview

```
┌─────────────────────────────────────────────────────────────┐
│                        JVM                                  │
│  ┌─────────────┐  ┌──────────────────────────────────────┐  │
│  │ Class Loader│  │       Runtime Data Areas             │  │
│  │  Subsystem  │  │  ┌────────┐ ┌───────┐ ┌───────────┐  │  │
│  │             │  │  │ Heap   │ │Stack  │ │ Metaspace │  │  │
│  │ Bootstrap   │  │  │(Shared)│ │(per   │ │ (Shared)  │  │  │
│  │ Extension   │  │  │        │ │thread │ │           │  │  │
│  │ Application │  │  └────────┘ └───────┘ └───────────┘  │  │
│  └─────────────┘  │  ┌────────────┐ ┌────────────────┐   │  │
│                   │  │ PC Register│ │ Native Stack   │   │  │
│                   │  └────────────┘ └────────────────┘   │  │
│                   └──────────────────────────────────────┘  │
│  ┌──────────────────────────────────────────────────────┐   │
│  │              Execution Engine                        │   │
│  │   Interpreter  │  JIT Compiler  │  Garbage Collector │   │
│  └──────────────────────────────────────────────────────┘   │
└─────────────────────────────────────────────────────────────┘
```

---

## 2. Class Loader Subsystem

Loads `.class` files into memory using delegation hierarchy.

### Hierarchy (Parent-First Delegation)

| Loader | Loads | Parent |
|--------|-------|--------|
| **Bootstrap** | Core Java classes (`java.lang.*`, rt.jar) | None (native) |
| **Platform (Ext)** | Extension classes (`lib/ext`) | Bootstrap |
| **Application** | Classpath (your code, dependencies) | Platform |

### Class Loading Phases

1. **Loading**: Read `.class` bytecode, create `Class` object.
2. **Linking**:
   - *Verification*: Check bytecode validity.
   - *Preparation*: Allocate memory for static vars (default values).
   - *Resolution*: Symbolic references → direct references.
3. **Initialization**: Execute static blocks, assign static values.

### Custom ClassLoader Use Cases

- Hot reloading (Tomcat reloads WARs)
- Isolation (OSGi bundles)
- Dynamic class generation (proxies)

---

## 3. Runtime Data Areas

### Heap (Shared Across Threads)

Where **all objects** and **arrays** are allocated.

```
┌────────────────────────────────────────────────────────┐
│                         HEAP                            │
│  ┌──────────────────────────┐  ┌────────────────────┐  │
│  │       Young Generation    │  │   Old Generation   │  │
│  │  ┌─────┐ ┌─────┐ ┌─────┐ │  │   (Tenured)        │  │
│  │  │Eden │ │ S0  │ │ S1  │ │  │                    │  │
│  │  │     │ │     │ │     │ │  │                    │  │
│  │  └─────┘ └─────┘ └─────┘ │  │                    │  │
│  └──────────────────────────┘  └────────────────────┘  │
└────────────────────────────────────────────────────────┘
```

- **Eden**: New objects allocated here.
- **Survivor (S0, S1)**: Objects surviving Minor GC.
- **Old Gen**: Long-lived objects (survived multiple GC cycles).

### Metaspace (Replaced PermGen in Java 8)

Stores:

- Class metadata (method bytecode, field info)
- Static variables
- Constant pool

**Key Point**: Uses **native memory**, not heap. Grows dynamically (can cause `OutOfMemoryError: Metaspace`).

### Stack (Per Thread)

Each thread has its own stack.

- Stores **Stack Frames** (one per method call):
  - Local variables
  - Operand stack
  - Frame data (return address)
- **LIFO** structure.
- `StackOverflowError` if too deep (recursion).

### PC (Program Counter) Register

- Per thread.
- Points to current bytecode instruction being executed.

### Native Method Stack

- For native (C/C++) method calls via JNI.

---

## 4. Object Memory Layout (HotSpot JVM)

Every object in heap has:

```
┌─────────────────────────────────────┐
│           Object Header             │
│  ┌─────────────┬─────────────────┐ │
│  │ Mark Word    │ Class Pointer   │ │
│  │ (8 bytes)    │ (4/8 bytes)     │ │
│  └─────────────┴─────────────────┘ │
├─────────────────────────────────────┤
│          Instance Data              │
│  (fields, padded to alignment)      │
├─────────────────────────────────────┤
│          Padding (optional)         │
└─────────────────────────────────────┘
```

### Mark Word Contains

- HashCode (if called)
- GC age (4 bits → max 15 cycles before promotion)
- Lock state (biased, thin, fat lock)
- GC metadata

**Interview Note**: Minimum object size is ~16 bytes on 64-bit JVM.

---

## 5. String Pool (Interning)

Special memory region for string literals.

### How It Works

```java
String s1 = "hello";      // Goes to pool
String s2 = "hello";      // Returns same reference from pool
String s3 = new String("hello"); // New object on heap (NOT in pool)

System.out.println(s1 == s2);  // true (same reference)
System.out.println(s1 == s3);  // false (different objects)
System.out.println(s1 == s3.intern());  // true (intern() returns pooled)
```

### Why Strings Are Immutable

1. **Security**: Used in class loading, network connections, file paths.
2. **Thread Safety**: No synchronization needed.
3. **HashCode Caching**: Computed once, reused (HashMap key performance).
4. **String Pool**: Immutability enables safe sharing.

---

## 6. Java Memory Model (JMM)

Defines how threads interact through memory.

### The Problem

```java
// Thread 1
x = 1;
ready = true;

// Thread 2
if (ready) {
    System.out.println(x); // Might print 0!
}
```

Without proper synchronization, Thread 2 might see `ready = true` but `x = 0` due to:

- Compiler reordering
- CPU cache not flushed

### Happens-Before Relationship

A **guarantee** that memory writes by one statement are visible to another.

| Rule | Description |
|------|-------------|
| **Program Order** | Each action in a thread happens-before later actions |
| **Monitor Lock** | Unlock happens-before subsequent lock |
| **Volatile** | Write to volatile happens-before subsequent read |
| **Thread Start** | `start()` happens-before actions in started thread |
| **Thread Join** | Actions in thread happen-before `join()` returns |
| **Transitivity** | If A happens-before B, and B happens-before C, then A happens-before C |

### Volatile Keyword

```java
volatile boolean flag = false;

// Thread 1
data = 42;
flag = true;  // Memory barrier: flushes all writes

// Thread 2
if (flag) {
    // Guaranteed to see data = 42
}
```

**Volatile Provides**:

- Visibility (writes immediately visible)
- Ordering (prevents reordering around volatile)

**Volatile Does NOT Provide**:

- Atomicity (`x++` is NOT atomic even if volatile)

---

## 7. Execution Engine

### Interpreter

- Executes bytecode line by line.
- Slow but starts immediately.

### JIT Compiler (Just-In-Time)

- Compiles "hot" methods to native machine code.
- **C1 (Client)**: Quick compilation, simple optimizations.
- **C2 (Server)**: Aggressive optimizations based on profiling.
- **Tiered Compilation**: Start with C1, promote to C2 if still hot.

### Key JIT Optimizations

| Optimization | Description |
|--------------|-------------|
| **Inlining** | Replace method call with method body |
| **Escape Analysis** | Stack-allocate objects that don't escape method |
| **Dead Code Elimination** | Remove unreachable code |
| **Loop Unrolling** | Reduce loop overhead |

---

## 8. Common Interview Questions

**Q: What's the difference between Stack and Heap?**

| Stack | Heap |
|-------|------|
| Per-thread | Shared |
| Stores primitives, references | Stores objects |
| LIFO, automatic cleanup | GC managed |
| Fast access | Slower access |
| Limited size | Large |

**Q: Can you force GC?**

No. `System.gc()` is a **hint**, JVM may ignore it.

**Q: What causes `OutOfMemoryError`?**

- Heap: Too many objects, memory leak
- Metaspace: Too many classes loaded (dynamic proxies)
- Stack: Deep recursion (`StackOverflowError` first, then OOM for thread stack allocation)
