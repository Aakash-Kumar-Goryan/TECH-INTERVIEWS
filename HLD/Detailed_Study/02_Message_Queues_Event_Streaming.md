# Message Queues & Event Streaming

Message queues decouple services, enabling asynchronous communication. **Kafka is a top-3 FAANG interview topic**.

## 1. Core Concepts

### Point-to-Point vs Pub/Sub

| Pattern | Description | Example |
|---------|-------------|---------|
| **Point-to-Point** | One producer, one consumer per message. Message deleted after consumption. | Task queue (job processing) |
| **Pub/Sub** | One producer, many consumers. Each subscriber gets a copy. | Notifications, event broadcasting |

### Delivery Guarantees

| Guarantee | Description | Trade-off |
|-----------|-------------|-----------|
| **At-most-once** | Fire and forget. Message may be lost. | Fast, no duplicates, possible loss |
| **At-least-once** | Retry until ACK. May duplicate. | Reliable, requires idempotent consumers |
| **Exactly-once** | No loss, no duplicates. | Complex, slower (Kafka supports this) |

---

## 2. Apache Kafka Deep-Dive (Critical for FAANG)

Kafka is a **distributed commit log** for event streaming.

### Core Components

```
Producer → Topic (Partitions) → Consumer Group
```

- **Topic**: A category/feed name for messages (e.g., `user-signups`).
- **Partition**: A topic is split into partitions for parallelism. Each partition is an ordered, immutable log.
- **Offset**: A unique ID for each message within a partition. Consumers track their offset.
- **Broker**: A Kafka server. A cluster has multiple brokers for fault tolerance.
- **Consumer Group**: A set of consumers that share the work. Each partition is consumed by exactly ONE consumer in a group.

### Partition Assignment

```
Topic: "orders" (3 partitions: P0, P1, P2)

Consumer Group A (2 consumers):
  - Consumer A1 → P0, P1
  - Consumer A2 → P2

Consumer Group B (3 consumers):
  - Consumer B1 → P0
  - Consumer B2 → P1
  - Consumer B3 → P2
```

**Rule**: Partitions ≥ Consumers in a group. Extra consumers sit idle.

### Replication & Fault Tolerance

- Each partition has **replicas** across brokers.
- One replica is the **Leader** (handles reads/writes); others are **Followers** (sync data).
- If Leader fails, a Follower is elected.
- **Replication Factor**: Number of copies. RF=3 means 3 copies of each partition.

### Exactly-Once Semantics (EOS)

Kafka achieves exactly-once via:

1. **Idempotent Producers**: Assigns sequence numbers to prevent duplicates.
2. **Transactions**: Atomic writes across multiple partitions.
3. **Consumer Offset Commits**: Commit offset only after processing.

---

## 3. RabbitMQ Basics

RabbitMQ is a traditional **message broker** (AMQP protocol).

### Key Concepts

- **Exchange**: Receives messages and routes them to queues based on rules.
  - **Direct**: Route by exact routing key.
  - **Fanout**: Broadcast to all bound queues (Pub/Sub).
  - **Topic**: Route by pattern matching (e.g., `order.*`).
- **Queue**: Buffer that stores messages until consumed.
- **Binding**: Link between exchange and queue.

### Flow

```
Producer → Exchange → (Binding Rules) → Queue → Consumer
```

---

## 4. Cloud Managed Services Comparison

| Service | Provider | Type | Use Case |
|---------|----------|------|----------|
| **SQS** | AWS | Queue (Point-to-Point) | Decoupling microservices, task queues |
| **SNS** | AWS | Pub/Sub | Notifications, fan-out |
| **Kinesis** | AWS | Stream (like Kafka) | Real-time analytics, log aggregation |
| **Pub/Sub** | GCP | Pub/Sub + Stream | General-purpose messaging |
| **Event Hubs** | Azure | Stream (like Kafka) | Big data pipelines |

### SQS vs Kinesis (Common Interview Question)

| Feature | SQS | Kinesis |
|---------|-----|---------|
| Message Retention | 14 days max | 7 days (extendable to 365) |
| Ordering | FIFO queues only | Per-shard ordering |
| Replay | No (message deleted after consumption) | Yes (re-read from any offset) |
| Throughput | Auto-scales | Provision shards manually |
| Use Case | Task queue, decoupling | Log aggregation, analytics |

---

## 5. When to Use What

| Scenario | Technology |
|----------|------------|
| Simple task queue (send email) | SQS, RabbitMQ |
| Real-time event streaming | Kafka, Kinesis |
| Fan-out notifications | SNS, RabbitMQ (Fanout) |
| Log aggregation | Kafka, Kinesis |
| Need message replay | Kafka, Kinesis |
| Exactly-once processing | Kafka (with EOS) |

---

## 6. Interview Patterns

### Dead Letter Queue (DLQ)

Messages that fail processing N times are moved to a DLQ for manual inspection.

### Backpressure

When consumers can't keep up:

- **Drop messages** (lossy systems)
- **Buffer** (risk OOM)
- **Slow down producer** (rate limiting)

### Ordering Guarantees

- Kafka: Ordered within a partition only.
- Strategy: Use a consistent key (e.g., `user_id`) to route related messages to same partition.
