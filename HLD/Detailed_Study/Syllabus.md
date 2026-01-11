# High Level Design (HLD) Study Syllabus

This syllabus outlines the structure of the HLD Detailed Study materials, categorized into logical modules. Each module covers the core concepts and key technologies necessary for mastering System Design at FAANG-level interviews.

## Module 1: Foundations of Distributed Systems

**Overview**: Fundamental principles governing large-scale systems. Understanding trade-offs and estimation skills.

* **Core Concepts**:
  * Functional vs Non-functional Requirements
  * Vertical vs Horizontal Scaling
  * CAP Theorem (Consistency, Availability, Partition Tolerance)
  * PACELC Theorem
  * Strong vs Eventual Consistency
  * Latency vs Throughput
  * **Back-of-Envelope Estimation** (QPS, Storage, Bandwidth)
* **Key Technologies**: *Theoretical Concepts*
* **Relevant Files**:
  * `00_Introduction_and_Topics.md`
  * `00_Back_of_Envelope_Estimation.md` ⭐ *NEW*
  * `01_System_Design_Basics.md`
  * `01_Scalability_Reliability_Availability.md`
  * `01_CAP_Theorem_and_Consistency.md`
  * `01_Latency_vs_Throughput.md`

---

## Module 2: Communication Protocols & API Design

**Overview**: Mechanisms for different components of a system to exchange data efficiently and reliably.

* **Core Concepts**:
  * RESTful Principles & Resources
  * RPC (Remote Procedure Call)
  * GraphQL (Query Language)
  * Message Formats (JSON vs Binary/Protobuf)
  * API Versioning & Backward Compatibility
  * Idempotency
  * **Real-time Communication** (WebSockets, SSE, Long Polling)
* **Key Technologies**:
  * HTTP/1.1 vs HTTP/2
  * gRPC (Google Remote Procedure Call)
  * GraphQL
  * REST
  * Swagger / OpenAPI
* **Relevant Files**:
  * `01_API_Design_REST_GRPC.md`
  * `01_Realtime_Communication_WebSockets_SSE.md`
  * `07_Misc_Topics.md` (Versioning)

---

## Module 3: Core Building Blocks & Patterns

**Overview**: The essential components and architectural patterns used to construct scalable architectures.

* **Core Concepts**:
  * Load Balancing Algorithms (Round Robin, Least Conn, Consistent Hashing, **L4 vs L7**)
  * Consistent Hashing (Virtual Nodes)
  * Distributed ID Generation (UUID, Snowflake, Multi-master)
  * Caching Strategies (Read-through, Write-through, Write-back, **Cache Invalidation**)
  * Stateless vs Stateful Architectures
  * **Message Queues & Event Streaming** (Kafka, RabbitMQ, SQS)
  * **Microservices Patterns** (API Gateway, Service Discovery, Service Mesh)
* **Key Technologies**:
  * **Load Balancers**: Nginx, HAProxy, AWS ELB/ALB/NLB
  * **Caching**: Redis, Memcached, CDNs (Cloudflare, AWS CloudFront)
  * **Coordination**: ZooKeeper, etcd
  * **Message Queues**: Apache Kafka, RabbitMQ, AWS SQS/SNS/Kinesis
* **Relevant Files**:
  * `02_Core_Components_Intro.md`
  * `02_Consistent_Hashing.md`
  * `02_Distributed_ID_Generators.md`
  * `02_Message_Queues_Event_Streaming.md` ⭐ *NEW*
  * `04_Scalability_Patterns_Intro.md` ⭐ *EXPANDED*

---

## Module 4: Data Storage & Management

**Overview**: Strategies for storing data at scale, handling replication, sharding, and ensuring performance.

* **Core Concepts**:
  * Relational (ACID) vs Non-Relational (BASE)
  * Sharding (Data Partitioning strategies)
  * Replication (Master-Slave, Multi-Master, Leaderless)
  * Object Storage vs Block Storage
  * Indexing & Query Optimization
  * **B-Trees vs LSM Trees** (Storage Engines)
* **Key Technologies**:
  * **RDBMS**: MySQL, PostgreSQL
  * **NoSQL**: Cassandra (Wide Column), DynamoDB (Key-Value), MongoDB (Document)
  * **Distributed File Systems**: HDFS, S3 (Object Store)
* **Relevant Files**:
  * `03_Storage_Basics.md`
  * `03_Sharding_and_Partitioning.md`

---

## Module 5: Reliability, Security & Operations

**Overview**: Practices to keep the system healthy, secure, and resilient to failure in a production environment.

* **Core Concepts**:
  * Failure Detection & Heartbeats
  * Circuit Breakers & Bulkheads
  * Retry Mechanisms with Exponential Backoff
  * **Consensus Protocols** (Paxos, Raft, Leader Election)
  * **Authentication & Authorization** (OAuth2, JWT, RBAC vs ABAC)
  * **Encryption** (TLS Handshake, At-Rest vs In-Transit)
  * Deployment Strategies (Blue-Green, Canary, Rolling)
  * Observability (Logging, Metrics, Tracing)
* **Key Technologies**:
  * **Consensus**: ZooKeeper, etcd, Consul
  * **Security**: OAuth2, JWT (JSON Web Tokens), TLS/SSL
  * **DevOps**: Docker, Kubernetes (Pods, Services, Deployments), Jenkins
  * **Monitoring**: Prometheus, Grafana, Jaeger, ELK Stack
* **Relevant Files**:
  * `05_Reliability_Patterns.md`
  * `05_Consensus_Protocols.md` ⭐ *NEW*
  * `06_Security_Basics.md` ⭐ *EXPANDED*
  * `07_Cloud_and_DevOps.md` ⭐ *EXPANDED*
  * `07_Deployment_Strategies.md`

---

## Module 6: Applied Design (Case Studies)

**Overview**: Putting concepts together to design specific real-world systems.

* **Core Steps**:
    1. Requirement Clarification (Functional/Non-Functional)
    2. Back-of-the-envelope Estimation (Capacity Planning)
    3. High-Level Design & API Definition
    4. Deep Dive into Data Model & Key Components
* **Case Studies**:
  * **URL Shortener**: Unique ID generation, Redirection, High Read:Write ratio.
  * **Uber/Ride Sharing**: Geo-hashing, Real-time location updates, Matching algorithms.
  * **WhatsApp/Chat**: WebSocket/Long-polling, Message ordering, One-on-one vs Group chat.
  * **Instagram/Feed**: Fan-out strategies (Push vs Pull), Media storage.
  * **YouTube/Netflix**: Video chunking, Adaptive streaming (DASH/HLS), CDN usage.
  * **Typeahead/Autocomplete**: Trie data structures, Top-k problems.
  * **Web Crawler**: Politeness, URL Frontier, Duplicate detection.
  * **Rate Limiter**: Token Bucket, Leaky Bucket algorithms, Distributed counting.
* **Relevant Files**:
  * `09_Case_Studies_Overview.md`
  * `09_Design_1_URL_Shortener.md`
  * `09_Design_2_Uber.md`
  * `09_Design_3_WhatsApp.md`
  * `09_Design_4_Instagram.md`
  * `09_Design_5_Youtube_Netflix.md`
  * `09_Design_6_Typeahead_Autocomplete.md`
  * `09_Design_7_Web_Crawler.md`
  * `09_Design_8_Rate_Limiter_System.md`

---

## Module 7: Interview Prep & Revision

**Overview**: Quick reference materials and practice for interviews.

* **Components**:
  * Key terms and definitions.
  * Common Q&A scenarios.
  * Practice question list.
* **Relevant Files**:
  * `00_Terms_to_Remember.md`
  * `08_Common_Interview_Questions_and_Solutions.md`
  * `10_Practice_Questions_List.md`

---

## Summary of Updates (FAANG Review)

| File | Status | Changes |
|------|--------|---------|
| `00_Back_of_Envelope_Estimation.md` | ⭐ NEW | Latency numbers, QPS/Storage formulas, YouTube example |
| `02_Message_Queues_Event_Streaming.md` | ⭐ NEW | Kafka deep-dive, partitions, consumer groups, exactly-once |
| `05_Consensus_Protocols.md` | ⭐ NEW | Paxos, Raft, leader election, ZooKeeper/etcd |
| `04_Scalability_Patterns_Intro.md` | ⭐ EXPANDED | Load Balancer algorithms, L4/L7, Microservices patterns |
| `06_Security_Basics.md` | ⭐ EXPANDED | OAuth2 flows, JWT structure, RBAC/ABAC, TLS |
| `07_Cloud_and_DevOps.md` | ⭐ EXPANDED | AWS services, K8s concepts, CI/CD, observability |
