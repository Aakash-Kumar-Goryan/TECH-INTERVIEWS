# OOP Concepts & SOLID Principles

Fundamental OOP and design principles tested in every FAANG interview.

## 1. Four Pillars of OOP

### Encapsulation

Bundling data and methods that operate on that data, restricting direct access.

```java
public class BankAccount {
    private double balance;  // Hidden from outside
    
    public void deposit(double amount) {
        if (amount > 0) {
            balance += amount;
        }
    }
    
    public double getBalance() {
        return balance;  // Controlled access
    }
}
```

**Benefits**:

- Data hiding (implementation details hidden)
- Validation (control how data is modified)
- Flexibility (change internals without breaking clients)

### Abstraction

Hiding complex implementation, exposing only essential features.

```java
public interface PaymentProcessor {
    void processPayment(double amount);  // What, not how
}

public class StripeProcessor implements PaymentProcessor {
    @Override
    public void processPayment(double amount) {
        // Complex Stripe API calls hidden
    }
}
```

**Abstraction vs Encapsulation**:

- Abstraction: Hiding complexity (design level)
- Encapsulation: Hiding data (implementation level)

### Inheritance

Creating new classes based on existing ones.

```java
public class Animal {
    protected String name;
    
    public void eat() {
        System.out.println(name + " is eating");
    }
}

public class Dog extends Animal {
    public void bark() {
        System.out.println(name + " barks");
    }
    
    @Override
    public void eat() {
        System.out.println(name + " is eating dog food");
    }
}
```

**Types**:

- Single: One parent class
- Multilevel: A → B → C
- Hierarchical: A → B, A → C
- Multiple (via interfaces only)

### Polymorphism

Same interface, different implementations.

**Compile-time (Overloading)**:

```java
public class Calculator {
    public int add(int a, int b) { return a + b; }
    public double add(double a, double b) { return a + b; }
    public int add(int a, int b, int c) { return a + b + c; }
}
```

**Runtime (Overriding)**:

```java
Animal animal = new Dog();  // Reference type vs Object type
animal.eat();  // Calls Dog's eat() - resolved at runtime
```

---

## 2. Abstract Class vs Interface

### Abstract Class

```java
public abstract class Shape {
    protected String color;
    
    public Shape(String color) {
        this.color = color;
    }
    
    public abstract double area();  // Must be implemented
    
    public String getColor() {      // Can have implementation
        return color;
    }
}
```

### Interface

```java
public interface Drawable {
    void draw();  // implicitly public abstract
    
    default void print() {  // Default method (Java 8+)
        System.out.println("Printing...");
    }
    
    static void info() {  // Static method (Java 8+)
        System.out.println("Drawable interface");
    }
}
```

### Comparison

| Feature | Abstract Class | Interface |
|---------|---------------|-----------|
| Methods | Abstract + Concrete | Abstract + Default + Static |
| Variables | Any | `public static final` only |
| Constructor | Yes | No |
| Inheritance | Single | Multiple |
| Access modifiers | Any | Public only (methods) |
| Use case | IS-A relationship | CAN-DO capability |

### When to Use What

- **Abstract Class**: Shared state/behavior among related classes
- **Interface**: Contract for unrelated classes, multiple inheritance

```java
// Abstract class: related classes
abstract class Vehicle { }
class Car extends Vehicle { }
class Truck extends Vehicle { }

// Interface: unrelated classes
interface Flyable { void fly(); }
class Bird implements Flyable { }
class Airplane implements Flyable { }
```

---

## 3. SOLID Principles

### S - Single Responsibility Principle

> A class should have only one reason to change.

```java
// Bad: Multiple responsibilities
public class User {
    public void save() { /* DB logic */ }
    public void sendEmail() { /* Email logic */ }
    public String validate() { /* Validation logic */ }
}

// Good: Single responsibility
public class User {
    private String name;
    private String email;
    // Only user data
}

public class UserRepository {
    public void save(User user) { /* DB logic */ }
}

public class EmailService {
    public void sendEmail(String email, String message) { }
}

public class UserValidator {
    public boolean validate(User user) { return true; }
}
```

### O - Open/Closed Principle

> Open for extension, closed for modification.

```java
// Bad: Modify class for each new shape
public class AreaCalculator {
    public double calculate(Object shape) {
        if (shape instanceof Circle) {
            return Math.PI * ((Circle) shape).radius * ((Circle) shape).radius;
        } else if (shape instanceof Square) {
            return ((Square) shape).side * ((Square) shape).side;
        }
        // Adding new shape requires modifying this class!
        return 0;
    }
}

// Good: Extend without modifying
public interface Shape {
    double area();
}

public class Circle implements Shape {
    private double radius;
    public double area() { return Math.PI * radius * radius; }
}

public class Square implements Shape {
    private double side;
    public double area() { return side * side; }
}

public class AreaCalculator {
    public double calculate(Shape shape) {
        return shape.area();  // Works for any new Shape
    }
}
```

### L - Liskov Substitution Principle

> Subtypes must be substitutable for their base types.

```java
// Bad: Square violates Rectangle contract
public class Rectangle {
    protected int width, height;
    
    public void setWidth(int w) { width = w; }
    public void setHeight(int h) { height = h; }
    public int area() { return width * height; }
}

public class Square extends Rectangle {
    @Override
    public void setWidth(int w) { width = height = w; }
    @Override
    public void setHeight(int h) { width = height = h; }
}

// Client code breaks!
Rectangle r = new Square();
r.setWidth(5);
r.setHeight(10);
r.area();  // Expected 50, got 100!

// Good: Separate abstractions
public interface Shape {
    int area();
}

public class Rectangle implements Shape { }
public class Square implements Shape { }
```

### I - Interface Segregation Principle

> Clients should not be forced to depend on interfaces they don't use.

```java
// Bad: Fat interface
public interface Worker {
    void work();
    void eat();
    void sleep();
}

public class Robot implements Worker {
    public void work() { /* OK */ }
    public void eat() { /* Robots don't eat! */ }
    public void sleep() { /* Robots don't sleep! */ }
}

// Good: Segregated interfaces
public interface Workable {
    void work();
}

public interface Eatable {
    void eat();
}

public interface Sleepable {
    void sleep();
}

public class Human implements Workable, Eatable, Sleepable { }
public class Robot implements Workable { }
```

### D - Dependency Inversion Principle

> Depend on abstractions, not concretions.

```java
// Bad: Direct dependency on concrete class
public class OrderService {
    private MySQLDatabase db = new MySQLDatabase();  // Tight coupling
    
    public void save(Order order) {
        db.insert(order);
    }
}

// Good: Depend on abstraction
public interface Database {
    void insert(Object obj);
}

public class OrderService {
    private final Database db;  // Depend on abstraction
    
    public OrderService(Database db) {  // Inject dependency
        this.db = db;
    }
    
    public void save(Order order) {
        db.insert(order);
    }
}

// Now can use any implementation
new OrderService(new MySQLDatabase());
new OrderService(new MongoDatabase());
new OrderService(new MockDatabase());  // For testing!
```

---

## 4. Composition vs Inheritance

### Inheritance (IS-A)

```java
class Animal { }
class Dog extends Animal { }  // Dog IS-A Animal
```

**Problems with inheritance**:

- Tight coupling
- Fragile base class problem
- Limited to single inheritance
- Exposes internals to subclasses

### Composition (HAS-A)

```java
class Engine {
    void start() { }
}

class Car {
    private Engine engine;  // Car HAS-A Engine
    
    Car(Engine engine) {
        this.engine = engine;
    }
    
    void start() {
        engine.start();
    }
}
```

**Benefits of composition**:

- Loose coupling
- Flexible at runtime (swap implementations)
- Easier testing (mock dependencies)

### Rule of Thumb

> "Favor composition over inheritance" - Gang of Four

Use inheritance when there's a true IS-A relationship with shared functionality.
Use composition for code reuse between unrelated classes.

---

## 5. Object Class Methods

Every class extends `Object`. Key methods:

### equals() and hashCode()

```java
public class Person {
    private String name;
    private int age;
    
    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        Person person = (Person) o;
        return age == person.age && Objects.equals(name, person.name);
    }
    
    @Override
    public int hashCode() {
        return Objects.hash(name, age);
    }
}
```

**Contract**:

- If `a.equals(b)` then `a.hashCode() == b.hashCode()`
- If hashCodes differ, objects are definitely not equal
- If hashCodes same, objects MIGHT be equal (collision)

### toString()

```java
@Override
public String toString() {
    return "Person{name='" + name + "', age=" + age + "}";
}
```

### clone()

```java
public class Person implements Cloneable {
    @Override
    protected Person clone() throws CloneNotSupportedException {
        return (Person) super.clone();  // Shallow copy
    }
}
```

**Deep copy** requires cloning nested objects:

```java
Person clone = (Person) super.clone();
clone.address = this.address.clone();  // Deep copy address
return clone;
```

---

## 6. Interview Questions

**Q: What is method hiding vs overriding?**

- **Overriding**: Instance methods. Runtime polymorphism.
- **Hiding**: Static methods. Compile-time resolution.

```java
class Parent {
    static void staticMethod() { System.out.println("Parent"); }
}

class Child extends Parent {
    static void staticMethod() { System.out.println("Child"); }
}

Parent p = new Child();
p.staticMethod();  // Prints "Parent" - hiding, not overriding
```

**Q: Can constructors be inherited?**

No. Constructors are not inherited. Subclass must call super constructor (implicitly or explicitly).

**Q: Why is multiple inheritance not allowed?**

Diamond problem: Ambiguity when two parents have same method. Java solves with single inheritance + multiple interfaces.
