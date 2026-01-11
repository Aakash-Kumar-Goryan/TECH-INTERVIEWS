# Modern Java Features (8-21)

Stay current with Java evolution. Interviewers expect knowledge of modern features.

## 1. Java 8 Features

### Lambda Expressions

Concise syntax for anonymous functions.

```java
// Before
Runnable r = new Runnable() {
    @Override
    public void run() {
        System.out.println("Hello");
    }
};

// After
Runnable r = () -> System.out.println("Hello");

// With parameters
Comparator<String> c = (a, b) -> a.compareTo(b);

// Multi-line
Comparator<String> c = (a, b) -> {
    int result = a.compareTo(b);
    return result;
};
```

### Functional Interfaces

An interface with exactly one abstract method.

| Interface | Method | Signature |
|-----------|--------|-----------|
| `Predicate<T>` | `test` | `T → boolean` |
| `Function<T,R>` | `apply` | `T → R` |
| `Consumer<T>` | `accept` | `T → void` |
| `Supplier<T>` | `get` | `() → T` |
| `BiFunction<T,U,R>` | `apply` | `(T, U) → R` |
| `UnaryOperator<T>` | `apply` | `T → T` |
| `BinaryOperator<T>` | `apply` | `(T, T) → T` |

```java
Predicate<String> isEmpty = s -> s.isEmpty();
Function<String, Integer> length = String::length;
Consumer<String> printer = System.out::println;
Supplier<List<String>> listFactory = ArrayList::new;
```

### Method References

Shorthand for lambdas calling existing methods.

| Type | Syntax | Lambda Equivalent |
|------|--------|-------------------|
| Static | `ClassName::staticMethod` | `x -> ClassName.staticMethod(x)` |
| Instance (bound) | `instance::method` | `x -> instance.method(x)` |
| Instance (unbound) | `ClassName::method` | `x -> x.method()` |
| Constructor | `ClassName::new` | `x -> new ClassName(x)` |

```java
// Static method
list.forEach(System.out::println);

// Instance method
String prefix = "Hello ";
list.map(prefix::concat);

// Unbound instance
list.map(String::toUpperCase);

// Constructor
list.map(StringBuilder::new);
```

### Stream API

Declarative data processing pipeline.

```java
List<String> names = people.stream()
    .filter(p -> p.getAge() > 18)           // Intermediate
    .map(Person::getName)                    // Intermediate
    .sorted()                                // Intermediate
    .distinct()                              // Intermediate
    .limit(10)                               // Intermediate
    .collect(Collectors.toList());           // Terminal
```

#### Intermediate vs Terminal Operations

| Intermediate (Lazy) | Terminal (Eager) |
|---------------------|------------------|
| `filter()` | `collect()` |
| `map()`, `flatMap()` | `forEach()` |
| `sorted()`, `distinct()` | `reduce()` |
| `limit()`, `skip()` | `count()`, `sum()` |
| `peek()` | `findFirst()`, `findAny()` |

#### Common Collectors

```java
// To List
.collect(Collectors.toList());

// To Set
.collect(Collectors.toSet());

// To Map
.collect(Collectors.toMap(Person::getId, Person::getName));

// Grouping
.collect(Collectors.groupingBy(Person::getDepartment));

// Joining strings
.collect(Collectors.joining(", "));

// Statistics
.collect(Collectors.summarizingInt(Person::getAge));
```

#### flatMap

Flattens nested structures.

```java
// List of lists → single list
List<List<String>> nested = List.of(List.of("a", "b"), List.of("c"));
List<String> flat = nested.stream()
    .flatMap(List::stream)
    .collect(Collectors.toList()); // [a, b, c]
```

### Optional

Avoid null checks, express absence of value.

```java
Optional<String> name = Optional.ofNullable(getName());

// Check and get
if (name.isPresent()) {
    System.out.println(name.get());
}

// Better: ifPresent
name.ifPresent(System.out::println);

// Default value
String result = name.orElse("default");

// Lazy default
String result = name.orElseGet(() -> computeDefault());

// Throw if empty
String result = name.orElseThrow(() -> new NotFoundException());

// Chaining
String upper = name
    .filter(n -> n.length() > 3)
    .map(String::toUpperCase)
    .orElse("SHORT");
```

**Anti-patterns**:

- `optional.get()` without `isPresent()` check
- `Optional` as method parameter
- `Optional` in fields

### Default Methods in Interfaces

```java
public interface Vehicle {
    void drive();
    
    default void honk() {
        System.out.println("Beep!");
    }
}
```

---

## 2. Java 9 Features

### Module System (JPMS)

```java
// module-info.java
module com.myapp {
    requires java.sql;
    exports com.myapp.api;
}
```

### Factory Methods for Collections

```java
List<String> list = List.of("a", "b", "c");         // Immutable
Set<String> set = Set.of("a", "b", "c");            // Immutable
Map<String, Integer> map = Map.of("a", 1, "b", 2);  // Immutable
```

### Private Interface Methods

```java
public interface MyInterface {
    default void publicMethod() {
        privateHelper();
    }
    
    private void privateHelper() {
        // Shared logic
    }
}
```

### Try-with-resources Enhancement

```java
// Java 7-8
try (BufferedReader br = new BufferedReader(new FileReader(file))) {
    // use br
}

// Java 9+: Effectively final variable
BufferedReader br = new BufferedReader(new FileReader(file));
try (br) {
    // use br
}
```

---

## 3. Java 10-11 Features

### Local Variable Type Inference (`var`)

```java
var list = new ArrayList<String>();  // Inferred as ArrayList<String>
var stream = list.stream();          // Inferred as Stream<String>

// Cannot use with:
var x;              // No initializer
var x = null;       // Cannot infer type
var x = () -> {};   // Lambda needs target type
```

### New String Methods

```java
"  hello  ".strip();          // "hello" (Unicode-aware trim)
"  hello  ".stripLeading();   // "hello  "
"line1\nline2".lines();       // Stream<String>
"ab".repeat(3);               // "ababab"
"  ".isBlank();               // true
```

### HttpClient (Java 11)

```java
HttpClient client = HttpClient.newHttpClient();

HttpRequest request = HttpRequest.newBuilder()
    .uri(URI.create("https://api.example.com"))
    .header("Content-Type", "application/json")
    .POST(HttpRequest.BodyPublishers.ofString(json))
    .build();

HttpResponse<String> response = client.send(request, 
    HttpResponse.BodyHandlers.ofString());
```

---

## 4. Java 14-17 Features

### Records (Java 16+)

Immutable data carriers with auto-generated boilerplate.

```java
// Before
public class Point {
    private final int x;
    private final int y;
    
    public Point(int x, int y) {
        this.x = x;
        this.y = y;
    }
    // getters, equals, hashCode, toString...
}

// After
public record Point(int x, int y) { }

// Usage
Point p = new Point(1, 2);
p.x();  // getter (no 'get' prefix)
```

**Records provide**:

- Constructor
- Getters (`x()`, `y()`)
- `equals()`, `hashCode()`, `toString()`
- Final fields

### Sealed Classes (Java 17)

Restrict which classes can extend.

```java
public sealed class Shape 
    permits Circle, Square, Rectangle {
}

public final class Circle extends Shape { }
public final class Square extends Shape { }
public non-sealed class Rectangle extends Shape { }
// Other classes cannot extend Shape
```

### Pattern Matching for instanceof (Java 16+)

```java
// Before
if (obj instanceof String) {
    String s = (String) obj;
    System.out.println(s.length());
}

// After
if (obj instanceof String s) {
    System.out.println(s.length());
}
```

### Switch Expressions (Java 14+)

```java
// Expression form (returns value)
String result = switch (day) {
    case MONDAY, FRIDAY -> "Work";
    case SATURDAY, SUNDAY -> "Rest";
    default -> "Unknown";
};

// With yield for complex cases
String result = switch (day) {
    case MONDAY -> {
        logMonday();
        yield "Start of week";
    }
    default -> "Other day";
};
```

### Text Blocks (Java 15+)

```java
String json = """
    {
        "name": "John",
        "age": 30
    }
    """;
```

---

## 5. Java 21 Features

### Virtual Threads (Project Loom)

Lightweight threads managed by JVM, not OS.

```java
// Create virtual thread
Thread vt = Thread.ofVirtual().start(() -> {
    // Task
});

// With executor
try (var executor = Executors.newVirtualThreadPerTaskExecutor()) {
    executor.submit(() -> handleRequest());
}
```

**Benefits**:

- Millions of threads possible
- Blocking is cheap (parks, not blocks OS thread)
- No thread pool sizing needed

**Use case**: High-throughput I/O-bound applications.

### Sequenced Collections

```java
SequencedCollection<String> list = new ArrayList<>();

list.addFirst("a");      // Add to beginning
list.addLast("z");       // Add to end
list.getFirst();         // Get first element
list.getLast();          // Get last element
list.reversed();         // Reversed view
```

### Pattern Matching for switch

```java
String format(Object obj) {
    return switch (obj) {
        case Integer i -> "int: " + i;
        case String s -> "string: " + s;
        case null -> "null";
        default -> "unknown";
    };
}
```

### Record Patterns

```java
record Point(int x, int y) {}

void printSum(Object obj) {
    if (obj instanceof Point(int x, int y)) {
        System.out.println(x + y);
    }
}
```

---

## 6. Parallel Streams

```java
list.parallelStream()
    .filter(x -> x > 10)
    .map(this::process)
    .collect(Collectors.toList());
```

### Parallel Stream Pitfalls

1. **Shared common ForkJoinPool**: One slow task blocks all parallel streams.
2. **Order not guaranteed**: Results may be in different order.
3. **Overhead**: For small data, sequential is faster.
4. **Side effects**: Avoid stateful operations.

```java
// Custom pool (workaround)
ForkJoinPool customPool = new ForkJoinPool(4);
customPool.submit(() ->
    list.parallelStream().forEach(...)
).get();
```

---

## 7. Interview Questions

**Q: Stream vs Collection?**

| Collection | Stream |
|------------|--------|
| Stores data | Computes data |
| Can iterate multiple times | Single use |
| Eager | Lazy evaluation |

**Q: What makes Optional useful?**

- Explicit nullability
- Forces handling of absent values
- Enables functional chaining
- Avoids `NullPointerException`

**Q: When to use Records vs Classes?**

- **Record**: Immutable data carrier, DTOs, value objects
- **Class**: Need mutability, complex behavior, inheritance
