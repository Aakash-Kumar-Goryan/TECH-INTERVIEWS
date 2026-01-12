# Modern Java Features (8-21)

> *Java has evolved dramatically. Let's explore modern features through the story of how Java became more expressive, safe, and powerful...*

---

## 🎬 The Java Evolution Story

```
2014 ─── Java 8 ─── The Functional Revolution
                    Lambdas, Streams, Optional
                    
2017 ─── Java 9 ─── The Modular Era
                    Modules, Collection factories
                    
2018 ─── Java 10-11 ─── Quality of Life
                    var, new String methods, HttpClient
                    
2021 ─── Java 17 (LTS) ─── Modern Java
                    Records, Sealed Classes, Pattern Matching
                    
2023 ─── Java 21 (LTS) ─── The Performance Era
                    Virtual Threads, Structured Concurrency
```

---

## 📖 Chapter 1: The Functional Revolution (Java 8)

**The Story**: Java decided to stop being so verbose. "Why write 10 lines when 1 will do?"

### Before & After: Lambda Expressions

```java
// THE DARK AGES (Before Java 8)
button.addActionListener(new ActionListener() {
    @Override
    public void actionPerformed(ActionEvent e) {
        System.out.println("Clicked!");
    }
});

// THE ENLIGHTENMENT (Java 8+)
button.addActionListener(e -> System.out.println("Clicked!"));

// From 5 lines to 1! 🎉
```

### Lambda Syntax Cheat Sheet

```java
// No parameters
Runnable r = () -> System.out.println("Hello");

// One parameter (parentheses optional)
Consumer<String> c = s -> System.out.println(s);

// Multiple parameters
BiFunction<Integer, Integer, Integer> add = (a, b) -> a + b;

// Multi-line (use braces and return)
Comparator<String> comp = (a, b) -> {
    int lenDiff = a.length() - b.length();
    return lenDiff != 0 ? lenDiff : a.compareTo(b);
};
```

### The Functional Toolkit

```
┌─────────────────────────────────────────────────────────────────────────┐
│ Interface        │ Method    │ Signature      │ Use Case              │
├─────────────────────────────────────────────────────────────────────────┤
│ Predicate<T>     │ test      │ T → boolean    │ Filtering             │
│ Function<T,R>    │ apply     │ T → R          │ Transforming          │
│ Consumer<T>      │ accept    │ T → void       │ Side effects          │
│ Supplier<T>      │ get       │ () → T         │ Lazy generation       │
│ BiFunction<T,U,R>│ apply     │ (T,U) → R      │ Two inputs            │
│ UnaryOperator<T> │ apply     │ T → T          │ Same type transform   │
│ BinaryOperator<T>│ apply     │ (T,T) → T      │ Reducing              │
└─────────────────────────────────────────────────────────────────────────┘
```

### Method References: Even Shorter

```java
// Lambda                          Method Reference
x -> System.out.println(x)    →   System.out::println    (static)
x -> obj.process(x)           →   obj::process           (bound instance)
x -> x.toUpperCase()          →   String::toUpperCase    (unbound instance)
x -> new ArrayList<>(x)       →   ArrayList::new         (constructor)
```

---

## 📖 Chapter 2: The Stream Pipeline (Still Java 8)

**The Story**: Instead of writing loops, describe WHAT you want. The Stream does the HOW.

### The Old Way vs The New Way

```java
// THE LOOP ERA 🔄
List<String> result = new ArrayList<>();
for (Person p : people) {
    if (p.getAge() > 18) {
        String name = p.getName().toUpperCase();
        result.add(name);
    }
}
Collections.sort(result);
return result;

// THE STREAM ERA 🌊
return people.stream()
    .filter(p -> p.getAge() > 18)     // Keep adults
    .map(p -> p.getName().toUpperCase()) // Transform to uppercase name
    .sorted()                          // Sort alphabetically
    .collect(Collectors.toList());    // Gather into list
```

### The Pipeline Visualization

```
┌─────────────────────────────────────────────────────────────────────────┐
│                        STREAM PIPELINE                                   │
│                                                                         │
│  Source              Intermediate Operations           Terminal         │
│    │                         │                           │             │
│    ▼                         ▼                           ▼             │
│  ┌─────┐   ┌────────┐   ┌─────────┐   ┌─────────┐   ┌─────────┐       │
│  │List │──▶│ filter │──▶│  map    │──▶│  sorted │──▶│ collect │──▶ Result
│  └─────┘   └────────┘   └─────────┘   └─────────┘   └─────────┘       │
│              (lazy)       (lazy)        (lazy)       (TRIGGERS!)       │
│                                                                         │
│  Nothing happens until terminal operation! 🎯                           │
└─────────────────────────────────────────────────────────────────────────┘
```

### Essential Operations

```java
// FILTERING
.filter(x -> x > 5)           // Keep elements matching predicate
.distinct()                    // Remove duplicates
.limit(10)                     // Keep first 10
.skip(5)                       // Skip first 5

// TRANSFORMING
.map(x -> x * 2)              // Transform each element
.flatMap(list -> list.stream()) // Flatten nested structures

// SORTING
.sorted()                      // Natural order
.sorted(Comparator.reverseOrder())

// TERMINAL (triggers execution)
.collect(Collectors.toList()) // Gather to collection
.forEach(System.out::println) // Side effect each
.count()                       // Count elements
.reduce(0, (a,b) -> a + b)    // Combine to single value
.findFirst()                   // Get first (Optional)
.anyMatch(x -> x > 10)        // True if any match
```

### Collectors: The Grand Finale

```java
// Group people by department
Map<String, List<Person>> byDept = people.stream()
    .collect(Collectors.groupingBy(Person::getDepartment));

// Count per department
Map<String, Long> countByDept = people.stream()
    .collect(Collectors.groupingBy(
        Person::getDepartment,
        Collectors.counting()
    ));

// Join strings
String names = people.stream()
    .map(Person::getName)
    .collect(Collectors.joining(", ", "[", "]"));
// → "[Alice, Bob, Carol]"

// Statistics
IntSummaryStatistics stats = people.stream()
    .collect(Collectors.summarizingInt(Person::getAge));
// stats.getAverage(), stats.getMax(), stats.getCount()...
```

### flatMap: Flattening the Nested

```java
// Problem: List of lists
List<List<String>> nested = List.of(
    List.of("a", "b"),
    List.of("c", "d", "e"),
    List.of("f")
);

// Goal: Single flat list
List<String> flat = nested.stream()
    .flatMap(List::stream)     // Each inner list → stream → merged
    .collect(Collectors.toList());
// → ["a", "b", "c", "d", "e", "f"]

// Visualization:
// [[a,b], [c,d,e], [f]]
//    ↓ flatMap
// [a, b, c, d, e, f]
```

---

## 📖 Chapter 3: The Null Killer (Optional)

**The Story**: Tony Hoare called null his "billion dollar mistake." Optional is Java's fix.

### The Problem

```java
// The NPE minefield 💣
String city = user.getAddress().getCity().toUpperCase();
// What if user is null? Address is null? City is null?
// 💥 NullPointerException!

// The ugly fix:
String city = null;
if (user != null) {
    Address addr = user.getAddress();
    if (addr != null) {
        String c = addr.getCity();
        if (c != null) {
            city = c.toUpperCase();
        }
    }
}
```

### The Optional Solution

```java
Optional<String> city = Optional.ofNullable(user)
    .map(User::getAddress)
    .map(Address::getCity)
    .map(String::toUpperCase);

// Provide default
String result = city.orElse("UNKNOWN");

// Or throw
String result = city.orElseThrow(() -> 
    new NotFoundException("No city!"));
```

### Optional Methods

```java
Optional<String> opt = Optional.ofNullable(getValue());

// Check presence
opt.isPresent()                  // true if value exists
opt.isEmpty()                    // true if empty (Java 11+)

// Get value safely
opt.orElse("default")            // Value or default
opt.orElseGet(() -> compute())   // Value or lazy compute
opt.orElseThrow()                // Value or NoSuchElementException

// Transform
opt.map(String::toUpperCase)     // Transform if present
opt.filter(s -> s.length() > 3)  // Keep if matches
opt.flatMap(this::getOptional)   // For nested Optionals

// Execute if present
opt.ifPresent(System.out::println)
opt.ifPresentOrElse(              // Java 9+
    System.out::println,
    () -> System.out.println("Empty!")
);
```

### Optional Anti-Patterns ⚠️

```java
// ❌ BAD: Optional.get() without check
optional.get();  // May throw NoSuchElementException!

// ❌ BAD: Optional as method parameter
void process(Optional<String> name) { }  // Just use @Nullable

// ❌ BAD: Optional in fields
class User {
    Optional<Address> address;  // Just use nullable field
}

// ❌ BAD: Wrapping collections
Optional<List<String>> items;  // Use empty list instead!

// ✅ GOOD: Return type for "may not exist"
Optional<User> findById(long id);
```

---

## 📖 Chapter 4: Modern Conveniences (Java 9-11)

### Collection Factory Methods (Java 9)

```java
// Before: Verbose! 😩
List<String> list = new ArrayList<>();
list.add("a");
list.add("b");
list.add("c");
list = Collections.unmodifiableList(list);

// After: Clean! 😊
List<String> list = List.of("a", "b", "c");  // Immutable!
Set<String> set = Set.of("a", "b", "c");
Map<String, Integer> map = Map.of("a", 1, "b", 2);

// ⚠️ These are IMMUTABLE - .add() throws UnsupportedOperationException!
```

### var: Local Type Inference (Java 10)

```java
// Compiler figures out the type
var list = new ArrayList<String>();  // ArrayList<String>
var stream = list.stream();          // Stream<String>
var entry = map.entrySet().iterator().next();  // Map.Entry<K,V>

// Great for long generic types!
var complexThing = new HashMap<String, List<Map<Integer, String>>>();

// ⚠️ Cannot use:
var x;                 // No initializer
var x = null;          // Cannot infer
var x = () -> {};      // Lambda needs target type
var x = {1, 2, 3};     // Array initializer needs type
```

### New String Methods (Java 11)

```java
"  hello  ".strip()         // "hello" (Unicode-aware!)
"  hello  ".stripLeading()  // "hello  "
"  hello  ".stripTrailing() // "  hello"

"".isBlank()                // true (empty or whitespace)
"   ".isBlank()             // true

"ab".repeat(3)              // "ababab"

"line1\nline2\nline3".lines()  // Stream<String>

// Java 12+
"  hello  ".indent(4)       // "      hello  \n"
"hello".transform(s -> s.toUpperCase())  // "HELLO"
```

---

## 📖 Chapter 5: The Data Revolution (Java 14-17)

### Records: Data Classes Done Right (Java 16+)

**The Story**: "I just want to hold some data. Why do I need 50 lines of boilerplate?"

```java
// THE OLD WAY: 30+ lines! 😫
public final class Point {
    private final int x;
    private final int y;
    
    public Point(int x, int y) {
        this.x = x;
        this.y = y;
    }
    
    public int getX() { return x; }
    public int getY() { return y; }
    
    @Override public boolean equals(Object o) { /* ... */ }
    @Override public int hashCode() { /* ... */ }
    @Override public String toString() { /* ... */ }
}

// THE NEW WAY: 1 line! 🎉
public record Point(int x, int y) { }

// You get:
// ✅ Constructor
// ✅ Getters: point.x(), point.y() (no 'get' prefix!)
// ✅ equals(), hashCode(), toString()
// ✅ Immutability (fields are final)
```

### Records with Validation

```java
public record User(String name, int age) {
    // Compact constructor for validation
    public User {
        if (age < 0) throw new IllegalArgumentException("Age must be positive");
        name = name.trim();  // Can modify parameters
    }
    
    // Additional methods allowed
    public String greeting() {
        return "Hello, " + name;
    }
}
```

### Sealed Classes: Controlled Inheritance (Java 17)

```java
// Only these three can extend Shape!
public sealed class Shape 
    permits Circle, Square, Rectangle { }

public final class Circle extends Shape { }      // No further extension
public final class Square extends Shape { }
public non-sealed class Rectangle extends Shape { }  // Anyone can extend

// Why? Enables exhaustive pattern matching:
double area = switch (shape) {
    case Circle c -> Math.PI * c.radius() * c.radius();
    case Square s -> s.side() * s.side();
    case Rectangle r -> r.width() * r.height();
    // No default needed! Compiler knows these are ALL cases
};
```

### Pattern Matching for instanceof (Java 16+)

```java
// Before: Cast after check 😒
if (obj instanceof String) {
    String s = (String) obj;  // Redundant!
    System.out.println(s.length());
}

// After: Check + cast in one! 🎉
if (obj instanceof String s) {
    System.out.println(s.length());  // s is already String!
}

// Even in boolean expressions
if (obj instanceof String s && s.length() > 5) {
    // s is in scope here
}
```

### Switch Expressions (Java 14+)

```java
// Old switch: Statement, fall-through danger
String result;
switch (day) {
    case MONDAY:
    case FRIDAY:
        result = "Work";
        break;  // Forget this = bug!
    case SATURDAY:
    case SUNDAY:
        result = "Rest";
        break;
    default:
        result = "Unknown";
}

// New switch: Expression, returns value!
String result = switch (day) {
    case MONDAY, FRIDAY -> "Work";
    case SATURDAY, SUNDAY -> "Rest";
    default -> "Unknown";
};

// With complex logic: use yield
String result = switch (day) {
    case MONDAY -> {
        logMonday();
        yield "Start of week";  // Like return for switch
    }
    default -> "Other";
};
```

### Text Blocks (Java 15+)

```java
// Before: Escape hell 😵
String json = "{\n" +
    "  \"name\": \"John\",\n" +
    "  \"age\": 30\n" +
    "}";

// After: Clean and readable! 📋
String json = """
    {
        "name": "John",
        "age": 30
    }
    """;

// The closing """ position determines indentation stripping!
```

---

## 📖 Chapter 6: The Performance Era (Java 21)

### Virtual Threads: Million Threads, No Problem! 🚀

**The Story**: OS threads are expensive. Virtual threads are cheap. Run millions!

```java
// Traditional thread: Heavy, OS-managed
Thread osThread = new Thread(() -> handleRequest());
osThread.start();  // Costs ~1MB stack each!

// Virtual thread: Light, JVM-managed
Thread virtualThread = Thread.ofVirtual().start(() -> {
    handleRequest();  // Blocking is now cheap!
});

// The killer use case: I/O bound servers
try (var executor = Executors.newVirtualThreadPerTaskExecutor()) {
    for (int i = 0; i < 1_000_000; i++) {
        executor.submit(() -> {
            // Each request gets its own virtual thread!
            fetchFromDatabase();  // Blocks, but no problem!
            callExternalAPI();    // Virtual thread "parks"
        });
    }
}
```

### Virtual Threads Visualized

```
┌─────────────────────────────────────────────────────────────────────────┐
│  Platform Threads (Old)           Virtual Threads (New)                 │
│  =====================           =====================                 │
│                                                                         │
│  OS Thread 1 ─┬─ Task 1          Carrier Thread 1 ─┬─ VT 1              │
│               │  (blocked)                         │  VT 2              │
│               │                                    │  VT 3              │
│  OS Thread 2 ─┬─ Task 2                            │  VT 4 (parked)     │
│               │  (blocked)                         │  ...               │
│               │                                    │  VT 1000           │
│  OS Thread 3 ─┬─ Task 3          Carrier Thread 2 ─┬─ VT 1001           │
│                                                    │  ...               │
│  10 threads = 10 tasks           10 threads = 1,000,000 tasks! 🎉      │
└─────────────────────────────────────────────────────────────────────────┘
```

### Structured Concurrency (Preview)

```java
// Problem: Forking tasks, one fails, others keep running
CompletableFuture<User> userF = fetchUserAsync();
CompletableFuture<Orders> ordersF = fetchOrdersAsync();
// If userF fails, ordersF keeps running... wasting resources!

// Solution: Structured Concurrency
UserData fetchUserData(long id) throws Exception {
    try (var scope = new StructuredTaskScope.ShutdownOnFailure()) {
        
        Subtask<User> user = scope.fork(() -> fetchUser(id));
        Subtask<Orders> orders = scope.fork(() -> fetchOrders(id));
        
        scope.join();           // Wait for both
        scope.throwIfFailed();  // Propagate any failure
        
        // If either failed, the other was cancelled!
        return new UserData(user.get(), orders.get());
    }
    // Clean exit: all tasks complete or cancelled
}
```

### Sequenced Collections

```java
// Finally! First/last access for all ordered collections
SequencedCollection<String> list = new ArrayList<>();

list.addFirst("a");       // Add to beginning
list.addLast("z");        // Add to end
list.getFirst();          // Get first (was list.get(0))
list.getLast();           // Get last (was list.get(list.size()-1))
list.removeFirst();       // Remove first
list.reversed();          // Reversed view

// Also works for maps!
SequencedMap<K,V> map = new LinkedHashMap<>();
map.firstEntry();
map.lastEntry();
```

### Pattern Matching for Switch

```java
Object obj = getObject();

String result = switch (obj) {
    case Integer i -> "Integer: " + i;
    case String s when s.length() > 5 -> "Long string: " + s;  // Guarded!
    case String s -> "Short string: " + s;
    case null -> "It's null!";  // Null handling!
    default -> "Unknown";
};

// With records (record patterns)
record Point(int x, int y) {}

switch (obj) {
    case Point(int x, int y) -> System.out.println("x=" + x + ", y=" + y);
    // Destructured! No need for p.x(), p.y()
}
```

---

## 📖 Chapter 7: Parallel Streams (Use With Care!)

```java
// Sequential (single thread)
list.stream()
    .filter(...)
    .collect(...);

// Parallel (multiple threads)
list.parallelStream()
    .filter(...)
    .collect(...);
```

### The Parallel Trap ⚠️

```
┌─────────────────────────────────────────────────────────────────────────┐
│  PARALLEL STREAMS: PROCEED WITH CAUTION                                 │
├─────────────────────────────────────────────────────────────────────────┤
│                                                                         │
│  ❌ Small collections: Overhead > benefit                               │
│  ❌ Shared mutable state: Race conditions!                              │
│  ❌ Order-dependent operations: Results may differ                      │
│  ❌ Blocking operations: Starves common ForkJoinPool                    │
│                                                                         │
│  ✅ Large collections (10,000+ elements)                                │
│  ✅ CPU-bound operations (computation heavy)                            │
│  ✅ Stateless, independent operations                                   │
│                                                                         │
└─────────────────────────────────────────────────────────────────────────┘
```

---

## 🎯 Java Version Decision Tree

```
┌─────────────────────────────────────────────────────────────────────────┐
│                     WHICH JAVA TO USE?                                   │
├─────────────────────────────────────────────────────────────────────────┤
│                                                                         │
│  New Project (2024+)?                                                   │
│     └─▶ Java 21 (LTS) - Virtual threads, pattern matching, records     │
│                                                                         │
│  Existing Project Upgrade?                                              │
│     └─▶ Java 17 (LTS) - Records, sealed classes, good stability        │
│                                                                         │
│  Legacy Constraints?                                                    │
│     └─▶ Java 11 (LTS) - Still widely supported, var, HttpClient        │
│                                                                         │
│  ⚠️ Avoid: Java 8 for new projects (EOL, missing modern features)      │
│                                                                         │
└─────────────────────────────────────────────────────────────────────────┘
```

---

## 📖 Quick Reference: Feature by Version

| Version | Key Features |
|---------|--------------|
| **Java 8** | Lambdas, Streams, Optional, default methods |
| **Java 9** | Modules, Collection factories, private interface methods |
| **Java 10** | `var` for local variables |
| **Java 11** | HttpClient, String methods, single-file execution |
| **Java 14** | Switch expressions, Records (preview) |
| **Java 15** | Text blocks |
| **Java 16** | Records, Pattern matching instanceof |
| **Java 17** | Sealed classes (LTS) |
| **Java 21** | Virtual threads, Pattern matching switch, Sequenced collections (LTS) |
