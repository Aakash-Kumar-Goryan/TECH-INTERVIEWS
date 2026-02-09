# Standard Java REST API Project Structure & Quick Revision Guide

## 1. Standard Repository Structure (Maven/Gradle)

A typical production-grade Java Spring Boot application follows a layered architecture.

```text
my-app/
├── pom.xml                   # Dependencies & Build Config
├── src/
│   ├── main/
│   │   ├── java/
│   │   │   └── com/example/myapp/
│   │   │       ├── MyAppApplication.java  # Main Entry Point
│   │   │       ├── config/                # App Configuration (Security, Swagger)
│   │   │       ├── controller/            # [Controller] Input/Output handling
│   │   │       ├── service/               # [Model] Core Business Logic
│   │   │       │   ├── impl/              # Service Implementations
│   │   │       │   └── SomeService.java   # Service Interface
│   │   │       ├── repository/            # [Model] Data Access Layer
│   │   │       ├── entity/                # [Model] DB Tables (Entities)
│   │   │       ├── dto/                   # [View] JSON Response Objects
│   │   │       ├── mapper/                # Object mapping (Entity <-> DTO)
│   │   │       ├── exception/             # Global Exception Handling
│   │   │       └── util/                  # Helper classes (Dates, Strings)
│   │   ├── resources/
│   │   │   ├── application.yml            # App properties (DB url, port, profiles)
│   │   │   ├── static/                    # HTML/JS/CSS (rare in pure REST API)
│   │   │   ├── templates/                 # [View] HTML Templates (if using Thymeleaf)
│   │   │   └── db/migration/              # Flyway/Liquibase scripts
│   └── test/
│       └── java/
│           └── com/example/myapp/         # Unit and Integration Tests
```

---

## 2. Component Breakdown

| Package | Purpose | Typical Annotations |
| :--- | :--- | :--- |
| **`config`** | Setup beans, security filters, CORS, Swagger/OpenAPI. | `@Configuration`, `@Bean`, `@EnableWebSecurity` |
| **`controller`** | Handle HTTP requests, validate input, return responses. **No logic here.** | `@RestController`, `@RequestMapping`, `@GetMapping`, `@PostMapping` |
| **`service`** | Core business logic, transactions, complex calculations. | `@Service`, `@Transactional` |
| **`repository`** | Interface with the database. | `@Repository`, `@Query` (extends `JpaRepository`) |
| **`entity`** (or `model`) | Represents database tables. | `@Entity`, `@Table`, `@Id`, `@Column` |
| **`dto`** | Pure data carriers for API inputs/outputs. Decouples API from DB schema. | `@Data`, `@Builder` (Lombok) |
| **`exception`** | Centralized error handling. | `@ControllerAdvice`, `@ExceptionHandler` |

---

## 3. Quick Revision Strategy: "How to Read"

When you are given a new codebase or need to revise quickly for an interview (System Design or LLD), following the **Request Lifecycle flow** is the most efficient method.

### **Step 1: The Bird's Eye View (Build File)**

* **Action**: Open `pom.xml` or `build.gradle`.
* **Why**: Quickest way to know the tech stack.
  * *Is it Spring Boot?* (Look for `spring-boot-starter-parent`)
  * *What DB?* (MySQL, Postgres, Mongo?)
  * *ORM?* (Hibernate, MyBatis?)
  * *Utilities?* (Lombok, MapStruct, Gson/Jackson?)

### **Step 2: The Inputs (Controller Layer)**

* **Action**: Go to `src/main/java/.../controller`.
* **Why**: Tells you **WHAT** the application does.
  * Look at `@GetMapping`, `@PostMapping` paths.
  * *Example*: If you see `OrderController` with `/checkout`, you know this app processes orders.
  * *Revision Tip*: Don't read method bodies yet. Just read method names and URL paths.

### **Step 3: The Data Model (Entity Layer)**

* **Action**: Go to `src/main/java/.../entity`.
* **Why**: Tells you **HOW** data is structured.
  * Look at fields and relationships (`@OneToMany`, `@ManyToOne`).
  * *Example*: `Order` has a list of `OrderItem`. `User` has a `Role`.
  * *Revision Tip*: This helps you visualize the database schema in your head.

### **Step 4: The Logic (Service Layer)** - *Deep Dive Here*

* **Action**: Go to `src/main/java/.../service`.
* **Why**: This is where the magic happens.
  * Trace the call from Controller -> Service.
  * Look for validation logic, calculations, and external API calls.
  * Check `@Transactional` boundaries.

### **Step 5: The wiring (Config & Properties)**

* **Action**: Check `application.yml` and `config` package.
* **Why**: understand the environment.
  * Which port? What DB connection?
  * Any external API keys?
  * Security configurations (JWT, OAuth)?

---

## 4. Summary: The 30-Second Scan
>
>**Scenario**: "Here is a repo, explain what it does."

1. **Pom.xml**: "It's a Spring Boot app using PostgreSQL and Kafka."
2. **Controller**: "It exposes endpoints for User Management and Payment Processing."
3. **Entity**: "The core entities are User, Wallet, and Transaction."
4. **Service**: "The `PaymentService` handles the deduction logic and updates the Wallet entity."

---

## 5. AI Prompting Strategy: Debugging User Scenarios

When you need to debug a specific user scenario (e.g., "Payment Failed") using an AI tool, use this structured prompt context.

**Template:**
> "I have a Java Spring Boot application.
> **Scenario**: [Describe the user action, e.g., User clicks 'Pay Now']
> **Observation**: [Describe the error, e.g., Returns 500 Internal Server Error]
>
> Here are the relevant files for this flow:
>
> 1. Controller: [Paste Controller Code]
> 2. Service: [Paste Service Code]
> 3. Repository: [Paste Repository Interface]
>
> Please trace the request flow and identify potential bugs, specifically looking for:
>
> * NullPointerExceptions
> * Transaction rollback issues
> * Incorrect Validation logic"

**Pro-Tip**: Always paste the *Interface* (Service/Controller) and the *Implementation* together if they are separate.

---

## 6. Appendix: Build Configuration Templates (Reference)

<details>
<summary><b>Click to expand Maven (pom.xml) & Gradle (build.gradle) Templates</b></summary>

### **Option A: Maven (`pom.xml`)**

Standard structure for a Spring Boot application.

```xml
<?xml version="1.0" encoding="UTF-8"?>
<project xmlns="http://maven.apache.org/POM/4.0.0" 
         xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
         xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 https://maven.apache.org/xsd/maven-4.0.0.xsd">
    <modelVersion>4.0.0</modelVersion>
    
    <!-- Spring Boot Parent -->
    <parent>
        <groupId>org.springframework.boot</groupId>
        <artifactId>spring-boot-starter-parent</artifactId>
        <version>3.2.0</version>
        <relativePath/> <!-- lookup parent from repository -->
    </parent>
    
    <groupId>com.example</groupId>
    <artifactId>myapp</artifactId>
    <version>0.0.1-SNAPSHOT</version>
    <name>myapp</name>
    <description>Demo project for Spring Boot</description>
    
    <properties>
        <java.version>17</java.version>
    </properties>
    
    <dependencies>
        <!-- Web Starter (REST API, Tomcat) -->
        <dependency>
            <groupId>org.springframework.boot</groupId>
            <artifactId>spring-boot-starter-web</artifactId>
        </dependency>
        
        <!-- JPA / Database -->
        <dependency>
            <groupId>org.springframework.boot</groupId>
            <artifactId>spring-boot-starter-data-jpa</artifactId>
        </dependency>
        <dependency>
            <groupId>com.h2database</groupId>
            <artifactId>h2</artifactId>
            <scope>runtime</scope>
        </dependency>
        
        <!-- Utilities -->
        <dependency>
            <groupId>org.projectlombok</groupId>
            <artifactId>lombok</artifactId>
            <optional>true</optional>
        </dependency>
        
        <!-- Testing -->
        <dependency>
            <groupId>org.springframework.boot</groupId>
            <artifactId>spring-boot-starter-test</artifactId>
            <scope>test</scope>
        </dependency>
    </dependencies>
    
    <build>
        <plugins>
            <plugin>
                <groupId>org.springframework.boot</groupId>
                <artifactId>spring-boot-maven-plugin</artifactId>
                <configuration>
                    <excludes>
                        <exclude>
                            <groupId>org.projectlombok</groupId>
                            <artifactId>lombok</artifactId>
                        </exclude>
                    </excludes>
                </configuration>
            </plugin>
        </plugins>
    </build>
</project>
```

### **Option B: Gradle (`build.gradle`)**

The clean, modern Groovy DSL approach.

```groovy
plugins {
    id 'java'
    id 'org.springframework.boot' version '3.2.0'
    id 'io.spring.dependency-management' version '1.1.4'
}

group = 'com.example'
version = '0.0.1-SNAPSHOT'

java {
    sourceCompatibility = '17'
}

configurations {
    compileOnly {
        extendsFrom annotationProcessor
    }
}

repositories {
    mavenCentral()
}

dependencies {
    // core
    implementation 'org.springframework.boot:spring-boot-starter-web'
    implementation 'org.springframework.boot:spring-boot-starter-data-jpa'
    
    // db
    runtimeOnly 'com.h2database:h2'
    
    // utils
    compileOnly 'org.projectlombok:lombok'
    annotationProcessor 'org.projectlombok:lombok'
    
    // test
    testImplementation 'org.springframework.boot:spring-boot-starter-test'
}

tasks.named('test') {
    useJUnitPlatform()
}
```

</details>
