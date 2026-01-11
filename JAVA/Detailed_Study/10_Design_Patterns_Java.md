# Design Patterns in Java

Patterns are frequently asked, especially in LLD rounds. Know the classic implementations.

## 1. Creational Patterns

### Singleton

Ensure only one instance exists.

```java
// Eager Initialization
public class Singleton {
    private static final Singleton INSTANCE = new Singleton();
    
    private Singleton() { }
    
    public static Singleton getInstance() {
        return INSTANCE;
    }
}

// Lazy Initialization (Thread-Safe - Double-Checked Locking)
public class Singleton {
    private static volatile Singleton instance;
    
    private Singleton() { }
    
    public static Singleton getInstance() {
        if (instance == null) {
            synchronized (Singleton.class) {
                if (instance == null) {
                    instance = new Singleton();
                }
            }
        }
        return instance;
    }
}

// Bill Pugh (Initialize-On-Demand Holder)
public class Singleton {
    private Singleton() { }
    
    private static class Holder {
        private static final Singleton INSTANCE = new Singleton();
    }
    
    public static Singleton getInstance() {
        return Holder.INSTANCE;
    }
}

// Enum Singleton (Best Practice)
public enum Singleton {
    INSTANCE;
    
    public void doSomething() { }
}
```

**Why Enum is best**: Serialization-safe, reflection-safe, thread-safe.

---

### Factory Method

Create objects without specifying exact class.

```java
// Product interface
public interface Notification {
    void send(String message);
}

// Concrete products
public class EmailNotification implements Notification {
    public void send(String message) {
        System.out.println("Email: " + message);
    }
}

public class SMSNotification implements Notification {
    public void send(String message) {
        System.out.println("SMS: " + message);
    }
}

// Factory
public class NotificationFactory {
    public static Notification create(String type) {
        return switch (type) {
            case "EMAIL" -> new EmailNotification();
            case "SMS" -> new SMSNotification();
            default -> throw new IllegalArgumentException("Unknown type");
        };
    }
}

// Usage
Notification notification = NotificationFactory.create("EMAIL");
notification.send("Hello");
```

---

### Abstract Factory

Create families of related objects.

```java
// Abstract products
public interface Button { void render(); }
public interface Checkbox { void render(); }

// Concrete products for Windows
public class WindowsButton implements Button {
    public void render() { System.out.println("Windows Button"); }
}
public class WindowsCheckbox implements Checkbox {
    public void render() { System.out.println("Windows Checkbox"); }
}

// Concrete products for Mac
public class MacButton implements Button {
    public void render() { System.out.println("Mac Button"); }
}
public class MacCheckbox implements Checkbox {
    public void render() { System.out.println("Mac Checkbox"); }
}

// Abstract Factory
public interface GUIFactory {
    Button createButton();
    Checkbox createCheckbox();
}

// Concrete Factories
public class WindowsFactory implements GUIFactory {
    public Button createButton() { return new WindowsButton(); }
    public Checkbox createCheckbox() { return new WindowsCheckbox(); }
}

public class MacFactory implements GUIFactory {
    public Button createButton() { return new MacButton(); }
    public Checkbox createCheckbox() { return new MacCheckbox(); }
}
```

---

### Builder

Construct complex objects step by step.

```java
public class User {
    private final String name;      // Required
    private final String email;     // Required
    private final int age;          // Optional
    private final String phone;     // Optional
    
    private User(Builder builder) {
        this.name = builder.name;
        this.email = builder.email;
        this.age = builder.age;
        this.phone = builder.phone;
    }
    
    public static class Builder {
        private final String name;
        private final String email;
        private int age = 0;
        private String phone = "";
        
        public Builder(String name, String email) {
            this.name = name;
            this.email = email;
        }
        
        public Builder age(int age) {
            this.age = age;
            return this;
        }
        
        public Builder phone(String phone) {
            this.phone = phone;
            return this;
        }
        
        public User build() {
            return new User(this);
        }
    }
}

// Usage
User user = new User.Builder("John", "john@example.com")
    .age(30)
    .phone("123-456-7890")
    .build();
```

---

### Prototype

Clone existing objects.

```java
public abstract class Shape implements Cloneable {
    protected String color;
    
    public abstract Shape clone();
}

public class Circle extends Shape {
    private int radius;
    
    public Circle(Circle source) {
        this.color = source.color;
        this.radius = source.radius;
    }
    
    @Override
    public Circle clone() {
        return new Circle(this);
    }
}
```

---

## 2. Structural Patterns

### Adapter

Convert interface to work with incompatible classes.

```java
// Existing interface
public interface MediaPlayer {
    void play(String filename);
}

// Incompatible interface
public interface AdvancedMediaPlayer {
    void playVlc(String filename);
    void playMp4(String filename);
}

public class VlcPlayer implements AdvancedMediaPlayer {
    public void playVlc(String filename) {
        System.out.println("Playing VLC: " + filename);
    }
    public void playMp4(String filename) { }  // Not used
}

// Adapter
public class MediaAdapter implements MediaPlayer {
    private AdvancedMediaPlayer player;
    
    public MediaAdapter(String type) {
        if (type.equals("vlc")) {
            player = new VlcPlayer();
        }
    }
    
    @Override
    public void play(String filename) {
        player.playVlc(filename);
    }
}
```

---

### Decorator

Add behavior dynamically without modifying class.

```java
// Component
public interface Coffee {
    double getCost();
    String getDescription();
}

// Concrete component
public class SimpleCoffee implements Coffee {
    public double getCost() { return 1.0; }
    public String getDescription() { return "Simple coffee"; }
}

// Decorator base
public abstract class CoffeeDecorator implements Coffee {
    protected Coffee wrappedCoffee;
    
    public CoffeeDecorator(Coffee coffee) {
        this.wrappedCoffee = coffee;
    }
}

// Concrete decorators
public class MilkDecorator extends CoffeeDecorator {
    public MilkDecorator(Coffee coffee) {
        super(coffee);
    }
    
    public double getCost() { return wrappedCoffee.getCost() + 0.5; }
    public String getDescription() { return wrappedCoffee.getDescription() + ", milk"; }
}

public class SugarDecorator extends CoffeeDecorator {
    public SugarDecorator(Coffee coffee) {
        super(coffee);
    }
    
    public double getCost() { return wrappedCoffee.getCost() + 0.2; }
    public String getDescription() { return wrappedCoffee.getDescription() + ", sugar"; }
}

// Usage
Coffee coffee = new MilkDecorator(new SugarDecorator(new SimpleCoffee()));
System.out.println(coffee.getDescription());  // Simple coffee, sugar, milk
System.out.println(coffee.getCost());          // 1.7
```

**Java examples**: `BufferedInputStream`, `Collections.synchronizedList()`

---

### Proxy

Control access to another object.

```java
public interface Image {
    void display();
}

// Real object (expensive to create)
public class RealImage implements Image {
    private String filename;
    
    public RealImage(String filename) {
        this.filename = filename;
        loadFromDisk();  // Expensive
    }
    
    private void loadFromDisk() {
        System.out.println("Loading " + filename);
    }
    
    public void display() {
        System.out.println("Displaying " + filename);
    }
}

// Proxy (lazy loading)
public class ImageProxy implements Image {
    private RealImage realImage;
    private String filename;
    
    public ImageProxy(String filename) {
        this.filename = filename;
    }
    
    public void display() {
        if (realImage == null) {
            realImage = new RealImage(filename);  // Load only when needed
        }
        realImage.display();
    }
}
```

**Types**: Virtual (lazy), Remote (RMI), Protection (access control).

---

### Facade

Simplified interface to complex subsystem.

```java
// Complex subsystem
class CPU { void freeze() { } void execute() { } }
class Memory { void load(long position, byte[] data) { } }
class HardDrive { byte[] read(long lba, int size) { return new byte[0]; } }

// Facade
public class Computer {
    private CPU cpu = new CPU();
    private Memory memory = new Memory();
    private HardDrive hd = new HardDrive();
    
    public void start() {
        cpu.freeze();
        memory.load(0, hd.read(0, 512));
        cpu.execute();
    }
}

// Usage
Computer computer = new Computer();
computer.start();  // Simple interface
```

---

## 3. Behavioral Patterns

### Strategy

Define a family of algorithms, make them interchangeable.

```java
// Strategy interface
public interface PaymentStrategy {
    void pay(int amount);
}

// Concrete strategies
public class CreditCardPayment implements PaymentStrategy {
    private String cardNumber;
    
    public CreditCardPayment(String cardNumber) {
        this.cardNumber = cardNumber;
    }
    
    public void pay(int amount) {
        System.out.println("Paid " + amount + " via Credit Card");
    }
}

public class PayPalPayment implements PaymentStrategy {
    private String email;
    
    public PayPalPayment(String email) {
        this.email = email;
    }
    
    public void pay(int amount) {
        System.out.println("Paid " + amount + " via PayPal");
    }
}

// Context
public class ShoppingCart {
    private PaymentStrategy paymentStrategy;
    
    public void setPaymentStrategy(PaymentStrategy strategy) {
        this.paymentStrategy = strategy;
    }
    
    public void checkout(int amount) {
        paymentStrategy.pay(amount);
    }
}

// Usage
ShoppingCart cart = new ShoppingCart();
cart.setPaymentStrategy(new CreditCardPayment("1234-5678"));
cart.checkout(100);

cart.setPaymentStrategy(new PayPalPayment("user@example.com"));
cart.checkout(200);
```

**Java examples**: `Comparator`, sorting strategies.

---

### Observer

One-to-many dependency; notify observers of state changes.

```java
// Observer interface
public interface Observer {
    void update(String message);
}

// Subject
public class NewsAgency {
    private List<Observer> observers = new ArrayList<>();
    private String news;
    
    public void addObserver(Observer o) {
        observers.add(o);
    }
    
    public void removeObserver(Observer o) {
        observers.remove(o);
    }
    
    public void setNews(String news) {
        this.news = news;
        notifyObservers();
    }
    
    private void notifyObservers() {
        for (Observer o : observers) {
            o.update(news);
        }
    }
}

// Concrete observer
public class NewsChannel implements Observer {
    private String name;
    
    public NewsChannel(String name) {
        this.name = name;
    }
    
    public void update(String message) {
        System.out.println(name + " received: " + message);
    }
}

// Usage
NewsAgency agency = new NewsAgency();
agency.addObserver(new NewsChannel("CNN"));
agency.addObserver(new NewsChannel("BBC"));
agency.setNews("Breaking news!");
```

**Java examples**: `PropertyChangeListener`, event listeners.

---

### Template Method

Define algorithm skeleton, let subclasses override steps.

```java
public abstract class DataProcessor {
    
    // Template method
    public final void process() {
        readData();
        processData();
        writeData();
    }
    
    protected abstract void readData();
    protected abstract void processData();
    
    // Hook method (optional override)
    protected void writeData() {
        System.out.println("Default write implementation");
    }
}

public class CSVProcessor extends DataProcessor {
    protected void readData() {
        System.out.println("Reading CSV file");
    }
    
    protected void processData() {
        System.out.println("Processing CSV data");
    }
}

public class XMLProcessor extends DataProcessor {
    protected void readData() {
        System.out.println("Reading XML file");
    }
    
    protected void processData() {
        System.out.println("Processing XML data");
    }
    
    @Override
    protected void writeData() {
        System.out.println("Writing XML output");
    }
}
```

---

### Command

Encapsulate request as an object.

```java
// Command interface
public interface Command {
    void execute();
    void undo();
}

// Receiver
public class Light {
    public void on() { System.out.println("Light on"); }
    public void off() { System.out.println("Light off"); }
}

// Concrete commands
public class LightOnCommand implements Command {
    private Light light;
    
    public LightOnCommand(Light light) {
        this.light = light;
    }
    
    public void execute() { light.on(); }
    public void undo() { light.off(); }
}

// Invoker
public class RemoteControl {
    private Command command;
    
    public void setCommand(Command command) {
        this.command = command;
    }
    
    public void pressButton() {
        command.execute();
    }
    
    public void pressUndo() {
        command.undo();
    }
}
```

**Use cases**: Undo/redo, queuing operations, macros.

---

## 4. Pattern Selection Guide

| Scenario | Pattern |
|----------|---------|
| Single instance | Singleton |
| Object creation variation | Factory |
| Complex object with many params | Builder |
| Add behavior at runtime | Decorator |
| Simplify complex system | Facade |
| Interchangeable algorithms | Strategy |
| Notify multiple objects of changes | Observer |
| Encapsulate operations | Command |
| Control object access | Proxy |
