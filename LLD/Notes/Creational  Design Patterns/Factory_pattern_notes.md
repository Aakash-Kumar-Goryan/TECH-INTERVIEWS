# Factory Design Pattern

## Definition

The **Factory Pattern** defines an interface for creating an object but lets subclasses alter the type of objects that will be created. It encapsulates object creation logic in one place.

---

## Use Cases

- When you don’t want the client to instantiate objects directly.
- When you want to delegate the responsibility of object instantiation to a separate class.
- When you have multiple implementations of an interface or superclass.

---

## Real-World Analogies

- **Vehicle Factory** → Depending on user input, it returns a `Car`, `Bike`, or `Truck`.
- **Document Converter** → Returns the appropriate parser for PDF, Word, or Excel.

---

## Why Use Factory Pattern?

### ✅ Centralized Object Creation

- Keeps instantiation logic in one place.

### ✅ Decouples Clients from Concrete Implementations

- Client code only knows the interface, not the specific class being used.

### ✅ Enables Runtime Decision Making

- Objects can be instantiated based on input/configuration.

---

## Frequently Asked in Interviews

> One of the most commonly asked patterns to test encapsulation, extensibility, and object-oriented design.

---

## Class Structure

### 1. Product Interface

```java
interface Shape {
    void draw();
}
```

### 2. Concrete Implementations

```java
class Circle implements Shape {
    public void draw() {
        System.out.println("Drawing a Circle");
    }
}

class Rectangle implements Shape {
    public void draw() {
        System.out.println("Drawing a Rectangle");
    }
}
```

### 3. Factory Class

```java
class ShapeFactory {
    public Shape getShape(String shapeType) {
        if (shapeType.equalsIgnoreCase("circle")) {
            return new Circle();
        } else if (shapeType.equalsIgnoreCase("rectangle")) {
            return new Rectangle();
        }
        return null;
    }
}
```

### 4. Usage Example

```java
public class Main {
    public static void main(String[] args) {
        ShapeFactory factory = new ShapeFactory();

        Shape shape1 = factory.getShape("circle");
        shape1.draw(); // Drawing a Circle

        Shape shape2 = factory.getShape("rectangle");
        shape2.draw(); // Drawing a Rectangle
    }
}
```
### OOP Concepts Used

| Concept         | Description                                                                 |
|----------------|-----------------------------------------------------------------------------|
| **Encapsulation** | Object creation logic is hidden from the client.         |
| **Abstraction**   | Factory method provides a level of abstraction over the concrete classes.    |
| **Polymorphism**  | Returned objects share a common interface and are used polymorphically. |

### SOLID Principles Demonstrated

| Principle       | How It Applies                                                                 |
|-----------------|----------------------------------------------------------------------------------|
| **S - SRP**      | Object creation logic is separated from business logic.             |
| **O - OCP**      | New product types can be added without changing client code.               |
| **L - LSP**      | All created objects follow a common interface or base class.        |
| **I - ISP**      | Product interfaces expose only what the client needs.     |
| **D - DIP**      | High-level modules depend on abstractions (interface), not concrete classes.  |

## Summary
🧠 Factory Pattern is all about centralizing object creation and hiding the instantiation logic, making your system easier to extend and maintain.