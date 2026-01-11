# Annotations & Reflection

Understanding how frameworks work under the hood is valuable for senior roles.

## 1. What Are Annotations?

Metadata added to code. Processed at compile-time or runtime.

```java
@Override
public String toString() {
    return "Example";
}
```

---

## 2. Built-in Annotations

### Standard Annotations

| Annotation | Purpose |
|------------|---------|
| `@Override` | Compiler checks method overrides parent |
| `@Deprecated` | Marks obsolete code |
| `@SuppressWarnings` | Suppress compiler warnings |
| `@FunctionalInterface` | Ensures exactly one abstract method |
| `@SafeVarargs` | Suppresses heap pollution warning |

### Examples

```java
@Override
public boolean equals(Object obj) { }

@Deprecated(since = "1.5", forRemoval = true)
public void oldMethod() { }

@SuppressWarnings("unchecked")
public void generic() {
    List<String> list = (List) new ArrayList();
}

@FunctionalInterface
public interface Processor {
    void process();  // Only one abstract method allowed
}
```

---

## 3. Meta-Annotations

Annotations that annotate other annotations.

### @Retention

When annotation is available:

```java
@Retention(RetentionPolicy.SOURCE)   // Discarded by compiler
@Retention(RetentionPolicy.CLASS)    // In .class file, not at runtime (default)
@Retention(RetentionPolicy.RUNTIME)  // Available via reflection
```

### @Target

Where annotation can be used:

```java
@Target(ElementType.METHOD)         // Only on methods
@Target(ElementType.TYPE)           // Class, interface, enum
@Target(ElementType.FIELD)          // Fields
@Target(ElementType.PARAMETER)      // Method parameters
@Target(ElementType.CONSTRUCTOR)    // Constructors
@Target({ElementType.METHOD, ElementType.FIELD})  // Multiple
```

### @Documented

Include in JavaDoc.

### @Inherited

Subclasses inherit annotation:

```java
@Inherited
@interface MyAnnotation { }

@MyAnnotation
class Parent { }

class Child extends Parent { }  // Also has @MyAnnotation
```

### @Repeatable (Java 8+)

Allow multiple of same annotation:

```java
@Repeatable(Schedules.class)
@interface Schedule {
    String day();
}

@interface Schedules {
    Schedule[] value();
}

@Schedule(day = "Mon")
@Schedule(day = "Wed")
public void task() { }
```

---

## 4. Custom Annotations

```java
@Retention(RetentionPolicy.RUNTIME)
@Target(ElementType.METHOD)
public @interface Test {
    String name() default "";
    int priority() default 0;
    String[] tags() default {};
}

// Usage
@Test(name = "userTest", priority = 1, tags = {"fast", "unit"})
public void testUser() { }
```

### Annotation Elements

Allowed types:

- Primitives
- String
- Class
- Enum
- Another annotation
- Arrays of above

```java
@interface Config {
    int value();                    // Primitive
    String name();                  // String
    Class<?> handler();             // Class
    Priority priority();            // Enum
    Author author();                // Annotation
    String[] tags();                // Array
}
```

### Default Values

```java
@interface Component {
    String value() default "";  // Optional, defaults to ""
}

@Component  // No value needed
class MyComponent { }
```

---

## 5. Reflection API

Inspect and modify program structure at runtime.

### Getting Class Object

```java
// Three ways
Class<?> c1 = String.class;
Class<?> c2 = "hello".getClass();
Class<?> c3 = Class.forName("java.lang.String");
```

### Inspecting Class Members

```java
Class<?> clazz = Person.class;

// Fields
Field[] fields = clazz.getDeclaredFields();  // All fields (including private)
Field[] publicFields = clazz.getFields();    // Public fields only

// Methods
Method[] methods = clazz.getDeclaredMethods();
Method method = clazz.getMethod("getName");

// Constructors
Constructor<?>[] constructors = clazz.getConstructors();
```

### Accessing Private Members

```java
Field field = clazz.getDeclaredField("name");
field.setAccessible(true);  // Bypass access check
Object value = field.get(instance);
field.set(instance, "newValue");
```

### Creating Instances

```java
// No-arg constructor
Person p1 = Person.class.getDeclaredConstructor().newInstance();

// With parameters
Constructor<Person> constructor = Person.class
    .getConstructor(String.class, int.class);
Person p2 = constructor.newInstance("John", 30);
```

### Invoking Methods

```java
Method method = Person.class.getMethod("sayHello", String.class);
Object result = method.invoke(personInstance, "World");
```

---

## 6. Processing Annotations at Runtime

```java
@Retention(RetentionPolicy.RUNTIME)
@Target(ElementType.METHOD)
@interface Loggable { }

class MyService {
    @Loggable
    public void process() { }
}

// Processing
for (Method method : MyService.class.getDeclaredMethods()) {
    if (method.isAnnotationPresent(Loggable.class)) {
        System.out.println("Method " + method.getName() + " is loggable");
    }
}
```

### Get Annotation Values

```java
@Test(name = "myTest", priority = 1)
public void testMethod() { }

Method method = getClass().getMethod("testMethod");
Test annotation = method.getAnnotation(Test.class);
String name = annotation.name();  // "myTest"
int priority = annotation.priority();  // 1
```

---

## 7. Real-World Use Cases

### JUnit

```java
@Test
public void shouldReturnTrue() {
    assertTrue(service.isValid());
}

@BeforeEach
public void setup() { }

@AfterEach
public void teardown() { }
```

### Spring Framework

```java
@RestController
@RequestMapping("/api")
public class UserController {
    
    @Autowired
    private UserService userService;
    
    @GetMapping("/users/{id}")
    public User getUser(@PathVariable Long id) {
        return userService.findById(id);
    }
}
```

**How Spring Works**:

1. Scans classpath for `@Component` annotations
2. Uses reflection to find `@Autowired` fields
3. Creates instances and injects dependencies

### Jackson JSON

```java
public class User {
    @JsonProperty("user_name")
    private String name;
    
    @JsonIgnore
    private String password;
    
    @JsonFormat(pattern = "yyyy-MM-dd")
    private Date birthDate;
}
```

### Hibernate/JPA

```java
@Entity
@Table(name = "users")
public class User {
    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    private Long id;
    
    @Column(nullable = false, length = 100)
    private String name;
    
    @OneToMany(mappedBy = "user")
    private List<Order> orders;
}
```

---

## 8. Compile-Time Annotation Processing

Process annotations during compilation (generate code, validate).

### Annotation Processor

```java
@SupportedAnnotationTypes("com.example.MyAnnotation")
@SupportedSourceVersion(SourceVersion.RELEASE_11)
public class MyProcessor extends AbstractProcessor {
    
    @Override
    public boolean process(Set<? extends TypeElement> annotations, 
                          RoundEnvironment roundEnv) {
        for (Element element : roundEnv.getElementsAnnotatedWith(MyAnnotation.class)) {
            // Generate code, validate, etc.
        }
        return true;
    }
}
```

**Examples**:

- Lombok (`@Data`, `@Builder`)
- MapStruct (DTO mapping)
- Dagger (dependency injection)

---

## 9. Reflection Performance

Reflection is **slower** than direct access:

- Method lookup on every call
- Security checks
- No JIT optimization

### Mitigation

```java
// Cache Method objects
private static final Method METHOD;
static {
    try {
        METHOD = MyClass.class.getMethod("process");
    } catch (NoSuchMethodException e) {
        throw new RuntimeException(e);
    }
}

// Use method handles (Java 7+) - faster
MethodHandle handle = MethodHandles.lookup()
    .findVirtual(MyClass.class, "process", MethodType.methodType(void.class));
```

---

## 10. Interview Questions

**Q: Difference between `getFields()` and `getDeclaredFields()`?**

| getFields() | getDeclaredFields() |
|-------------|---------------------|
| Public fields only | All fields (including private) |
| Includes inherited | This class only |

**Q: Can you invoke private methods via reflection?**

Yes, with `setAccessible(true)`:

```java
Method method = clazz.getDeclaredMethod("privateMethod");
method.setAccessible(true);
method.invoke(instance);
```

**Q: What is the purpose of @Retention?**

Defines annotation lifecycle:

- `SOURCE`: Available only in source code
- `CLASS`: In bytecode, not at runtime
- `RUNTIME`: Available via reflection

**Q: How does Spring's @Autowired work?**

1. Spring scans for beans
2. Finds `@Autowired` fields via reflection
3. Matches by type (or name with `@Qualifier`)
4. Sets field value using `field.set()`
