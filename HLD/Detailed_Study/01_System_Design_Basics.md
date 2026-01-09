# Explanation: System Design Fundamentals

This document provides detailed explanations for each topic listed in the System Design Fundamentals notes.

## Requirements Gathering
- **Functional requirements** specify what the system should do (features, behaviors, services).
- **Non-functional requirements** define system qualities (performance, scalability, security, usability).
- **Use cases** describe how users interact with the system.
- **Constraints** are limitations (technology, budget, deadlines) that affect design choices.

## API Design
- **REST**: Architectural style using HTTP methods (GET, POST, etc.), stateless communication, and resource-based URLs.
- **gRPC**: High-performance, open-source RPC framework using Protocol Buffers, suitable for microservices. For more, see [gRPC in API Design](./API_Design.md#grpc).
- **GraphQL**: Query language for APIs, allows clients to request exactly the data they need.
- **Versioning**: Managing changes to APIs without breaking existing clients (e.g., v1, v2 in URLs).
- **Documentation**: Clear, up-to-date docs (Swagger/OpenAPI) help consumers use APIs correctly.

For a detailed guide, see [API Design: Concepts and Best Practices](./API_Design.md).


## Scalability, Reliability, Availability, Maintainability
- **Scalability**: Ability to handle increased load by adding resources (horizontal/vertical scaling).
- **Reliability**: System consistently performs as expected, even under failure conditions.
- **Availability**: Percentage of time the system is operational (measured as uptime, e.g., 99.99%).
- **Maintainability**: Ease of making changes, fixing bugs, and adding features.
- **Trade-offs**: Improving one quality may impact others (e.g., higher availability may reduce maintainability).

## CAP Theorem, Consistency Models
- **CAP Theorem**: In a distributed system, you can only guarantee two of Consistency, Availability, and Partition Tolerance at the same time.
- **Consistency**: All nodes see the same data at the same time.
- **Availability**: Every request receives a response (success or failure).
- **Partition Tolerance**: System continues to operate despite network splits.
- **Consistency Models**: Strong (immediate), eventual (over time), causal, etc.

## Latency vs Throughput
- **Latency**: Time taken to process a single request (measured in ms or s).
- **Throughput**: Number of requests processed per unit time (e.g., requests/sec).
- **Optimization**: Reducing latency improves user experience; increasing throughput improves system capacity.
