# Abstract Factory Design Pattern

## Definition

The **Abstract Factory Pattern** provides an interface for creating families of related or dependent objects without specifying their concrete classes.

---

## Use Cases

- When your system needs to be independent of how its objects are created.
- When you want to enforce consistency among related product objects.
- When you have multiple families of products and need to switch between them easily.

---

## Real-World Analogies

- **UI Toolkit**: You can switch between MacOS and Windows-style widgets (buttons, menus).
- **Theme Engine**: Each theme provides its own styles for all components (dark theme, light theme, etc).

---

## Why Use Abstract Factory Pattern?

### ✅ Enforces Consistency
- Ensures that all components used together belong to the same family.

### ✅ Promotes Scalability
- Add new families without touching existing code.

### ✅ Decouples Product Families from Clients
- Client code depends only on interfaces.

---

## Frequently Asked in Interviews

> Commonly asked to demonstrate layered architecture and decoupling through factory abstraction.

---

## Class Structure

### 1. Product Interfaces

```java
interface Button {
    void render();
}

interface Checkbox {
    void render();
}
```

### 2. Concrete Product Families

```java
class MacButton implements Button {
    public void render() {
        System.out.println("Render Mac Button");
    }
}

class WindowsButton implements Button {
    public void render() {
        System.out.println("Render Windows Button");
    }
}

class MacCheckbox implements Checkbox {
    public void render() {
        System.out.println("Render Mac Checkbox");
    }
}

class WindowsCheckbox implements Checkbox {
    public void render() {
        System.out.println("Render Windows Checkbox");
    }
}
```

### 3. Abstract Factory Interface

```java
interface GUIFactory {
    Button createButton();
    Checkbox createCheckbox();
}
```

### 4. Concrete Factories

```java
class MacFactory implements GUIFactory {
    public Button createButton() {
        return new MacButton();
    }

    public Checkbox createCheckbox() {
        return new MacCheckbox();
    }
}

class WindowsFactory implements GUIFactory {
    public Button createButton() {
        return new WindowsButton();
    }

    public Checkbox createCheckbox() {
        return new WindowsCheckbox();
    }
}
```

### 5. Client Code

```java
class Application {
    private Button button;
    private Checkbox checkbox;

    public Application(GUIFactory factory) {
        button = factory.createButton();
        checkbox = factory.createCheckbox();
    }

    public void renderUI() {
        button.render();
        checkbox.render();
    }
}
```


### 6. Usage Example

```java
public class Main {
    public static void main(String[] args) {
        GUIFactory factory = new MacFactory(); // Or new WindowsFactory()
        Application app = new Application(factory);
        app.renderUI();
    }
}
```

---

### OOP Concepts Used

| Concept           | Description                                                                  |
|-------------------|-------------------------------------------------------------------------------|
| **Encapsulation** | Grouped families of related objects are created without exposing details.  |
| **Abstraction**   | Abstract factories define interfaces for families of products.             |
| **Polymorphism**  | Factories and products can be substituted via their interfaces.            |
| **Composition**   | Factory is composed of multiple product creation methods.                  |

### SOLID Principles Demonstrated

| Principle        | How It Applies                                                                   |
|------------------|----------------------------------------------------------------------------------|
| **S - SRP**      | Object creation responsibility is encapsulated in factory classes.               |
| **O - OCP**      | New product families can be added with minimal/no changes to existing code.      |
| **L - LSP**      | Concrete factories and products can be replaced via interfaces.                  |
| **I - ISP**      | Abstract factories expose only product-related creation methods.                 |
| **D - DIP**      | Client depends only on abstract factories and product interfaces.                |

## Summary
🧠 Abstract Factory is ideal when you need to create families of related objects without tying your code to specific classes — think of it as a factory of factories.