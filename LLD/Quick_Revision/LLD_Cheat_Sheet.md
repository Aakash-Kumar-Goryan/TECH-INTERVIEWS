# LLD Cheat Sheet

## 1. SOLID Principles

- **S**ingle Responsibility: Class = 1 Job.
- **O**pen/Closed: Extend, don't modify.
- **L**iskov Substitution: Child can replace Parent.
- **I**nterface Segregation: Tiny interfaces > Huge interface.
- **D**ependency Inversion: Depend on abstractions (Interface), not implementations (Class).

## 2. Design Patterns Quick Ref

### Creational

- **Singleton**: One instance (One DB Connection).
- **Factory**: Interface for creating objects (CreditCardFactory).
- **Builder**: Step-by-step construction (Complex objects).

### Structural

- **Adapter**: Compatible wrapper (US to EU Plug).
- **Decorator**: Add features dynamically (Pizza Toppings).
- **Facade**: Simple interface for complex system (One button start car).
- **Proxy**: Placeholder (Lazy load images).

### Behavioral

- **Strategy**: Swap algorithms (Maps Route: Car/Walk).
- **Observer**: Notify listeners (Youtube Subs).
- **Command**: Encapsulate request (Undo/Redo).
- **State**: Change behavior on state (Vending Machine).

## 3. Class Relationships

- **Is-A**: Inheritance (`extends`).
- **Has-A (Strong)**: Composition (House/Room). diesWithParent = True.
- **Has-A (Weak)**: Aggregation (School/Student). diesWithParent = False.
- **Uses-A**: Association.

## 4. Common LLD Questions

- **Parking Lot**: Muti-floor, Vehicle Types, Pricing Strategy.
- **Elevator**: State Pattern, Dispatch Algorithm.
- **Snake & Ladder**: Game Loop, Board Entities.
- **Hotel Management**: Booking, Room Types, Date Range overlap.
- **ATM**: State Pattern (HasCard, HasPin, Dispensing).
