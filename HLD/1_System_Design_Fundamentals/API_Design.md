# API Design: Concepts and Best Practices

This document expands on the API Design section from the [System Design Fundamentals Explanation](./Explanation.md).

## What is API Design?
API (Application Programming Interface) design is the process of defining how different software components or systems communicate with each other. Good API design ensures that services are easy to use, maintain, and scale.

## Key API Styles

### REST (Representational State Transfer)

REST is an architectural style for designing networked applications. It relies on a stateless, client-server protocol—almost always HTTP. RESTful APIs are widely used for web services due to their simplicity, scalability, and ease of integration.

**Key Characteristics:**
- Uses HTTP methods (GET, POST, PUT, DELETE, PATCH) for CRUD operations.
- Stateless: Each request contains all information needed for processing; the server does not store client context between requests.
- Resource-based URLs (e.g., `/users/123`), where each URL represents a resource (object or data).
- Supports multiple data formats (JSON, XML, etc.), but JSON is most common.
- Can be easily tested with tools like Postman or curl.
- Uses standard HTTP status codes for responses (e.g., 200 OK, 201 Created, 404 Not Found, 400 Bad Request).

**Example: Simple Spring Boot REST API**

Suppose you want to create a REST API for managing users. Here is a minimal example using Spring Boot (Java):

```java
// User.java (Model)
public class User {
	private Long id;
	private String name;
	private String email;
	// getters and setters
}

// UserController.java (Controller)
import org.springframework.web.bind.annotation.*;
import java.util.*;

@RestController
@RequestMapping("/users")
public class UserController {
	private Map<Long, User> users = new HashMap<>();

	@GetMapping("/{id}")
	public User getUser(@PathVariable Long id) {
		return users.get(id);
	}

	@PostMapping
	public User createUser(@RequestBody User user) {
		users.put(user.getId(), user);
		return user;
	}

	@PutMapping("/{id}")
	public User updateUser(@PathVariable Long id, @RequestBody User user) {
		users.put(id, user);
		return user;
	}

	@DeleteMapping("/{id}")
	public void deleteUser(@PathVariable Long id) {
		users.remove(id);
	}
}
```

**How it works:**
- `GET /users/1` returns the user with ID 1.
- `POST /users` creates a new user (data in request body).
- `PUT /users/1` updates the user with ID 1.
- `DELETE /users/1` deletes the user with ID 1.

**Best Practices for REST APIs:**
- Use nouns for resource names (e.g., `/users`, `/orders`).
- Use plural names for collections.
- Support filtering, sorting, and pagination for large collections.
- Use appropriate HTTP status codes.
- Provide clear error messages in the response body.
- Secure endpoints with authentication and authorization.
- Document your API (e.g., with Swagger/OpenAPI).

### gRPC

gRPC is a high-performance, open-source Remote Procedure Call (RPC) framework developed by Google. It is widely used for service-to-service communication in modern distributed systems and microservices architectures.

**Key Characteristics:**
- Uses HTTP/2 for transport, enabling multiplexed streams, header compression, and efficient connection management.
- Defines service contracts using Protocol Buffers (protobuf), which provide efficient binary serialization and strong typing.
- Supports multiple communication patterns: unary (single request/response), server streaming, client streaming, and bidirectional streaming.
- Enables code generation for client and server stubs in multiple languages (Java, Go, Python, C#, etc.).
- Built-in support for deadlines, timeouts, and cancellation.
- Provides authentication, load balancing, and health checking features.

**High-Level Design (HLD) Considerations:**
- **Service Definition:** APIs are defined in `.proto` files, specifying services, methods, and message types. This contract-first approach ensures consistency across services.
- **Inter-Service Communication:** gRPC is ideal for internal communication between microservices due to its low latency and high throughput. It is less suited for direct browser or public API consumption.
- **Streaming:** Use gRPC streaming for real-time data transfer, such as chat applications, telemetry, or live updates.
- **Error Handling:** gRPC uses a standard set of status codes for error reporting, which should be mapped to business logic errors in your services.
- **Security:** Supports TLS for encrypted communication and can integrate with authentication mechanisms (e.g., JWT, OAuth2).
- **Backward Compatibility:** Evolve protobuf schemas carefully to maintain compatibility (e.g., avoid removing fields, use reserved tags).
- **Observability:** Integrate with distributed tracing and monitoring tools to track requests across services.

**Example: gRPC Service Definition (User Service)**

```proto
// user.proto
syntax = "proto3";

service UserService {
	rpc GetUser (GetUserRequest) returns (UserResponse);
	rpc CreateUser (CreateUserRequest) returns (UserResponse);
	rpc ListUsers (ListUsersRequest) returns (stream UserResponse);
}

message GetUserRequest {
	int64 id = 1;
}

message CreateUserRequest {
	string name = 1;
	string email = 2;
}

message ListUsersRequest {}

message UserResponse {
	int64 id = 1;
	string name = 2;
	string email = 3;
}
```

**Best Practices for gRPC APIs:**
- Use clear and consistent naming in `.proto` files.
- Version your APIs by using package names or service namespacing.
- Prefer gRPC for internal APIs and high-performance use cases; use REST/GraphQL for public or browser-facing APIs.
- Document your services and messages for maintainability.
- Monitor and log gRPC traffic for troubleshooting and performance analysis.

### GraphQL

GraphQL is a query language and runtime for APIs developed by Facebook. It enables clients to request exactly the data they need and nothing more, making it highly flexible and efficient for modern applications.

**Key Characteristics:**
- Single endpoint for all queries and mutations (typically `/graphql`).
- Clients define the structure of the response, reducing over-fetching and under-fetching of data.
- Strongly typed schema defined using the GraphQL Schema Definition Language (SDL).
- Supports real-time updates via subscriptions (WebSockets).
- Introspective: clients can query the schema for available types and operations.

**High-Level Design (HLD) Considerations:**
- **Schema Design:** The schema is the contract between client and server, defining types, queries, mutations, and subscriptions. Design schemas to reflect business domains and relationships.
- **Resolvers:** Functions that resolve a value for a type or field in the schema. Each field in a query maps to a resolver on the server.
- **Data Fetching:** GraphQL servers can aggregate data from multiple sources (databases, REST APIs, microservices) in a single query.
- **N+1 Problem:** Be aware of performance issues when fetching nested data; use tools like DataLoader to batch and cache requests.
- **Authorization:** Implement fine-grained access control at the resolver or schema level.
- **Error Handling:** GraphQL responses include both data and errors, allowing partial success.
- **Versioning:** Prefer schema evolution (deprecating fields) over versioning endpoints.
- **Monitoring:** Track query complexity and depth to prevent abuse and ensure performance.

**Example: GraphQL Schema for User Management**

```graphql
type User {
	id: ID!
	name: String!
	email: String!
}

type Query {
	user(id: ID!): User
	users: [User]
}

type Mutation {
	createUser(name: String!, email: String!): User
	updateUser(id: ID!, name: String, email: String): User
	deleteUser(id: ID!): Boolean
}
```

**How it works:**
- `query { user(id: "1") { name email } }` fetches the name and email of user with ID 1.
- `mutation { createUser(name: "Alice", email: "alice@example.com") { id } }` creates a new user.

**Best Practices for GraphQL APIs:**
- Design schemas to reflect business needs and relationships.
- Use field-level authorization and validation.
- Limit query depth and complexity to prevent performance issues.
- Provide clear error messages and use the `errors` field in responses.
- Use schema documentation and introspection for discoverability.
- Monitor and log query performance and usage patterns.

## Versioning
- Prevents breaking changes for existing clients.
- Common strategies: URI versioning (`/v1/users`), header versioning, or query parameters.

## Documentation
- Essential for API adoption and correct usage.
- Tools: Swagger/OpenAPI, Postman collections.
- Should include endpoints, request/response formats, error codes, and examples.

## Best Practices
- Use consistent naming conventions (snake_case, camelCase).
- Provide meaningful error messages and status codes.
- Secure APIs with authentication and authorization.
- Rate limit to prevent abuse.
- Deprecate old versions gracefully.

---
For more foundational concepts, see the [System Design Fundamentals Explanation](./Explanation.md).
