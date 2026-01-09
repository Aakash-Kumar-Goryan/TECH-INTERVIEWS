# Behavioral Design Patterns

Algorithms and assignment of responsibilities between objects.

## 1. Observer Pattern

**Goal**: Define a subscription mechanism to notify multiple objects about events.
**Use Case**: YouTube Channel (Subject) -> Subscribers (Observers). When new video uploaded, notify all.
**Implemenation**: `Subject` has `List<Observer>`. `notify()` loops through list and calls `update()`.

## 2. Strategy Pattern

**Goal**: Define a family of algorithms, put each in a class, and make them interchangeable.
**Use Case**: Navigation App. `RouteStrategy` interface. Concretes: `RoadStrategy`, `WalkingStrategy`, `PublicTransportStrategy`. Client switches strategy at runtime.

## 3. Command Pattern

**Goal**: Turn a request into a stand-alone object.
**Use Case**: GUI Buttons. Copy, Paste, Undo. Store `Command` objects in a history stack to implement **Undo/Redo**.

## 4. Chain of Responsibility

**Goal**: Pass request along a chain of handlers.
**Use Case**: Support Ticket System. Level 1 Support -> Level 2 Support -> Manager. Or Logging (Debug -> Info -> Error).

## 5. State Pattern

**Goal**: Alter object behavior when internal state changes.
**Use Case**: Vending Machine.

- `NoCoinState`: InsertCoin() -> switch to `HasCoinState`.
- `HasCoinState`: EjectCoin() -> switch to `NoCoinState`.
- `HasCoinState`: SelectItem() -> switch to `SoldState`.

## 6. Mediator Pattern

**Goal**: Reduce chaotic dependencies between objects. Force them to communicate via Mediator.
**Use Case**: Air Traffic Control Tower (Mediator). Planes don't talk to each other; they talk to Tower.
