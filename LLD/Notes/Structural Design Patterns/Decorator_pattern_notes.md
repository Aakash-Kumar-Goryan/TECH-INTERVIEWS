# Decorator Design Pattern

## Use Cases

The **Decorator Pattern** allows you to dynamically add behavior to objects without altering their structure.

### Examples

- **Pizza**  
  `BasePizza + Extra Cheese + Mushroom + Extra Cheese`

- **Car**  
  `BaseCar + Seat Cover + AC + Power Steering`

- **Coffee**  
  `BaseCoffee + Milk + Sugar + Cream` (etc.)

---

## Why Use the Decorator Pattern?

### ✅ Problem: Class Explosion

- Large number of subclasses needed to represent every combination of features.
- Example:  
  - `PizzaWithCheeseAndMushroom`  
  - `PizzaWithCheeseOnly`  
  - `PizzaWithMushroomOnly`  
  - etc.

### ✅ Solution: Decorator Pattern

- Adds features (like toppings or add-ons) at **runtime**
- Promotes **flexibility** and **reusability**
- Follows the **Open/Closed Principle**  
  (open for extension, closed for modification)

---

## Interview Relevance

> This is a **frequently asked** design pattern in **object-oriented** and **system design** interviews.

---

## Class Structure Example: Pizza Cost Calculator

### 1. Abstract Base Class

```java
abstract class BasePizza {
    abstract int cost();
}
```

### 2. Concrete Implementations
```java
class Margherita extends BasePizza {
    int cost() {
        return 100;
    }
}

class VegDelight extends BasePizza {
    int cost() {
        return 200;
    }
}
```
### 3. Abstract Decorator
```java
abstract class ToppingDecorator extends BasePizza {
    BasePizza basePizza;

    ToppingDecorator(BasePizza basePizza) {
        this.basePizza = basePizza;
    }
}
```
📌 Note: ToppingDecorator is both a BasePizza (is-a) and wraps a BasePizza (has-a).

### 4. Concrete Decorators

```java
class ExtraCheese extends ToppingDecorator {
    ExtraCheese(BasePizza basePizza) {
        super(basePizza);
    }

    int cost() {
        return basePizza.cost() + 20;
    }
}

class Mushroom extends ToppingDecorator {
    Mushroom(BasePizza basePizza) {
        super(basePizza);
    }

    int cost() {
        return basePizza.cost() + 20;
    }
}
```

### 5. Usage Example

```java
BasePizza pizza = new ExtraCheese(
                    new Mushroom(
                        new ExtraCheese(
                            new Margherita()
                        )
                    )
                 );

System.out.println(pizza.cost()); // Output: 100 + 20 + 20 + 20 = 160
```
---

## OOP Concepts Used

| Concept         | Description                                                                 |
|----------------|-----------------------------------------------------------------------------|
| **Encapsulation** | Each decorator wraps functionality without changing the original object.   |
| **Abstraction**   | The base class (`BasePizza`) provides a common interface.                  |
| **Polymorphism**  | Decorators and concrete components are interchangeable via the base class. |
| **Composition**   | Decorators wrap other objects to add behavior dynamically.                 |

## SOLID Principles Demonstrated

| Principle       | How It Applies                                                                 |
|-----------------|----------------------------------------------------------------------------------|
| **S - SRP**      | Each class (pizza or topping) has a single responsibility.                     |
| **O - OCP**      | New toppings can be added without changing existing pizza classes.             |
| **L - LSP**      | Any `BasePizza` or `ToppingDecorator` can be substituted for one another.      |
| **I - ISP**      | Only relevant methods (`cost()`) are exposed — no unnecessary responsibilities.|
| **D - DIP**      | High-level modules depend on abstractions (`BasePizza`) not concrete classes.  |

## Key Concepts Recap
- **IS-A**: Decorators extend BasePizza so they can be used anywhere a BasePizza is expected.
- **HAS-A**: Each decorator wraps another BasePizza object.
- **Composability**: You can layer decorators to form complex feature combinations.

🧠 Think of decorators like layers — each adds new functionality to the original object without modifying its structure.