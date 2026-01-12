# Generics & Type System

> *Generics look scary but are your friends. Let's demystify them through the story of type safety...*

---

## 🎬 The Problem Before Generics

**The Story**: In the old days (Java 1-4), collections were like unlabeled boxes. You never knew what you'd pull out!

```java
// The Dark Ages (Pre-Java 5)
List stuff = new ArrayList();
stuff.add("Hello");
stuff.add(42);           // No complaint! 😨
stuff.add(new Dog());    // Chaos!

String s = (String) stuff.get(1);  // 💥 ClassCastException!
// Got an Integer, expected String!
```

**The Solution**: Labels for boxes!

```java
// The Modern Era (Java 5+)
List<String> words = new ArrayList<>();
words.add("Hello");
words.add(42);           // ❌ Compile error! Caught early!
words.add(new Dog());    // ❌ Nope!

String s = words.get(0); // ✅ No cast needed, always String
```

---

## 📖 Chapter 1: Creating Generic Types

### Generic Classes

```java
// The labeled box
public class Box<T> {
    private T contents;
    
    public void put(T item) {
        this.contents = item;
    }
    
    public T get() {
        return contents;
    }
}

// Using the labeled boxes
Box<String> stringBox = new Box<>();
stringBox.put("Hello");
String s = stringBox.get();  // No cast!

Box<Integer> intBox = new Box<>();
intBox.put(42);
Integer n = intBox.get();    // No cast!

Box<Dog> dogBox = new Box<>();
dogBox.put(new Dog("Buddy"));
Dog d = dogBox.get();        // No cast!
```

### Multiple Type Parameters

```java
public class Pair<K, V> {
    private final K key;
    private final V value;
    
    public Pair(K key, V value) {
        this.key = key;
        this.value = value;
    }
    
    public K getKey() { return key; }
    public V getValue() { return value; }
}

Pair<String, Integer> age = new Pair<>("Alice", 30);
Pair<Integer, List<String>> weird = new Pair<>(1, List.of("a", "b"));
```

### Generic Methods

```java
public class Util {
    // The <T> before return type declares the type parameter
    public static <T> T firstOrNull(List<T> list) {
        return list.isEmpty() ? null : list.get(0);
    }
    
    // Multiple type parameters
    public static <K, V> Pair<V, K> swap(Pair<K, V> pair) {
        return new Pair<>(pair.getValue(), pair.getKey());
    }
}

// Type inference - compiler figures it out!
String first = Util.firstOrNull(List.of("a", "b", "c"));
Integer num = Util.firstOrNull(List.of(1, 2, 3));
```

---

## 📖 Chapter 2: The Dark Secret (Type Erasure)

**The Story**: Generics are a *compile-time illusion*. At runtime, the JVM has no idea!

### The Disappearing Act

```
┌─────────────────────────────────────────────────────────────────────────┐
│           COMPILE TIME                      RUNTIME                     │
├─────────────────────────────────────────────────────────────────────────┤
│   List<String>    ────────────────────▶    List                        │
│   List<Integer>   ────────────────────▶    List                        │
│   Box<Dog>        ────────────────────▶    Box                         │
│   T               ────────────────────▶    Object                      │
│   T extends Number ───────────────────▶    Number                      │
└─────────────────────────────────────────────────────────────────────────┘
                         TYPE ERASURE
```

### What You CAN'T Do (Because of Erasure)

```java
// ❌ Cannot create instances of type parameters
public class Box<T> {
    public T createNew() {
        return new T();  // ERROR! JVM doesn't know what T is!
    }
}

// ❌ Cannot create generic arrays
T[] array = new T[10];  // ERROR!

// ❌ Cannot use instanceof with generic types
if (list instanceof List<String>) { }  // ERROR! Type erased!

// ❌ Cannot have static fields of type T
public class Box<T> {
    private static T shared;  // ERROR! T is instance-level
}
```

### Proof of Erasure

```java
List<String> strings = new ArrayList<>();
List<Integer> integers = new ArrayList<>();

// At runtime, they're the SAME class!
System.out.println(strings.getClass() == integers.getClass());  // true!
System.out.println(strings.getClass().getName());  // java.util.ArrayList
```

### The Bridge Method Magic

```java
public class StringBox extends Box<String> {
    @Override
    public void put(String item) { }  // Your code
}

// Compiler secretly generates:
public class StringBox extends Box {
    public void put(String item) { }  // Your code
    
    // Bridge method (synthetic)
    public void put(Object item) {    // Erased signature
        put((String) item);           // Calls your method
    }
}
```

---

## 📖 Chapter 3: Bounded Type Parameters

**The Story**: "I don't want just ANY type. I want types that can do specific things!"

### Upper Bound: `extends`

```java
// T must be a Number (or subclass like Integer, Double...)
public class Statistics<T extends Number> {
    private List<T> numbers;
    
    public double average() {
        double sum = 0;
        for (T n : numbers) {
            sum += n.doubleValue();  // Can call Number methods!
        }
        return sum / numbers.size();
    }
}

Statistics<Integer> intStats = new Statistics<>();  // ✅
Statistics<Double> dblStats = new Statistics<>();   // ✅
Statistics<String> strStats = new Statistics<>();   // ❌ String isn't a Number!
```

### Multiple Bounds

```java
// T must extend Comparable AND be Serializable
public <T extends Comparable<T> & Serializable> T findMax(List<T> list) {
    return Collections.max(list);
}

// Order matters! Class first, then interfaces
<T extends SomeClass & Interface1 & Interface2>
```

---

## 📖 Chapter 4: Wildcards (The Interview Favorite!)

**The Story**: Sometimes you don't care about the exact type. You just need *something* that fits.

### Unbounded Wildcard: `<?>`

```java
// "I'll accept a list of ANYTHING"
public void printAll(List<?> list) {
    for (Object item : list) {
        System.out.println(item);
    }
}

printAll(List.of("a", "b", "c"));      // ✅
printAll(List.of(1, 2, 3));            // ✅
printAll(List.of(new Dog(), new Cat())); // ✅

// But you can't ADD anything (except null)!
List<?> mystery = new ArrayList<String>();
mystery.add("hello");  // ❌ ERROR! Could be List<Integer>!
mystery.add(null);     // ✅ This is the only thing allowed
```

### Upper-Bounded Wildcard: `<? extends T>`

```java
// "I'll accept Number or any SUBCLASS of Number"
public double sum(List<? extends Number> numbers) {
    double sum = 0;
    for (Number n : numbers) {  // Read as Number
        sum += n.doubleValue();
    }
    return sum;
}

sum(List.of(1, 2, 3));          // ✅ List<Integer>
sum(List.of(1.5, 2.5, 3.5));    // ✅ List<Double>

// Can READ as Number, but cannot WRITE!
List<? extends Number> nums = List.of(1, 2, 3);
Number n = nums.get(0);    // ✅ Read works
nums.add(42);              // ❌ Cannot add! Could be List<Double>!
```

### Lower-Bounded Wildcard: `<? super T>`

```java
// "I'll accept Integer or any SUPERCLASS of Integer"
public void addIntegers(List<? super Integer> list) {
    list.add(1);    // ✅ Can add Integers!
    list.add(2);
    list.add(3);
}

addIntegers(new ArrayList<Integer>());  // ✅
addIntegers(new ArrayList<Number>());   // ✅
addIntegers(new ArrayList<Object>());   // ✅

// Can WRITE Integers, but read returns Object
List<? super Integer> list = new ArrayList<Number>();
list.add(42);              // ✅ Write works
Object obj = list.get(0);  // Only Object, not Integer!
```

---

## 📖 Chapter 5: The PECS Rule (Memorize This!)

> **P**roducer **E**xtends, **C**onsumer **S**uper

```
┌─────────────────────────────────────────────────────────────────────────┐
│  If you're READING from the collection (it PRODUCES items):            │
│     → Use <? extends T>                                                 │
│     → "Give me anything that IS-A T"                                    │
│                                                                         │
│  If you're WRITING to the collection (it CONSUMES items):              │
│     → Use <? super T>                                                   │
│     → "Give me anything that can HOLD a T"                              │
│                                                                         │
│  If you're doing BOTH:                                                  │
│     → Use <T>                                                           │
│     → No wildcards!                                                     │
└─────────────────────────────────────────────────────────────────────────┘
```

### The Classic Example: Collections.copy()

```java
public static <T> void copy(
    List<? super T> dest,     // CONSUMER: will write T's into it
    List<? extends T> src     // PRODUCER: will read T's from it
) {
    for (T item : src) {      // Read from producer
        dest.add(item);       // Write to consumer
    }
}

// Usage:
List<Object> destination = new ArrayList<>();
List<Integer> source = List.of(1, 2, 3);
Collections.copy(destination, source);  // Works!
```

---

## 📖 Chapter 6: Covariance vs Contravariance

### The Problem with Arrays

```java
// Arrays are COVARIANT (and it's a problem!)
Number[] numbers = new Integer[10];  // Compiles! ✅
numbers[0] = 3.14;  // Compiles! But... 💥 ArrayStoreException!

// Why? The array is actually Integer[], so can't hold Double!
```

### Generics Are Invariant (Safer!)

```java
List<Number> numbers = new ArrayList<Integer>();  // ❌ Doesn't compile!
// Even though Integer extends Number!

// Why? Consider what could go wrong:
List<Integer> ints = new ArrayList<>();
List<Number> nums = ints;  // If this were allowed...
nums.add(3.14);            // We could add a Double!
Integer i = ints.get(0);   // 💥 But ints thinks it has Integers!
```

### Wildcards Enable Controlled Variance

```
┌─────────────────────────────────────────────────────────────────────────┐
│  Type                      │ Variance      │ What you can do           │
├─────────────────────────────────────────────────────────────────────────┤
│  List<Number>              │ Invariant     │ Read Number, Write Number │
│  List<? extends Number>    │ Covariant     │ Read Number, Write NOTHING│
│  List<? super Number>      │ Contravariant │ Read Object, Write Number │
└─────────────────────────────────────────────────────────────────────────┘
```

---

## 📖 Chapter 7: Common Pitfalls

### Cannot Create Generic Arrays

```java
// ❌ This fails
T[] array = new T[10];

// ✅ Workaround 1: Cast (unsafe but common)
@SuppressWarnings("unchecked")
T[] array = (T[]) new Object[10];

// ✅ Workaround 2: Pass class token
public <T> T[] createArray(Class<T> type, int size) {
    return (T[]) Array.newInstance(type, size);
}
```

### Cannot Use Primitives

```java
List<int> numbers;     // ❌ ERROR! No primitives
List<Integer> numbers; // ✅ Use wrapper
```

### Raw Types (Avoid!)

```java
List list = new ArrayList();  // ❌ Raw type! Type safety lost!
List<String> list = new ArrayList<>();  // ✅ Parameterized
```

---

## 🎯 Quick Reference: Wildcard Cheat Sheet

```
┌─────────────────────────────────────────────────────────────────────────┐
│ Wildcard              │ Read        │ Write      │ Use When            │
├─────────────────────────────────────────────────────────────────────────┤
│ List<?>               │ Object      │ null only  │ Just iterating      │
│ List<? extends T>     │ T           │ NOTHING    │ Reading T's out     │
│ List<? super T>       │ Object      │ T          │ Writing T's in      │
│ List<T>               │ T           │ T          │ Both read & write   │
└─────────────────────────────────────────────────────────────────────────┘

REMEMBER: PECS → Producer Extends, Consumer Super
```

---

## 📖 Interview Questions

**Q: Why can't you do `new T()`?**

```
Type erasure! At runtime, T becomes Object. JVM doesn't know
which constructor to call.

Workaround:
public <T> T create(Class<T> clazz) throws Exception {
    return clazz.getDeclaredConstructor().newInstance();
}
```

**Q: What's the difference between `List<?>` and `List<Object>`?**

```java
List<Object> objects = new ArrayList<>();
objects.add("string");  // ✅ Can add anything
objects.add(42);        // ✅

List<?> unknown = new ArrayList<String>();
unknown.add("string");  // ❌ Cannot add! Type unknown!

// List<?> is "a list of some specific unknown type"
// List<Object> is "a list specifically of Objects"
```
