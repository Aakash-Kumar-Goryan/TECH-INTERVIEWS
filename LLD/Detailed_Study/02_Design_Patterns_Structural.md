# Structural Design Patterns

How to assemble objects and classes into larger structures.

## 1. Adapter Pattern

**Goal**: Allows incompatible interfaces to work together.
**Analogy**: Using a US-to-EU power plug adapter.
**Use Case**: Your app expects `XmlData` interface, but you want to use a 3rd party library that returns `JsonData`. Create a `JsonToXmlAdapter` wrapper.

## 2. Decorator Pattern

**Goal**: Add dynamic behavior to objects without inheritance.
**Analogy**: Pizza toppings. `BasePizza` -> `CheeseDecorator` -> `PepperoniDecorator`.
**Use Case**: `FileStream` -> `BufferedStream` -> `EncryptedStream`.

## 3. Facade Pattern

**Goal**: Provide a simplified interface to a complex library/subsystem.
**Use Case**: A `VideoConverter` facade that hides the complexity of `CodecFactory`, `AudioMixer`, `BitrateReader`, etc. "Just call `convert(file)`".

## 4. Proxy Pattern

**Goal**: Provide a substitute or placeholder for another object.
**Use Case**:

- **Virtual Proxy**: Lazy loading massive images.
- **Protection Proxy**: Check access rights (Admin/User).
- **Remote Proxy**: Representing an object on a remote server.

## 5. Composite Pattern

**Goal**: Compose objects into tree structures (Part-Whole). Treat individual objects and compositions uniformly.
**Use Case**: File System (Folder contains Files and Folders). `Folder.getSize()` calls `getSize()` on children.
