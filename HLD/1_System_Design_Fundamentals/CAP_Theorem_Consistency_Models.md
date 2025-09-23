# CAP Theorem and Consistency Models

This document expands on the section from the [System Design Fundamentals Explanation](./Explanation.md) and provides deeper insights into distributed system theory and consistency models.

## CAP Theorem
The CAP Theorem states that in any distributed data system, it is impossible to simultaneously guarantee all three of the following:

- **Consistency:** Every read receives the most recent write or an error.
- **Availability:** Every request receives a (non-error) response, without guarantee that it contains the most recent write.
- **Partition Tolerance:** The system continues to operate despite arbitrary network partitioning (communication breaks between nodes).

**Implications:**
- In the presence of a network partition, a system must choose between consistency and availability.
- Real-world systems often favor partition tolerance and then choose either consistency (CP systems, e.g., HBase, MongoDB) or availability (AP systems, e.g., Cassandra, DynamoDB).

## Consistency Models
Consistency models define the rules for visibility and ordering of updates in distributed systems. They determine how and when changes made by one client become visible to others.

- **Strong Consistency:** After an update, all clients see the same data instantly (e.g., relational databases, some NoSQL systems in CP mode).
- **Eventual Consistency:** Updates will propagate to all nodes, but not immediately. Eventually, all nodes will have the same data (e.g., DynamoDB, Cassandra).
- **Causal Consistency:** Writes that are causally related are seen by all nodes in the same order; concurrent writes may be seen in different orders.
- **Read-Your-Writes Consistency:** After a client writes data, it will always read its own updates.
- **Session Consistency:** Guarantees read-your-writes consistency within a session.

## Design Considerations
- Choose the consistency model based on application requirements (e.g., banking needs strong consistency, social media can use eventual consistency).
- Understand trade-offs: higher consistency often means lower availability or performance.
- Use replication, sharding, and conflict resolution strategies to balance consistency and availability.

---
For foundational concepts, see the [System Design Fundamentals Explanation](./Explanation.md).
