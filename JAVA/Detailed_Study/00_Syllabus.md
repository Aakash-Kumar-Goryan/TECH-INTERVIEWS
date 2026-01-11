# Java Detailed Study Syllabus (FAANG Level)

This syllabus outlines comprehensive Java topics for SDE2/3 interviews at Google, Microsoft, Amazon, and similar companies.

## Module 1: JVM Internals & Memory Management

**Overview**: Understanding how Java runs under the hood is critical for performance-related questions.

* **Core Concepts**:
  * JVM Architecture (ClassLoader, Execution Engine)
  * Runtime Data Areas (Heap, Stack, Metaspace)
  * Java Memory Model (JMM) & Happens-Before
  * String Pool & Interning
  * Object Memory Layout
* **Relevant Files**:
  * `01_JVM_Architecture_Memory.md`
  * `02_Garbage_Collection.md`

---

## Module 2: Garbage Collection

**Overview**: GC knowledge is essential for debugging memory issues and tuning production systems.

* **Core Concepts**:
  * Generational Hypothesis
  * GC Algorithms (Serial, Parallel, G1, ZGC)
  * GC Tuning & Flags
  * Memory Leak Detection
* **Relevant Files**:
  * `02_Garbage_Collection.md`

---

## Module 3: Collections Framework

**Overview**: Deep understanding of collection internals is a FAANG staple.

* **Core Concepts**:
  * List, Set, Queue, Map Hierarchies
  * ArrayList vs LinkedList Internals
  * HashMap (Buckets, Treeification, Load Factor)
  * ConcurrentHashMap (CAS, Segment Locking)
  * TreeMap (Red-Black Tree)
  * Comparable vs Comparator
* **Relevant Files**:
  * `03_Collections_Framework.md`

---

## Module 4: Concurrency & Multi-threading

**Overview**: Concurrency is asked in almost every senior Java interview.

* **Core Concepts**:
  * Thread Lifecycle & States
  * `synchronized`, `volatile`
  * Locks: ReentrantLock, ReadWriteLock, StampedLock
  * Executors & Thread Pools
  * CompletableFuture
  * Atomic Classes & CAS
  * Deadlock, Livelock, Starvation
* **Relevant Files**:
  * `04_Concurrency_Multithreading.md`

---

## Module 5: Modern Java (8-21)

**Overview**: Keep up with Java evolution - interviewers expect modern Java knowledge.

* **Core Concepts**:
  * Lambdas & Functional Interfaces
  * Stream API (Intermediate, Terminal, Lazy Evaluation)
  * Optional
  * Records, Sealed Classes (Java 17)
  * Virtual Threads (Java 21)
* **Relevant Files**:
  * `05_Java8_Plus_Features.md`

---

## Module 6: OOP & Design Principles

**Overview**: Fundamental OOP and SOLID principles are tested directly or through LLD.

* **Core Concepts**:
  * Four Pillars (Encapsulation, Abstraction, Inheritance, Polymorphism)
  * SOLID Principles
  * Abstract Class vs Interface
  * Composition vs Inheritance
* **Relevant Files**:
  * `06_OOP_SOLID_Principles.md`

---

## Module 7: Type System & Generics

**Overview**: Generics questions test deep language understanding.

* **Core Concepts**:
  * Type Erasure
  * Bounded Types & Wildcards
  * PECS Principle
  * Covariance vs Contravariance
* **Relevant Files**:
  * `07_Generics_Type_System.md`

---

## Module 8: Exception Handling

**Overview**: Proper exception handling separates junior from senior developers.

* **Core Concepts**:
  * Checked vs Unchecked Exceptions
  * try-with-resources
  * Best Practices
  * Custom Exceptions
* **Relevant Files**:
  * `08_Exception_Handling.md`

---

## Module 9: Annotations & Reflection

**Overview**: Understanding how frameworks work under the hood.

* **Core Concepts**:
  * Built-in Annotations
  * Custom Annotations
  * Reflection API
  * Framework Use Cases
* **Relevant Files**:
  * `09_Annotations_Reflection.md`

---

## Module 10: Design Patterns

**Overview**: Patterns are asked frequently, especially in LLD rounds.

* **Core Concepts**:
  * Creational: Singleton, Factory, Builder
  * Structural: Adapter, Decorator, Proxy
  * Behavioral: Strategy, Observer, Template
* **Relevant Files**:
  * `10_Design_Patterns_Java.md`

---

## Module 11: Interview Problems

**Overview**: Common coding problems that test Java-specific knowledge.

* **Topics**:
  * LRU Cache Implementation
  * Custom HashMap
  * Producer-Consumer
  * Thread-safe Singleton
  * Immutable Class
* **Relevant Files**:
  * `11_Common_Interview_Problems.md`
