# Creational Design Patterns

Object creation mechanisms that increase flexibility and reuse.

## 1. Singleton Pattern

**Goal**: Ensure a class has only one instance and provide global access to it.
**Use Case**: Database Connection Pool, Logger, Configuration Manager.
**Implementation**: Private constructor, static instance variable, public static `getInstance()` method.

```java
public class Database {
    private static Database instance;
    private Database() {} // Private Ctor
    public static synchronized Database getInstance() {
        if (instance == null) instance = new Database();
        return instance;
    }
}
```

## 2. Factory Method Pattern

**Goal**: Define an interface for creating an object, but let subclasses decide which class to instantiate.
**Use Case**: A logistics app needs `Transport`. Subclasses `TruckTransport` and `ShipTransport` create `Truck` and `Ship` objects.

## 3. Abstract Factory Pattern

**Goal**: Produce families of related objects (GUI Factories: MacButton/MacCheckbox vs WinButton/WinCheckbox).
**Difference**: Factory Method creates *one* product. Abstract Factory creates *families* of products.

## 4. Builder Pattern

**Goal**: Construct complex objects step by step.
**Use Case**: Constructing a `House` (Walls, Doors, Windows, Garage, Pool...). Instead of `new House(4, 2, 4, true, true...)` constructor hell, use `HouseBuilder.setWalls(4).setPool(true).build()`.

## 5. Prototype Pattern

**Goal**: Copy existing objects without making functionality dependent on their classes.
**Use Case**: Cloning a complex Game Unit (Orc) instead of running initialization logic again. `orc.clone()`.
