# OOP Concepts & SOLID Principles

> *Object-Oriented Programming is the foundation of Java. Let's explore OOP through the story of building maintainable, scalable software...*

---

## 🎬 The OOP Story

Imagine you're building a zoo management system. Each animal has properties (name, age, species) and behaviors (eat, sleep, make sound). This is the essence of OOP!

---

## 📖 Chapter 1: The Four Pillars of OOP

### Pillar 1: Encapsulation (The Capsule)

**The Story**: A bank account doesn't let you directly change the balance. You must use deposit/withdraw - which enforce rules!

```java
// ❌ BAD: Anyone can corrupt the balance
public class BrokenAccount {
    public double balance;  // Public = chaos!
}

account.balance = -1000000;  // 💥 Now the bank owes YOU money!

// ✅ GOOD: Controlled access
public class SecureAccount {
    private double balance;  // Hidden inside the capsule
    
    public void deposit(double amount) {
        if (amount > 0) {         // Rule enforced!
            balance += amount;
        }
    }
    
    public void withdraw(double amount) {
        if (amount > 0 && amount <= balance) {  // Can't overdraw!
            balance -= amount;
        }
    }
    
    public double getBalance() {
        return balance;  // Read-only access
    }
}
```

**Encapsulation Gives You:**

- 🛡️ **Data Protection**: Bad data can't sneak in
- 🔄 **Flexibility**: Change internals without breaking clients
- ✅ **Validation**: Rules always enforced

### Pillar 2: Abstraction (The Remote Control)

**The Story**: You press "Channel Up" on your TV remote. You don't know (or care) how the signal is processed inside. That's abstraction!

```java
// The "remote control" - just the buttons you care about
public interface PaymentProcessor {
    boolean processPayment(double amount);
    void refund(String transactionId);
}

// The "inside of the TV" - complex, but hidden!
public class StripeProcessor implements PaymentProcessor {
    @Override
    public boolean processPayment(double amount) {
        // 50 lines of Stripe API code you don't care about
        connectToStripe();
        authenticateRequest();
        createPaymentIntent();
        confirmPayment();
        handleWebhooks();
        return true;
    }
}

// User code - simple!
PaymentProcessor payments = new StripeProcessor();
payments.processPayment(99.99);  // Just press the button!
```

**Abstraction vs Encapsulation:**

```
┌─────────────────────────────────────────────────────────────────────────┐
│  Abstraction                    │  Encapsulation                       │
├─────────────────────────────────────────────────────────────────────────┤
│  Hiding COMPLEXITY              │  Hiding DATA                         │
│  "What buttons does the         │  "What's inside the                  │
│   remote have?"                 │   remote?"                           │
│  Design level                   │  Implementation level                │
│  Interface/Abstract class       │  private fields + public methods     │
└─────────────────────────────────────────────────────────────────────────┘
```

### Pillar 3: Inheritance (The Family Tree)

**The Story**: A Dog IS-A Animal. It inherits all animal traits plus adds its own!

```java
// The ancestor
public class Animal {
    protected String name;
    
    public void eat() {
        System.out.println(name + " is eating");
    }
    
    public void sleep() {
        System.out.println(name + " is sleeping");
    }
}

// The descendant - inherits + extends
public class Dog extends Animal {
    
    public void bark() {
        System.out.println(name + " says WOOF!");  // Dog-specific
    }
    
    @Override
    public void eat() {
        System.out.println(name + " is eating dog food");  // Override!
    }
}

Dog buddy = new Dog();
buddy.name = "Buddy";
buddy.eat();    // "Buddy is eating dog food" (overridden)
buddy.sleep();  // "Buddy is sleeping" (inherited)
buddy.bark();   // "Buddy says WOOF!" (new)
```

**The Inheritance Types:**

```
Single:           Multilevel:        Hierarchical:
  Animal            Animal             Animal
     │                │               /     \
     │                ▼            Dog      Cat
     ▼              Mammal
   Dog                │
                      ▼
                    Dog
```

### Pillar 4: Polymorphism (The Shape-Shifter)

**The Story**: One interface, many forms. A remote's "Play" button works on TV, DVD, Streaming - different devices, same button!

```java
// Compile-time Polymorphism: OVERLOADING (same name, different params)
public class Calculator {
    public int add(int a, int b) { return a + b; }
    public double add(double a, double b) { return a + b; }
    public int add(int a, int b, int c) { return a + b + c; }
}

// Runtime Polymorphism: OVERRIDING (parent reference, child behavior)
Animal myPet = new Dog();  // Reference: Animal, Object: Dog
myPet.eat();  // Calls Dog's eat()! Decided at RUNTIME
```

**The Magic Moment:**

```java
public void makeSound(Animal a) {
    a.makeSound();  // Works for ANY animal!
}

makeSound(new Dog());   // "Woof!"
makeSound(new Cat());   // "Meow!"
makeSound(new Cow());   // "Moo!"

// ONE method handles infinite animal types! 🎉
```

---

## 📖 Chapter 2: Abstract Class vs Interface

### The Showdown

```
┌─────────────────────────────────────────────────────────────────────────┐
│               ABSTRACT CLASS                 INTERFACE                  │
├─────────────────────────────────────────────────────────────────────────┤
│  "I am a partial blueprint"      "I am a contract"                     │
│                                                                         │
│  Can have constructors ✅         No constructors ❌                    │
│  Can have state (fields) ✅       Only constants (static final) ❌      │
│  Single inheritance only ❌       Multiple inheritance ✅               │
│  Any access modifier ✅           Public only (methods) ❌              │
│  IS-A relationship                CAN-DO capability                     │
│                                                                         │
│  "What this thing IS"             "What this thing CAN DO"              │
└─────────────────────────────────────────────────────────────────────────┘
```

### When to Use What

```java
// Abstract Class: Related things sharing state & behavior
abstract class Vehicle {
    protected int speed;          // Shared state
    protected String brand;
    
    public void accelerate() {    // Shared behavior
        speed += 10;
    }
    
    abstract void honk();         // Must implement
}

class Car extends Vehicle {
    void honk() { System.out.println("Beep!"); }
}

class Truck extends Vehicle {
    void honk() { System.out.println("HONK HONK!"); }
}


// Interface: Unrelated things with common capability
interface Flyable {
    void fly();
}

class Bird implements Flyable {
    public void fly() { System.out.println("Flapping wings"); }
}

class Airplane implements Flyable {
    public void fly() { System.out.println("Engine thrust"); }
}

class Superman implements Flyable {
    public void fly() { System.out.println("Just flies somehow"); }
}

// Birds, Planes, Superman - totally unrelated, but all Flyable!
```

---

## 📖 Chapter 3: The SOLID Principles

**The Story**: Code rots over time. SOLID principles are the vitamins that keep it healthy!

### S - Single Responsibility Principle

> **"A class should have only ONE reason to change"**

```java
// ❌ BAD: This class has 3 jobs!
public class Employee {
    public void calculatePay() { }     // Accounting's concern
    public void saveToDatabase() { }    // DBA's concern
    public void generateReport() { }    // Reporting's concern
}

// If accounting rules change → modify Employee
// If database changes → modify Employee
// If report format changes → modify Employee
// 💥 Too many reasons to change!

// ✅ GOOD: One class, one job
public class Employee {
    private String name;
    private double salary;
    // Just employee DATA
}

public class PayrollCalculator {
    public double calculatePay(Employee e) { }  // Only pay logic
}

public class EmployeeRepository {
    public void save(Employee e) { }  // Only persistence
}

public class ReportGenerator {
    public void generate(Employee e) { }  // Only reporting
}
```

### O - Open/Closed Principle

> **"Open for extension, closed for modification"**

```java
// ❌ BAD: Must modify this class for every new shape
public class AreaCalculator {
    public double calculate(Object shape) {
        if (shape instanceof Circle c) {
            return Math.PI * c.radius * c.radius;
        } else if (shape instanceof Square s) {
            return s.side * s.side;
        }
        // Add Triangle? Modify this class!
        // Add Hexagon? Modify this class!
        return 0;
    }
}

// ✅ GOOD: Add new shapes without touching calculator
public interface Shape {
    double area();
}

public class Circle implements Shape {
    double radius;
    public double area() { return Math.PI * radius * radius; }
}

public class Square implements Shape {
    double side;
    public double area() { return side * side; }
}

// New shape? Just add a new class!
public class Triangle implements Shape {
    double base, height;
    public double area() { return 0.5 * base * height; }
}

public class AreaCalculator {
    public double calculate(Shape shape) {
        return shape.area();  // Works for ALL shapes, forever!
    }
}
```

### L - Liskov Substitution Principle

> **"Subtypes must be substitutable for their base types"**

```java
// ❌ BAD: Square violates Rectangle's contract
public class Rectangle {
    protected int width, height;
    
    public void setWidth(int w) { width = w; }
    public void setHeight(int h) { height = h; }
    public int area() { return width * height; }
}

public class Square extends Rectangle {
    @Override
    public void setWidth(int w) { width = height = w; }  // Breaks expectation!
    @Override
    public void setHeight(int h) { width = height = h; }
}

// Client code breaks!
Rectangle r = new Square();
r.setWidth(5);
r.setHeight(10);
System.out.println(r.area());  // Expected: 50, Got: 100! 💥

// ✅ GOOD: Separate hierarchies
public interface Shape {
    int area();
}

public class Rectangle implements Shape {
    private int width, height;
    public int area() { return width * height; }
}

public class Square implements Shape {
    private int side;
    public int area() { return side * side; }
}
```

### I - Interface Segregation Principle

> **"Clients shouldn't depend on interfaces they don't use"**

```java
// ❌ BAD: Fat interface forces useless implementations
public interface Worker {
    void work();
    void eat();
    void sleep();
}

public class Robot implements Worker {
    public void work() { /* OK */ }
    public void eat() { /* Robots don't eat! 🤖 */ }
    public void sleep() { /* Robots don't sleep! */ }
}

// ✅ GOOD: Segregated interfaces
public interface Workable { void work(); }
public interface Eatable { void eat(); }
public interface Sleepable { void sleep(); }

public class Human implements Workable, Eatable, Sleepable {
    // Humans do all three
}

public class Robot implements Workable {
    // Robots only work - clean interface!
}
```

### D - Dependency Inversion Principle

> **"Depend on abstractions, not concretions"**

```java
// ❌ BAD: High-level depends on low-level
public class OrderService {
    private MySQLDatabase db = new MySQLDatabase();  // Concrete!
    
    public void save(Order order) {
        db.insert(order);  // Married to MySQL forever
    }
}

// Want to use MongoDB? Rewrite the class!
// Want to test without database? Good luck!

// ✅ GOOD: Depend on abstraction
public interface Database {
    void insert(Object obj);
}

public class OrderService {
    private final Database db;  // Abstract!
    
    public OrderService(Database db) {  // Inject dependency
        this.db = db;
    }
    
    public void save(Order order) {
        db.insert(order);
    }
}

// Now works with anything!
new OrderService(new MySQLDatabase());
new OrderService(new MongoDatabase());
new OrderService(new MockDatabase());  // Easy testing! 🧪
```

---

## 📖 Chapter 4: Composition vs Inheritance

### The Famous Quote

> **"Favor composition over inheritance"** - Gang of Four

### Inheritance: IS-A (Tight Coupling)

```java
class Engine {
    void start() { System.out.println("Vroom!"); }
}

// Bad idea: Car IS-A Engine? 🤔
class Car extends Engine {
    // Hmm, now Car inherits ALL of Engine's internals...
}
```

### Composition: HAS-A (Loose Coupling)

```java
class Car {
    private Engine engine;  // Car HAS-A Engine
    
    public Car(Engine engine) {
        this.engine = engine;
    }
    
    void start() {
        engine.start();  // Delegate!
    }
}

// Benefits:
// ✅ Swap engines at runtime: new Car(new ElectricEngine())
// ✅ Test with mock: new Car(new MockEngine())
// ✅ Engine can be shared or replaced
```

### Decision Tree

```
┌─────────────────────────────────────────────────────────────────────────┐
│  Should I use inheritance?                                              │
├─────────────────────────────────────────────────────────────────────────┤
│                                                                         │
│  Is it a TRUE "IS-A" relationship?                                      │
│     └─ Dog IS-A Animal? ✅ Yes → Consider inheritance                   │
│     └─ Car IS-A Engine? ❌ No → Use composition                         │
│                                                                         │
│  Will the child use MOST of the parent's behavior?                      │
│     └─ Yes → Inheritance might work                                     │
│     └─ No, override everything → Composition!                           │
│                                                                         │
│  Might you need to change the relationship at runtime?                  │
│     └─ Yes → Composition (you can't change parent at runtime)           │
│                                                                         │
│  When in doubt → COMPOSITION                                            │
└─────────────────────────────────────────────────────────────────────────┘
```

---

## 📖 Chapter 5: Object Class Methods

Every class extends `Object`. Know these methods!

### The equals() and hashCode() Contract

```java
public class Person {
    private String name;
    private int age;
    
    @Override
    public boolean equals(Object o) {
        if (this == o) return true;  // Same reference?
        if (o == null || getClass() != o.getClass()) return false;
        Person person = (Person) o;
        return age == person.age && Objects.equals(name, person.name);
    }
    
    @Override
    public int hashCode() {
        return Objects.hash(name, age);  // Same fields as equals!
    }
}
```

**The Contract:**

```
IF a.equals(b) == true
THEN a.hashCode() == b.hashCode()  // MUST be true!

IF a.hashCode() != b.hashCode()
THEN a.equals(b) == false          // MUST be false!

IF a.hashCode() == b.hashCode()
THEN a.equals(b) == ???            // Could be either! (collision)
```

**Why it matters:**

```java
Set<Person> people = new HashSet<>();
Person p1 = new Person("Alice", 30);
Person p2 = new Person("Alice", 30);

// Without proper equals/hashCode:
people.add(p1);
people.add(p2);
people.size();  // 2 💥 (duplicates!)

// With proper equals/hashCode:
people.add(p1);
people.add(p2);
people.size();  // 1 ✅ (recognized as same)
```

---

## 🎯 Quick Reference: SOLID

```
┌───────────────────────────────────────────────────────────────────────┐
│ S │ Single Responsibility  │ One class = One job                     │
│ O │ Open/Closed            │ Extend without modifying                │
│ L │ Liskov Substitution    │ Subtypes work like parent              │
│ I │ Interface Segregation  │ Many specific interfaces > one fat one │
│ D │ Dependency Inversion   │ Depend on abstractions                 │
└───────────────────────────────────────────────────────────────────────┘
```
