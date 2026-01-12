# Exception Handling

> *Exceptions are Java's way of saying "something went wrong." Let's explore exception handling through the story of preventing and recovering from disasters...*

---

## 🎬 The Exception Story

Imagine you're building a spaceship. Things can go wrong:

- The fuel tank is empty (recoverable: refuel!)
- The engine explodes (unrecoverable: abandon ship!)

Java has the same distinction!

---

## 📖 Chapter 1: The Exception Family Tree

```
                           👑 Throwable
                          (The ancestor)
                         /              \
                        /                \
               💀 Error                 ⚠️ Exception
          (Abandon ship!)            (We can fix this!)
               |                           |
        ┌──────┴──────┐          ┌─────────┴──────────┐
        |             |          |                    |
   OutOfMemory   StackOver   RuntimeException    Checked
      Error       flow        (Unchecked)       Exceptions
                               |                    |
                        ┌──────┴──────┐      ┌──────┴──────┐
                   NullPointer  Illegal    IOException  SQLException
                   Exception   Argument
```

### The Three Categories

| Type | Examples | Handle? | Philosophy |
|------|----------|---------|------------|
| **Error** 💀 | `OutOfMemoryError`, `StackOverflowError` | ❌ Don't catch | JVM is dying. You can't help. |
| **Checked** ✋ | `IOException`, `SQLException` | ✅ Must handle | "Bad things happen to good code" |
| **Unchecked** 🐛 | `NullPointerException`, `IllegalArgumentException` | ⚡ Optional | "Fix your bugs!" |

---

## 📖 Chapter 2: Checked vs Unchecked

### Checked Exceptions: "Expect the Unexpected"

```java
// The file might not exist. That's NORMAL. Handle it!
public String readFile(String path) throws IOException {  // Must declare!
    return Files.readString(Path.of(path));
}

// Caller MUST handle
try {
    String content = readFile("data.txt");
} catch (IOException e) {
    System.out.println("File not found, using defaults");
    content = getDefaultContent();
}
```

**Checked exceptions are for recoverable situations:**

- File not found → use default
- Network timeout → retry
- Database connection lost → use backup

### Unchecked Exceptions: "Fix Your Code!"

```java
public void process(String input) {
    input.length();  // If input is null → NullPointerException!
}

// These are BUGS. Don't catch, FIX!
// ❌ BAD: Catching a bug
try {
    process(null);
} catch (NullPointerException e) {
    // This is a bandaid on a bug
}

// ✅ GOOD: Prevent the bug
if (input != null) {
    process(input);
}

// ✅ EVEN BETTER: Fail fast with clear message
public void process(String input) {
    Objects.requireNonNull(input, "Input cannot be null");
    input.length();
}
```

---

## 📖 Chapter 3: The try-catch-finally Dance

### The Basic Pattern

```java
try {
    // The risky code
    FileReader fr = new FileReader("data.txt");
    char c = (char) fr.read();
    
} catch (FileNotFoundException e) {
    // Handle specific problem
    System.out.println("File doesn't exist!");
    
} catch (IOException e) {
    // Handle general IO problems
    System.out.println("Can't read file!");
    
} finally {
    // ALWAYS runs - cleanup!
    System.out.println("Cleanup complete");
}
```

### The Execution Order Visualized

```
Normal execution:
  try block → finally → continue

Exception caught:
  try block → [EXCEPTION!] → matching catch → finally → continue

Exception NOT caught:
  try block → [EXCEPTION!] → finally → propagate up

Return in try:
  try block → return X → finally → actually return X
                          ↑
                     (still runs!)
```

### Multi-catch (Java 7+)

```java
// Before: Duplicate code 😩
try {
    riskyOperation();
} catch (IOException e) {
    log.error("Failed", e);
    throw new ServiceException(e);
} catch (SQLException e) {
    log.error("Failed", e);  // Same code!
    throw new ServiceException(e);
}

// After: Clean! 🎉
try {
    riskyOperation();
} catch (IOException | SQLException e) {
    log.error("Failed", e);
    throw new ServiceException(e);
}
```

### The finally Trap ⚠️

```java
public int tricky() {
    try {
        return 1;
    } finally {
        return 2;  // ⚠️ OVERRIDES the return 1!
    }
}
// Returns 2, not 1!

// NEVER return from finally - it's confusing and can swallow exceptions!
```

---

## 📖 Chapter 4: try-with-resources (The Game Changer)

**The Story**: Before Java 7, closing resources was a nightmare.

### The Old Way (Messy)

```java
BufferedReader br = null;
try {
    br = new BufferedReader(new FileReader("file.txt"));
    return br.readLine();
} catch (IOException e) {
    // Handle error
} finally {
    if (br != null) {
        try {
            br.close();  // close() can also throw! 😱
        } catch (IOException e) {
            // What do we do here?!
        }
    }
}
```

### The New Way (Clean!)

```java
try (BufferedReader br = new BufferedReader(new FileReader("file.txt"))) {
    return br.readLine();
}  // Automatically closed! Even if exception occurs!

// Multiple resources (closed in reverse order)
try (InputStream in = new FileInputStream(src);
     OutputStream out = new FileOutputStream(dest)) {
    in.transferTo(out);
}
// out closes first, then in
```

### How It Works

Any class implementing `AutoCloseable` works:

```java
public interface AutoCloseable {
    void close() throws Exception;
}

// Your own resources!
public class MyConnection implements AutoCloseable {
    public MyConnection() {
        System.out.println("Opened!");
    }
    
    @Override
    public void close() {
        System.out.println("Closed!");
    }
}

try (MyConnection conn = new MyConnection()) {
    // Use conn
}  // "Closed!" printed automatically
```

### Suppressed Exceptions

What if BOTH the try block AND close() throw?

```java
try (ProblematicResource r = new ProblematicResource()) {
    throw new Exception("Primary error");
}  // close() also throws!

// The primary exception wins!
// close() exception is "suppressed" but saved

catch (Exception e) {
    System.out.println(e.getMessage());  // "Primary error"
    
    for (Throwable t : e.getSuppressed()) {
        System.out.println("Also: " + t.getMessage());  // close() error
    }
}
```

---

## 📖 Chapter 5: Custom Exceptions

**The Story**: Standard exceptions don't always tell the full story. Add context!

```java
public class UserNotFoundException extends RuntimeException {
    private final String userId;
    private final String searchLocation;
    
    public UserNotFoundException(String userId, String location) {
        super(String.format("User '%s' not found in %s", userId, location));
        this.userId = userId;
        this.searchLocation = location;
    }
    
    // Constructor with cause (for exception chaining)
    public UserNotFoundException(String userId, Throwable cause) {
        super("User not found: " + userId, cause);
        this.userId = userId;
        this.searchLocation = "unknown";
    }
    
    public String getUserId() { return userId; }
    public String getSearchLocation() { return searchLocation; }
}

// Usage
throw new UserNotFoundException("12345", "LDAP directory");

// Catch and extract context
catch (UserNotFoundException e) {
    log.error("User {} not found in {}", 
        e.getUserId(), e.getSearchLocation());
}
```

### When to Create Custom Exceptions

| Scenario | Do It? |
|----------|--------|
| Need to carry extra context (userId, orderId) | ✅ Yes |
| Part of public API contract | ✅ Yes |
| Distinguish from standard exceptions | ✅ Yes |
| Standard exception already exists | ❌ No |

---

## 📖 Chapter 6: Exception Best Practices

### ✅ DO

```java
// 1. Catch specific exceptions first (most specific → least specific)
try {
    readFile();
} catch (FileNotFoundException e) {
    // Handle file missing
} catch (IOException e) {
    // Handle other IO issues
}

// 2. Always log OR rethrow (never both!)
catch (Exception e) {
    log.error("Operation failed", e);
    // OR
    throw new ServiceException("Operation failed", e);
    // NOT BOTH - avoids duplicate logs!
}

// 3. Use try-with-resources for all closeable resources
try (Connection conn = dataSource.getConnection();
     PreparedStatement ps = conn.prepareStatement(sql)) {
    // Use resources
}

// 4. Fail fast with validation
public void process(@NotNull String input) {
    if (input == null) {
        throw new IllegalArgumentException("Input cannot be null");
    }
    if (input.isBlank()) {
        throw new IllegalArgumentException("Input cannot be blank");
    }
    // Now we know input is valid!
}

// 5. Preserve the cause
catch (SQLException e) {
    throw new RepositoryException("Failed to save user", e);  // Chain!
}
```

### ❌ DON'T

```java
// 1. Don't catch Throwable or Error
try {
    code();
} catch (Throwable t) {  // Catches OutOfMemoryError!
    // Can't recover from this anyway
}

// 2. Don't swallow exceptions
catch (Exception e) {
    // Empty! The exception vanished!
}

// 3. Don't use exceptions for control flow
// BAD
try {
    while (true) {
        array[i++]; 
    }
} catch (ArrayIndexOutOfBoundsException e) {
    // "End of array"
}

// GOOD
for (int i = 0; i < array.length; i++) {
    array[i];
}

// 4. Don't log AND rethrow (duplicate logs!)
catch (Exception e) {
    log.error("Error", e);
    throw e;  // Will be logged AGAIN by caller!
}
```

---

## 📖 Chapter 7: Exception Chaining

**The Story**: Exceptions often have underlying causes. Preserve the trail!

```java
public void saveUser(User user) throws ServiceException {
    try {
        repository.save(user);
    } catch (SQLException e) {
        // Wrap low-level exception in high-level exception
        throw new ServiceException("Failed to save user " + user.getId(), e);
    }
}

// Later, when debugging:
catch (ServiceException e) {
    System.out.println(e.getMessage());  
    // "Failed to save user 123"
    
    System.out.println(e.getCause().getMessage());  
    // "Connection refused: DB is down"
    
    e.printStackTrace();  
    // Shows the FULL chain!
}
```

---

## 🎯 Quick Reference

```
┌─────────────────────────────────────────────────────────────────────────┐
│                    EXCEPTION QUICK REFERENCE                            │
├─────────────────────────────────────────────────────────────────────────┤
│  Checked Exception    │ Must catch or declare with throws              │
│  Unchecked Exception  │ RuntimeException family, optional handling     │
│  Error                │ DON'T catch - JVM is dying                     │
├─────────────────────────────────────────────────────────────────────────┤
│  try-with-resources   │ Automatic close() for AutoCloseable           │
│  Multi-catch          │ catch (E1 | E2 e) { }                          │
│  Exception chaining   │ new MyException("msg", cause)                  │
├─────────────────────────────────────────────────────────────────────────┤
│  ✅ Catch specific first                                               │
│  ✅ Log OR rethrow (not both)                                          │
│  ✅ Use try-with-resources                                             │
│  ❌ Don't swallow exceptions                                           │
│  ❌ Don't use exceptions for flow control                              │
└─────────────────────────────────────────────────────────────────────────┘
```
