# Exception Handling

Proper exception handling separates junior from senior developers.

## 1. Exception Hierarchy

```
Throwable
├── Error (Don't catch - unrecoverable)
│   ├── OutOfMemoryError
│   ├── StackOverflowError
│   └── VirtualMachineError
│
└── Exception
    ├── RuntimeException (Unchecked)
    │   ├── NullPointerException
    │   ├── IllegalArgumentException
    │   ├── IndexOutOfBoundsException
    │   └── ClassCastException
    │
    └── Checked Exceptions
        ├── IOException
        ├── SQLException
        └── ClassNotFoundException
```

---

## 2. Checked vs Unchecked Exceptions

### Checked Exceptions

Must be caught or declared in method signature.

```java
public void readFile(String path) throws IOException {
    FileReader fr = new FileReader(path);  // Throws checked exception
}

// Caller must handle
try {
    readFile("data.txt");
} catch (IOException e) {
    logger.error("Failed to read file", e);
}
```

**Examples**: `IOException`, `SQLException`, `ClassNotFoundException`

**Philosophy**: Recoverable situations (file not found, network issue).

### Unchecked Exceptions (RuntimeException)

No compile-time requirement to handle.

```java
public int divide(int a, int b) {
    return a / b;  // May throw ArithmeticException
}

// Can throw without declaring
public void process(String s) {
    s.length();  // May throw NullPointerException
}
```

**Examples**: `NullPointerException`, `IllegalArgumentException`, `ArrayIndexOutOfBoundsException`

**Philosophy**: Programming errors (bugs). Fix the code, don't catch.

---

## 3. try-catch-finally

### Basic Structure

```java
try {
    riskyOperation();
} catch (SpecificException e) {
    handleSpecific(e);
} catch (GeneralException e) {
    handleGeneral(e);
} finally {
    cleanup();  // ALWAYS executes
}
```

### Multi-catch (Java 7+)

```java
try {
    riskyOperation();
} catch (IOException | SQLException e) {
    logger.error("IO or SQL error", e);
}
```

**Note**: Caught exception is implicitly `final`.

### finally Behavior

```java
public int test() {
    try {
        return 1;
    } finally {
        return 2;  // Overrides! Returns 2.
    }
}
```

**Warning**: Never return from finally. Suppresses exceptions.

---

## 4. try-with-resources (Java 7+)

Automatically closes `AutoCloseable` resources.

```java
// Before Java 7
BufferedReader br = null;
try {
    br = new BufferedReader(new FileReader("file.txt"));
    return br.readLine();
} finally {
    if (br != null) {
        br.close();  // May throw, masking original exception
    }
}

// Java 7+
try (BufferedReader br = new BufferedReader(new FileReader("file.txt"))) {
    return br.readLine();
}  // Automatically closed

// Multiple resources
try (InputStream in = new FileInputStream(src);
     OutputStream out = new FileOutputStream(dest)) {
    // Both closed in reverse order
}
```

### AutoCloseable Interface

```java
public interface AutoCloseable {
    void close() throws Exception;
}

// Closeable (IO) extends AutoCloseable
public interface Closeable extends AutoCloseable {
    void close() throws IOException;
}
```

### Custom AutoCloseable

```java
public class DatabaseConnection implements AutoCloseable {
    public DatabaseConnection() {
        System.out.println("Connection opened");
    }
    
    @Override
    public void close() {
        System.out.println("Connection closed");
    }
}

try (DatabaseConnection conn = new DatabaseConnection()) {
    // Use connection
}  // close() called automatically
```

---

## 5. Suppressed Exceptions

When close() throws while handling another exception:

```java
try (Resource r = new Resource()) {
    throw new Exception("Primary");
}
// If close() throws, it becomes suppressed

catch (Exception e) {
    System.out.println(e.getMessage());  // "Primary"
    for (Throwable t : e.getSuppressed()) {
        System.out.println(t.getMessage());  // Exception from close()
    }
}
```

---

## 6. Custom Exceptions

```java
public class UserNotFoundException extends RuntimeException {
    private final String userId;
    
    public UserNotFoundException(String userId) {
        super("User not found: " + userId);
        this.userId = userId;
    }
    
    public UserNotFoundException(String userId, Throwable cause) {
        super("User not found: " + userId, cause);
        this.userId = userId;
    }
    
    public String getUserId() {
        return userId;
    }
}

// Usage
throw new UserNotFoundException("12345");
```

### When to Create Custom Exceptions

- Need to carry additional context (userId, orderNumber)
- Distinguish from standard exceptions
- Part of public API contract

---

## 7. Exception Best Practices

### DO

✅ **Catch specific exceptions first**

```java
try {
    // code
} catch (FileNotFoundException e) {
    // Handle specifically
} catch (IOException e) {
    // Handle general IO
}
```

✅ **Always log or rethrow**

```java
catch (Exception e) {
    logger.error("Operation failed", e);
    throw new ServiceException("Could not complete operation", e);
}
```

✅ **Use try-with-resources**

```java
try (Connection conn = dataSource.getConnection()) {
    // Use connection
}
```

✅ **Validate early, fail fast**

```java
public void process(String input) {
    if (input == null) {
        throw new IllegalArgumentException("Input cannot be null");
    }
    // Process
}
```

### DON'T

❌ **Catch `Throwable` or `Error`**

```java
try {
    // code
} catch (Throwable t) {  // Catches OutOfMemoryError!
    // BAD
}
```

❌ **Swallow exceptions**

```java
try {
    // code
} catch (Exception e) {
    // Empty - exception is lost!
}
```

❌ **Use exceptions for flow control**

```java
// BAD
try {
    while (true) {
        array[i++];
    }
} catch (ArrayIndexOutOfBoundsException e) {
    // End of array
}

// GOOD
while (i < array.length) {
    array[i++];
}
```

❌ **Log and rethrow the same exception**

```java
catch (Exception e) {
    logger.error("Error", e);
    throw e;  // Will be logged twice!
}
```

---

## 8. Chained Exceptions

Preserve the root cause:

```java
public void saveUser(User user) throws ServiceException {
    try {
        repository.save(user);
    } catch (SQLException e) {
        throw new ServiceException("Failed to save user", e);  // Wrap
    }
}

// Access chain
catch (ServiceException e) {
    Throwable cause = e.getCause();  // SQLException
}
```

---

## 9. Error vs Exception

| Error | Exception |
|-------|-----------|
| Unrecoverable | Recoverable |
| JVM failures | Application issues |
| Don't catch | Catch and handle |
| `OutOfMemoryError`, `StackOverflowError` | `IOException`, `NullPointerException` |

---

## 10. Interview Questions

**Q: Can we have try without catch?**

Yes, with finally:

```java
try {
    return compute();
} finally {
    cleanup();
}
```

**Q: What happens if exception is thrown in finally?**

It replaces the original exception (original is lost unless suppressed).

**Q: Difference between `throw` and `throws`?**

| throw | throws |
|-------|--------|
| Throws an exception | Declares exceptions |
| In method body | In method signature |
| `throw new Exception();` | `void method() throws Exception` |

**Q: Can main() throw an exception?**

Yes. Uncaught exception terminates JVM with stack trace.

```java
public static void main(String[] args) throws Exception {
    throw new Exception("Boom");
}
```

**Q: Which runs first - catch or finally?**

1. Try block executes
2. If exception, matching catch runs
3. Finally ALWAYS runs (even after return/throw in catch)
