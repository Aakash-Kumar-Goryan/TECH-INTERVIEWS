# Design Patterns in Java

> *Design patterns are battle-tested solutions to common problems. Let's explore them through real scenarios where you'd actually use them...*

---

## 🎬 The Pattern Story

Imagine you're building a complex system. You keep hitting the same types of problems:

- "I need exactly ONE database connection" → **Singleton**
- "I don't want clients knowing which class they're creating" → **Factory**
- "This object has too many constructor parameters" → **Builder**
- "I want to add features without changing the class" → **Decorator**

Patterns are the wisdom of developers who solved these before!

---

## 📖 Chapter 1: Creational Patterns (Making Objects)

### 🏛️ Singleton: "There Can Be Only One"

**The Story**: Your app needs ONE database connection pool, ONE logger, ONE configuration. Not two. Not zero. ONE.

```java
// 🌟 BEST: Enum Singleton
public enum DatabasePool {
    INSTANCE;
    
    private final List<Connection> connections = new ArrayList<>();
    
    public Connection getConnection() {
        // Return connection from pool
    }
}

// Usage
DatabasePool.INSTANCE.getConnection();

// Why Enum is best:
// ✅ Thread-safe (JVM guarantees)
// ✅ Serialization-safe (can't create duplicates via deserialization)
// ✅ Reflection-safe (can't create via reflection)
// ✅ Simple!
```

**Other Options (Know for Interviews):**

```java
// Double-Checked Locking (Classic Interview Question)
public class Singleton {
    private static volatile Singleton instance;  // volatile is REQUIRED!
    
    private Singleton() { }
    
    public static Singleton getInstance() {
        if (instance == null) {                    // First check (no lock)
            synchronized (Singleton.class) {
                if (instance == null) {            // Second check (with lock)
                    instance = new Singleton();
                }
            }
        }
        return instance;
    }
}

// Bill Pugh Holder (Lazy + Thread-safe)
public class Singleton {
    private Singleton() { }
    
    private static class Holder {
        static final Singleton INSTANCE = new Singleton();
    }
    
    public static Singleton getInstance() {
        return Holder.INSTANCE;  // Class loaded only when called!
    }
}
```

### 🏭 Factory: "Don't Ask What, Just Ask For"

**The Story**: Client should say "give me a notification" not "new SMSNotification(apiKey, sender, ...)".

```java
// Product interface
public interface Notification {
    void send(String message);
}

// Concrete products
public class EmailNotification implements Notification {
    public void send(String message) {
        System.out.println("📧 Email: " + message);
    }
}

public class SMSNotification implements Notification {
    public void send(String message) {
        System.out.println("📱 SMS: " + message);
    }
}

public class PushNotification implements Notification {
    public void send(String message) {
        System.out.println("🔔 Push: " + message);
    }
}

// The Factory
public class NotificationFactory {
    public static Notification create(String type) {
        return switch (type.toUpperCase()) {
            case "EMAIL" -> new EmailNotification();
            case "SMS" -> new SMSNotification();
            case "PUSH" -> new PushNotification();
            default -> throw new IllegalArgumentException("Unknown: " + type);
        };
    }
}

// Client code - clean and oblivious!
Notification notif = NotificationFactory.create("EMAIL");
notif.send("Hello!");
```

### 🏗️ Abstract Factory: "Family of Products"

**The Story**: You need Windows buttons AND Windows checkboxes that look consistent together. Or Mac versions. But never mix!

```java
// Abstract products
interface Button { void render(); }
interface Checkbox { void render(); }

// Windows family
class WindowsButton implements Button {
    public void render() { System.out.println("[Windows Button]"); }
}
class WindowsCheckbox implements Checkbox {
    public void render() { System.out.println("☐ Windows Checkbox"); }
}

// Mac family  
class MacButton implements Button {
    public void render() { System.out.println("( Mac Button )"); }
}
class MacCheckbox implements Checkbox {
    public void render() { System.out.println("◯ Mac Checkbox"); }
}

// Abstract Factory
interface GUIFactory {
    Button createButton();
    Checkbox createCheckbox();
}

class WindowsFactory implements GUIFactory {
    public Button createButton() { return new WindowsButton(); }
    public Checkbox createCheckbox() { return new WindowsCheckbox(); }
}

class MacFactory implements GUIFactory {
    public Button createButton() { return new MacButton(); }
    public Checkbox createCheckbox() { return new MacCheckbox(); }
}

// Client picks a factory, gets consistent family
GUIFactory factory = isWindows ? new WindowsFactory() : new MacFactory();
Button btn = factory.createButton();       // Matching family
Checkbox chk = factory.createCheckbox();   // Matching family
```

### 🧱 Builder: "Complex Objects, Step by Step"

**The Story**: User has name (required), email (required), age (optional), phone (optional), address (optional)... Do you really want a constructor with 10 parameters?

```java
public class User {
    // Required
    private final String name;
    private final String email;
    
    // Optional
    private final int age;
    private final String phone;
    private final String address;
    
    private User(Builder builder) {
        this.name = builder.name;
        this.email = builder.email;
        this.age = builder.age;
        this.phone = builder.phone;
        this.address = builder.address;
    }
    
    public static class Builder {
        // Required
        private final String name;
        private final String email;
        
        // Optional with defaults
        private int age = 0;
        private String phone = "";
        private String address = "";
        
        public Builder(String name, String email) {
            this.name = name;
            this.email = email;
        }
        
        public Builder age(int age) {
            this.age = age;
            return this;  // Return this for chaining!
        }
        
        public Builder phone(String phone) {
            this.phone = phone;
            return this;
        }
        
        public Builder address(String address) {
            this.address = address;
            return this;
        }
        
        public User build() {
            return new User(this);
        }
    }
}

// Beautiful fluent API!
User user = new User.Builder("John", "john@email.com")
    .age(30)
    .phone("123-456-7890")
    .build();
```

---

## 📖 Chapter 2: Structural Patterns (Organizing Classes)

### 🔌 Adapter: "Square Peg, Round Hole"

**The Story**: You have a cool new media player, but your app expects the old interface. Make them work together!

```java
// What your app expects
interface MediaPlayer {
    void play(String filename);
}

// The new fancy player (different interface!)
class AdvancedPlayer {
    void playMp4(String file) { System.out.println("Playing MP4: " + file); }
    void playVlc(String file) { System.out.println("Playing VLC: " + file); }
}

// The Adapter - makes new work with old!
class MediaAdapter implements MediaPlayer {
    private AdvancedPlayer advancedPlayer = new AdvancedPlayer();
    
    @Override
    public void play(String filename) {
        if (filename.endsWith(".mp4")) {
            advancedPlayer.playMp4(filename);
        } else if (filename.endsWith(".vlc")) {
            advancedPlayer.playVlc(filename);
        }
    }
}

// Now your app works with both!
MediaPlayer player = new MediaAdapter();
player.play("movie.mp4");  // Works!
```

### 🎨 Decorator: "Adding Toppings"

**The Story**: You want to add features to objects without modifying their class. Like adding milk to coffee, then sugar, then whipped cream...

```java
// The base
interface Coffee {
    double getCost();
    String getDescription();
}

class SimpleCoffee implements Coffee {
    public double getCost() { return 2.0; }
    public String getDescription() { return "Coffee"; }
}

// The decorator base
abstract class CoffeeDecorator implements Coffee {
    protected Coffee wrapped;
    
    CoffeeDecorator(Coffee coffee) { this.wrapped = coffee; }
    
    public double getCost() { return wrapped.getCost(); }
    public String getDescription() { return wrapped.getDescription(); }
}

// Concrete decorators
class Milk extends CoffeeDecorator {
    Milk(Coffee coffee) { super(coffee); }
    
    public double getCost() { return super.getCost() + 0.5; }
    public String getDescription() { return super.getDescription() + " + Milk"; }
}

class Sugar extends CoffeeDecorator {
    Sugar(Coffee coffee) { super(coffee); }
    
    public double getCost() { return super.getCost() + 0.25; }
    public String getDescription() { return super.getDescription() + " + Sugar"; }
}

// Build your coffee!
Coffee order = new Sugar(new Milk(new SimpleCoffee()));
System.out.println(order.getDescription());  // "Coffee + Milk + Sugar"
System.out.println(order.getCost());          // 2.75

// Real Java examples:
// BufferedInputStream wraps FileInputStream
// Collections.synchronizedList() wraps ArrayList
```

### 🛡️ Proxy: "The Bouncer"

**The Story**: Control access to an expensive object. Load it only when really needed!

```java
interface Image {
    void display();
}

// Expensive real object
class RealImage implements Image {
    private String filename;
    
    RealImage(String filename) {
        this.filename = filename;
        loadFromDisk();  // Expensive! 💰
    }
    
    private void loadFromDisk() {
        System.out.println("Loading " + filename + "...");
    }
    
    public void display() {
        System.out.println("Displaying " + filename);
    }
}

// Proxy - lazy loading
class ImageProxy implements Image {
    private RealImage realImage;  // Not loaded yet!
    private String filename;
    
    ImageProxy(String filename) {
        this.filename = filename;  // Just save the name
    }
    
    public void display() {
        if (realImage == null) {
            realImage = new RealImage(filename);  // Load only NOW
        }
        realImage.display();
    }
}

// Usage
Image img = new ImageProxy("photo.jpg");  // No loading yet!
// ... later ...
img.display();  // NOW it loads
```

### 🏠 Facade: "One Button to Rule Them All"

**The Story**: Starting a computer involves CPU, memory, hard drive... but users just want `computer.start()`.

```java
// Complex subsystems
class CPU { void freeze() {} void execute() {} }
class Memory { void load() {} }
class HardDrive { byte[] read() { return new byte[0]; } }

// Facade - simple interface
class Computer {
    private CPU cpu = new CPU();
    private Memory memory = new Memory();
    private HardDrive hd = new HardDrive();
    
    public void start() {
        cpu.freeze();
        memory.load();
        hd.read();
        cpu.execute();
        System.out.println("Computer started! 🖥️");
    }
}

// User just does:
new Computer().start();  // All complexity hidden!
```

---

## 📖 Chapter 3: Behavioral Patterns (Object Communication)

### 🎯 Strategy: "Pick Your Algorithm"

**The Story**: Payment can be via credit card, PayPal, or crypto. Same checkout, different strategies!

```java
// Strategy interface
interface PaymentStrategy {
    void pay(int amount);
}

// Concrete strategies
class CreditCard implements PaymentStrategy {
    public void pay(int amount) {
        System.out.println("💳 Paid $" + amount + " via Credit Card");
    }
}

class PayPal implements PaymentStrategy {
    public void pay(int amount) {
        System.out.println("🅿️ Paid $" + amount + " via PayPal");
    }
}

class Crypto implements PaymentStrategy {
    public void pay(int amount) {
        System.out.println("₿ Paid $" + amount + " via Crypto");
    }
}

// Context
class ShoppingCart {
    private PaymentStrategy payment;
    
    void setPaymentMethod(PaymentStrategy strategy) {
        this.payment = strategy;
    }
    
    void checkout(int amount) {
        payment.pay(amount);
    }
}

// Usage - swap algorithms at runtime!
ShoppingCart cart = new ShoppingCart();
cart.setPaymentMethod(new CreditCard());
cart.checkout(100);  // 💳 Paid $100 via Credit Card

cart.setPaymentMethod(new Crypto());
cart.checkout(200);  // ₿ Paid $200 via Crypto
```

### 👁️ Observer: "News Flash!"

**The Story**: When news breaks, all subscribers get notified instantly!

```java
// Observer
interface Subscriber {
    void update(String news);
}

// Subject
class NewsAgency {
    private List<Subscriber> subscribers = new ArrayList<>();
    
    void subscribe(Subscriber s) { subscribers.add(s); }
    void unsubscribe(Subscriber s) { subscribers.remove(s); }
    
    void publishNews(String news) {
        System.out.println("📢 Breaking: " + news);
        for (Subscriber s : subscribers) {
            s.update(news);
        }
    }
}

// Concrete observers
class PhoneApp implements Subscriber {
    public void update(String news) {
        System.out.println("📱 Phone notification: " + news);
    }
}

class EmailClient implements Subscriber {
    public void update(String news) {
        System.out.println("📧 Email received: " + news);
    }
}

// Usage
NewsAgency agency = new NewsAgency();
agency.subscribe(new PhoneApp());
agency.subscribe(new EmailClient());

agency.publishNews("Java 22 Released!");
// 📢 Breaking: Java 22 Released!
// 📱 Phone notification: Java 22 Released!
// 📧 Email received: Java 22 Released!
```

### 📝 Template Method: "The Recipe"

**The Story**: Every data processor reads, processes, writes. But HOW they do each step varies.

```java
abstract class DataProcessor {
    // The template method - DO NOT OVERRIDE!
    public final void process() {
        readData();
        processData();
        writeData();
    }
    
    abstract void readData();
    abstract void processData();
    
    // Hook - optional override
    void writeData() {
        System.out.println("Default write");
    }
}

class CSVProcessor extends DataProcessor {
    void readData() { System.out.println("Reading CSV..."); }
    void processData() { System.out.println("Parsing CSV rows..."); }
}

class JSONProcessor extends DataProcessor {
    void readData() { System.out.println("Reading JSON..."); }
    void processData() { System.out.println("Parsing JSON objects..."); }
    
    @Override
    void writeData() {
        System.out.println("Writing formatted JSON");
    }
}

// Same flow, different implementations!
new CSVProcessor().process();
new JSONProcessor().process();
```

### ⌨️ Command: "Undo This!"

**The Story**: Encapsulate commands as objects. Queue them, undo them, replay them!

```java
interface Command {
    void execute();
    void undo();
}

// Receiver
class TextEditor {
    StringBuilder text = new StringBuilder();
    
    void insert(String s) { text.append(s); }
    void delete(int n) { text.delete(text.length() - n, text.length()); }
    String getText() { return text.toString(); }
}

// Concrete command
class InsertCommand implements Command {
    private TextEditor editor;
    private String textToInsert;
    
    InsertCommand(TextEditor editor, String text) {
        this.editor = editor;
        this.textToInsert = text;
    }
    
    public void execute() { editor.insert(textToInsert); }
    public void undo() { editor.delete(textToInsert.length()); }
}

// Invoker with history
class CommandHistory {
    private Stack<Command> history = new Stack<>();
    
    void execute(Command cmd) {
        cmd.execute();
        history.push(cmd);
    }
    
    void undo() {
        if (!history.isEmpty()) {
            history.pop().undo();
        }
    }
}

// Usage
TextEditor editor = new TextEditor();
CommandHistory history = new CommandHistory();

history.execute(new InsertCommand(editor, "Hello "));
history.execute(new InsertCommand(editor, "World!"));
System.out.println(editor.getText());  // "Hello World!"

history.undo();
System.out.println(editor.getText());  // "Hello "
```

---

## 🎯 Pattern Selection Guide

```
┌─────────────────────────────────────────────────────────────────────────┐
│  Problem                              │  Pattern                        │
├───────────────────────────────────────────────────────────────────────── │
│  Need exactly one instance            │  Singleton (use Enum!)          │
│  Don't want to expose which class     │  Factory Method                 │
│  Need families of related objects     │  Abstract Factory               │
│  Constructor has too many params      │  Builder                        │
│  Need to copy objects                 │  Prototype                      │
├─────────────────────────────────────────────────────────────────────────┤
│  Incompatible interfaces              │  Adapter                        │
│  Add features without modifying       │  Decorator                      │
│  Control access / lazy loading        │  Proxy                          │
│  Simplify complex system              │  Facade                         │
├─────────────────────────────────────────────────────────────────────────┤
│  Swap algorithms at runtime           │  Strategy                       │
│  Notify multiple objects of changes   │  Observer                       │
│  Define skeleton with variable steps  │  Template Method                │
│  Encapsulate operations / undo        │  Command                        │
└─────────────────────────────────────────────────────────────────────────┘
```
