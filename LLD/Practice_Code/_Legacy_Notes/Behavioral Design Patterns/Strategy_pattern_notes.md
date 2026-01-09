# Strategy Design Pattern

## Definition

The **Strategy Pattern** defines a family of algorithms, encapsulates each one, and makes them interchangeable. It lets the algorithm vary independently from clients that use it.

---

## Use Cases

Use the Strategy Pattern when:

- You have multiple algorithms for a task, and you want to choose one at runtime.
- You want to avoid using conditional statements (if/else or switch) for selecting behavior.
- You want to follow the Open/Closed Principle — extend behaviors without modifying existing code.

---

## Real-World Analogies

- **Navigation App**: You can choose between driving, walking, or cycling routes.
- **Payment System**: Use CreditCard, UPI, PayPal, etc., based on user selection.
- **Compression Tool**: Choose between Zip, RAR, GZip, etc.

---

## Why Use Strategy Pattern?

### ✅ Avoids Conditional Logic

- Eliminates the need for `if/else` or `switch` for behavior selection.

### ✅ Promotes Code Reusability & Flexibility

- New strategies can be added without modifying existing code.

### ✅ Allows Runtime Behavior Changes

- Strategy can be swapped dynamically at runtime.

---

## Frequently Asked in Interviews

> Often used to evaluate your understanding of behavior encapsulation and separation of concerns.

---

## Class Structure

### 1. Strategy Interface

```java
interface PaymentStrategy {
    void pay(int amount);
}
```

### 2. Concrete Strategies

```java
class CreditCardPayment implements PaymentStrategy {
    public void pay(int amount) {
        System.out.println("Paid ₹" + amount + " using Credit Card.");
    }
}

class PayPalPayment implements PaymentStrategy {
    public void pay(int amount) {
        System.out.println("Paid ₹" + amount + " using PayPal.");
    }
}

class UpiPayment implements PaymentStrategy {
    public void pay(int amount) {
        System.out.println("Paid ₹" + amount + " using UPI.");
    }
}
```

### 3. Context Class

```java
class ShoppingCart {
    private PaymentStrategy paymentStrategy;

    public void setPaymentStrategy(PaymentStrategy strategy) {
        this.paymentStrategy = strategy;
    }

    public void checkout(int amount) {
        if (paymentStrategy == null) {
            throw new IllegalStateException("Payment strategy not set.");
        }
        paymentStrategy.pay(amount);
    }
}
```

### 4. Usage Example

```java
public class Main {
    public static void main(String[] args) {
        ShoppingCart cart = new ShoppingCart();

        cart.setPaymentStrategy(new CreditCardPayment());
        cart.checkout(1000); // Paid ₹1000 using Credit Card.

        cart.setPaymentStrategy(new UpiPayment());
        cart.checkout(500);  // Paid ₹500 using UPI.
    }
}
```

### Output

```java
Paid ₹1000 using Credit Card.
Paid ₹500 using UPI.
```

## OOP Concepts Used

| Concept         | Description                                                                 |
|----------------|-----------------------------------------------------------------------------|
| **Encapsulation** | Each strategy encapsulates an algorithm behind a common interface.         |
| **Abstraction**   | The `PaymentStrategy` interface abstracts away the payment behavior.       |
| **Polymorphism**  | The same interface (`pay`) is implemented differently by various strategies. |
| **Composition**   | The `ShoppingCart` is composed with a `PaymentStrategy` to delegate behavior. |

## SOLID Principles Demonstrated

| Principle       | How It Applies                                                                 |
|-----------------|----------------------------------------------------------------------------------|
| **S - SRP**      | Each class has a single responsibility (either payment logic or cart logic).   |
| **O - OCP**      | New strategies can be added without modifying existing code.                   |
| **L - LSP**      | Any `PaymentStrategy` subclass can be used without breaking the system.        |
| **I - ISP**      | `PaymentStrategy` interface is minimal and specific (only `pay()` method).     |
| **D - DIP**      | `ShoppingCart` depends on the abstraction `PaymentStrategy`, not concrete classes. |

## 🚀 Modern Java Implementation (Lambda Support)

Since the `Strategy` interface usually has a single abstract method (SAM), it qualifies as a **Functional Interface**. You can use Lambdas to avoid creating extra classes!

```java
public class Main {
    public static void main(String[] args) {
        ShoppingCart cart = new ShoppingCart();

        // Using Lambda for on-the-fly strategy
        cart.setPaymentStrategy(amount -> System.out.println("Paid ₹" + amount + " via Bitcoin"));
        
        cart.checkout(999);
    }
}
```

---

## 🆚 Strategy vs State Pattern (Interview Common Q)

They look identical (Class Diagrams are same!), but their **intent** differs:
- **Strategy**: "I want to perform ONE task, and I can choose HOW (algorithm) to do it." (Driven by Client).
- **State**: "My specific behavior changes completely based on what state I am in." (Driven by Internal State).

---

- **Strategy Interface**: Defines the common algorithm contract.
- **Concrete Strategies**: Implement the algorithm in different ways.
- **Context**: Uses a strategy to perform a specific task.
- **Behavioral Pattern**: Focuses on how an object performs a task.

🧠 Think of Strategy Pattern as a way to swap behaviors dynamically — just like changing gears in a car without replacing the car itself.
