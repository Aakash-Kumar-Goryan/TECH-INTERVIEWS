# OOP & SOLID Principles

## 1. Object Oriented Programming (OOP)

The four pillars of OOP:

1. **Encapsulation**: Bundling data (variables) and methods (functions) that act on the data into a single unit (Class). Hiding internal details (private/protected).
2. **Abstraction**: Hiding implementation details and showing only functionality to the user. (Interfaces, Abstract Classes).
3. **Inheritance**: One class acquires the properties and behaviors of another class. Promotes code reuse.
4. **Polymorphism**: "Many forms".
    - **Compile-time**: Method Overloading (Same method name, diff parameters).
    - **Runtime**: Method Overriding (Child class modifies parent method).

## 2. SOLID Principles (The Holy Bible of LLD)

Writing scalable, maintainable code requires sticking to SOLID.

### **S** - Single Responsibility Principle (SRP)

- **Definition**: A class should have only **one reason to change**.
- **Bad Example**: A `User` class that handles authentication, database saving, and email sending.
- **Good Example**: `UserAuth`, `UserRepository` (DB), `EmailService`.

### **O** - Open/Closed Principle (OCP)

- **Definition**: Classes should be **Open for extension, but Closed for modification**.
- **Example**: Instead of modifying a `calculateArea(Shape s)` method with `if (s is Circle)... else if (s is Rectangle)`, make `Shape` an interface with `area()`. Add new shapes by creating new classes, not touching existing code.

### **L** - Liskov Substitution Principle (LSP)

- **Definition**: Subtypes must be substitutable for their base types without breaking the program.
- **Classic Fail**: `Square extends Rectangle`. If you set `width` of Square, `height` changes too. A function expecting a Rectangle (where width and height change independently) will break if passed a Square.
- **Fix**: Don't inherit. Use a common `Shape` interface.

### **I** - Interface Segregation Principle (ISP)

- **Definition**: Clients should not be forced to implement interfaces they don't use.
- **Example**: Instead of one huge `Worker` interface (eat, work, sleep), split it into `Workable` and `Eatable`. A Robot implements `Workable` but not `Eatable`.

### **D** - Dependency Inversion Principle (DIP)

- **Definition**: High-level modules should not depend on low-level modules. Both should depend on abstractions.
- **Example**: `PaymentService` should not depend on `PayPalProcessor` directly. depend on `PaymentProcessor` interface. This allows swapping PayPal with Stripe easily.

## 3. UML Class Diagram Basics

- **Class**: Box with 3 sections (Name, Attributes, Methods).
- **Relationships**:
  - **Association** (`-->`): "Uses a". (Driver uses Car).
  - **Dependency** (`..>`): Short-term relationship (param in method).
  - **Aggregation** (`<>--`): "Has a" (Weak). (School has Students). If School dies, Students survive.
  - **Composition** (`<*>--`): "Has a" (Strong). (House has Rooms). If House dies, Rooms die.
  - **Inheritance/Generalization** (`--|>`): "Is a".
  - **Realization/Implementation** (`..|>`): Interface implementation.
