# Annotations & Reflection

> *Annotations and reflection are the magic behind frameworks like Spring, Hibernate, and JUnit. Let's uncover how the magic works...*

---

## 🎬 The Framework Magic

Ever wondered how Spring knows to inject dependencies with just `@Autowired`? Or how JUnit finds test methods with `@Test`? The answer is **annotations + reflection**!

```java
@RestController
public class UserController {
    @Autowired  // How does Spring know to inject this? 🤔
    private UserService userService;
    
    @GetMapping("/users")  // How does Spring map this URL? 🤔
    public List<User> getUsers() { }
}
```

---

## 📖 Chapter 1: Annotations (The Labels)

**The Story**: Annotations are like Post-it notes on your code. They don't change what the code does, but tools can read them and act accordingly.

### Built-in Annotations

```java
// @Override: "Dear compiler, I'm overriding a parent method"
@Override
public String toString() {
    return "Example";
}
// If parent doesn't have toString, compiler yells! ✅

// @Deprecated: "Dear developer, don't use this anymore"
@Deprecated(since = "2.0", forRemoval = true)
public void oldMethod() { }
// IDE shows strikethrough, compiler warns ⚠️

// @SuppressWarnings: "I know what I'm doing, compiler!"
@SuppressWarnings("unchecked")
List<String> list = (List) rawList;
// No warning about unsafe cast 🤫

// @FunctionalInterface: "This must have exactly ONE abstract method"
@FunctionalInterface
public interface Calculator {
    int calculate(int a, int b);
    // Adding another abstract method = compile error! ❌
}
```

---

## 📖 Chapter 2: Creating Custom Annotations

### The Anatomy of an Annotation

```java
@Retention(RetentionPolicy.RUNTIME)  // When is it available?
@Target(ElementType.METHOD)           // Where can it be used?
public @interface Test {              // @interface = annotation!
    String name() default "";         // Element with default value
    int priority() default 0;
    String[] tags() default {};       // Arrays allowed!
}

// Usage
@Test(name = "userCreation", priority = 1, tags = {"fast", "unit"})
public void testCreateUser() { }
```

### The Lifecycle: @Retention

```
┌─────────────────────────────────────────────────────────────────────────┐
│  RetentionPolicy.SOURCE   │ Exists only in .java file                  │
│                           │ Gone after compilation                      │
│                           │ Example: @Override, @SuppressWarnings      │
├─────────────────────────────────────────────────────────────────────────┤
│  RetentionPolicy.CLASS    │ In .class file, but not at runtime         │
│  (default)                │ Used by bytecode tools                      │
├─────────────────────────────────────────────────────────────────────────┤
│  RetentionPolicy.RUNTIME  │ Available via reflection at runtime! ✨    │
│                           │ Example: Spring's @Autowired, JUnit's @Test │
└─────────────────────────────────────────────────────────────────────────┘
```

### The Placement: @Target

```java
@Target(ElementType.TYPE)           // Classes, interfaces, enums
@Target(ElementType.FIELD)          // Fields (instance variables)
@Target(ElementType.METHOD)         // Methods
@Target(ElementType.PARAMETER)      // Method parameters
@Target(ElementType.CONSTRUCTOR)    // Constructors
@Target(ElementType.LOCAL_VARIABLE) // Local variables

// Multiple targets
@Target({ElementType.FIELD, ElementType.PARAMETER})
```

### Other Meta-Annotations

```java
// @Inherited: Subclasses inherit the annotation
@Inherited
@interface Loggable { }

@Loggable
class Parent { }
class Child extends Parent { }  // Also @Loggable!


// @Repeatable: Same annotation multiple times
@Repeatable(Schedules.class)
@interface Schedule {
    String day();
}

@interface Schedules {
    Schedule[] value();  // Container
}

@Schedule(day = "Monday")
@Schedule(day = "Friday")  // Now allowed!
public void weeklyTask() { }


// @Documented: Shows up in JavaDoc
@Documented
@interface ApiVersion {
    String value();
}
```

### What Can Be Annotation Elements?

```java
@interface Config {
    // ✅ Allowed types:
    int number();                    // Primitive
    String name();                   // String
    Class<?> handler();              // Class
    Priority priority();             // Enum
    Nested nested();                 // Another annotation
    String[] tags();                 // Array of above
    
    // ❌ NOT allowed:
    // Object obj();                 // Too general
    // List<String> list();          // Collections
    // Integer wrapper();            // Wrapper classes
}
```

---

## 📖 Chapter 3: Reflection (The X-Ray Vision)

**The Story**: Reflection lets you look inside classes at runtime - see their fields, call their methods, even access private stuff! 🔍

### Getting the Class Object

```java
// Three ways to get the Class object
Class<?> c1 = String.class;              // From type itself
Class<?> c2 = "hello".getClass();        // From instance
Class<?> c3 = Class.forName("java.lang.String");  // From name

// They're all the same!
c1 == c2 && c2 == c3;  // true
```

### X-Raying a Class

```java
Class<?> clazz = Person.class;

// See all fields
Field[] allFields = clazz.getDeclaredFields();  // Private too!
Field[] publicFields = clazz.getFields();       // Public only

// See all methods  
Method[] allMethods = clazz.getDeclaredMethods();
Method getName = clazz.getMethod("getName");

// See constructors
Constructor<?>[] constructors = clazz.getConstructors();

// Example output
for (Field f : allFields) {
    System.out.printf("%s %s%n", f.getType().getSimpleName(), f.getName());
}
// String name
// int age
// List<Order> orders
```

### The Dangerous Power: Accessing Private Fields

```java
public class Secret {
    private String password = "supersecret";
}

// Normally can't access private field
Secret s = new Secret();
// s.password  → compile error!

// With reflection...
Field f = Secret.class.getDeclaredField("password");
f.setAccessible(true);  // Bypass the "private" modifier! 🔓
String password = (String) f.get(s);  // "supersecret"

// Can even CHANGE it!
f.set(s, "hacked");
```

### Creating Instances Dynamically

```java
// No-arg constructor
Person p1 = Person.class.getDeclaredConstructor().newInstance();

// With parameters
Constructor<Person> ctor = Person.class.getConstructor(String.class, int.class);
Person p2 = ctor.newInstance("Alice", 30);
```

### Invoking Methods Dynamically

```java
Person person = new Person("Bob", 25);

Method method = Person.class.getMethod("greet", String.class);
Object result = method.invoke(person, "Alice");
// Equivalent to: person.greet("Alice")
```

---

## 📖 Chapter 4: Putting It Together (How Frameworks Work)

### Mini JUnit Implementation

```java
@Retention(RetentionPolicy.RUNTIME)
@Target(ElementType.METHOD)
public @interface Test { }

// Test class
public class CalculatorTest {
    @Test
    public void testAdd() {
        assert 2 + 2 == 4;
    }
    
    @Test
    public void testMultiply() {
        assert 3 * 4 == 12;
    }
    
    public void notATest() {
        // No @Test - won't run
    }
}

// Mini test runner
public class TestRunner {
    public static void run(Class<?> testClass) throws Exception {
        Object instance = testClass.getDeclaredConstructor().newInstance();
        
        for (Method method : testClass.getDeclaredMethods()) {
            if (method.isAnnotationPresent(Test.class)) {
                System.out.println("Running: " + method.getName());
                try {
                    method.invoke(instance);
                    System.out.println("  ✅ PASSED");
                } catch (Exception e) {
                    System.out.println("  ❌ FAILED: " + e.getCause());
                }
            }
        }
    }
}

// Usage
TestRunner.run(CalculatorTest.class);
// Running: testAdd
//   ✅ PASSED
// Running: testMultiply
//   ✅ PASSED
```

### How Spring's @Autowired Works (Simplified)

```java
@Retention(RetentionPolicy.RUNTIME)
@Target(ElementType.FIELD)
public @interface Autowired { }

// Spring container simulation
public class MiniSpring {
    private Map<Class<?>, Object> beans = new HashMap<>();
    
    public void registerBean(Object bean) {
        beans.put(bean.getClass(), bean);
    }
    
    public void inject(Object target) throws Exception {
        for (Field field : target.getClass().getDeclaredFields()) {
            if (field.isAnnotationPresent(Autowired.class)) {
                Object bean = beans.get(field.getType());
                if (bean != null) {
                    field.setAccessible(true);
                    field.set(target, bean);
                    System.out.println("Injected " + field.getType().getSimpleName());
                }
            }
        }
    }
}

// Usage
public class MyController {
    @Autowired
    private UserService userService;  // Will be injected!
}

MiniSpring container = new MiniSpring();
container.registerBean(new UserServiceImpl());

MyController controller = new MyController();
container.inject(controller);
// "Injected UserServiceImpl"
```

---

## 📖 Chapter 5: Real-World Annotations

### JUnit 5

```java
@Test                           // This is a test
@BeforeEach                     // Run before each test
@AfterEach                      // Run after each test
@BeforeAll                      // Run once before all tests
@DisplayName("Should add numbers")  // Custom test name
@Disabled("Not implemented yet")     // Skip this test
@ParameterizedTest              // Data-driven test
@ValueSource(ints = {1, 2, 3})  // Test data
```

### Spring Framework

```java
@Component          // "I'm a bean, register me"
@Service            // "I'm a service layer bean"
@Repository         // "I'm a data layer bean"
@Controller         // "I'm a web controller"
@RestController     // "I'm a REST controller"
@Autowired          // "Inject a dependency here"
@Value("${key}")    // "Inject this property"
@Transactional      // "Wrap this in a transaction"
```

### Jackson (JSON)

```java
@JsonProperty("user_name")  // Map this field to "user_name" in JSON
@JsonIgnore                 // Don't include in JSON
@JsonFormat(pattern = "yyyy-MM-dd")  // Date format
@JsonCreator                // Use this constructor for deserialization
```

### JPA/Hibernate

```java
@Entity                     // "I'm a database table"
@Table(name = "users")      // "My table name is 'users'"
@Id                         // "This is my primary key"
@GeneratedValue             // "Auto-generate this value"
@Column(name = "user_name") // "Map to this column"
@OneToMany                  // "1-to-many relationship"
@ManyToOne                  // "Many-to-1 relationship"
```

---

## 📖 Chapter 6: Performance Considerations

**Reflection is SLOW!** Why?

1. **No compile-time optimization** - JIT can't inline reflective calls
2. **Security checks** - Every access is verified
3. **Method lookup** - Finding the right method each time

### Mitigation Strategies

```java
// ❌ BAD: Looking up method every time
for (int i = 0; i < 1000000; i++) {
    Method m = clazz.getMethod("process");  // Costly lookup!
    m.invoke(instance);
}

// ✅ GOOD: Cache the Method object
Method m = clazz.getMethod("process");  // Once
for (int i = 0; i < 1000000; i++) {
    m.invoke(instance);  // Reuse
}

// ✅ EVEN BETTER: MethodHandle (Java 7+)
MethodHandle handle = MethodHandles.lookup()
    .findVirtual(clazz, "process", MethodType.methodType(void.class));
    
for (int i = 0; i < 1000000; i++) {
    handle.invoke(instance);  // Much faster!
}
```

---

## 🎯 Quick Reference

```
┌─────────────────────────────────────────────────────────────────────────┐
│                 ANNOTATIONS & REFLECTION REFERENCE                       │
├─────────────────────────────────────────────────────────────────────────┤
│  @Retention(RUNTIME)  │ Available via reflection at runtime            │
│  @Retention(SOURCE)   │ Compile-time only (like @Override)             │
│  @Target(...)         │ Where annotation can be placed                  │
├─────────────────────────────────────────────────────────────────────────┤
│  getFields()          │ Public fields only (+ inherited)               │
│  getDeclaredFields()  │ All fields, private too (this class only)      │
│  setAccessible(true)  │ Bypass private modifier                        │
│  isAnnotationPresent()│ Check if annotation exists                     │
│  getAnnotation()      │ Get annotation to read values                  │
├─────────────────────────────────────────────────────────────────────────┤
│  ⚠️ Reflection is SLOW - cache Method/Field objects!                   │
└─────────────────────────────────────────────────────────────────────────┘
```
