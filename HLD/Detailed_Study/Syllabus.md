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
  * `00_Back_of_Envelope_Estimation.md`
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
  * `05_Idempotency_Patterns.md`
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
  * `02_Message_Queues_Event_Streaming.md`
  * `04_Scalability_Patterns_Intro.md`
  * `05_Distributed_Locking.md` ⭐ *NEW*
  * `05_Distributed_Caching.md` ⭐ *NEW*

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
  * `03_Data_Pipelines.md`
  * `03_Event_Sourcing_CQRS.md` ⭐ *NEW*

---

## Module 5: Reliability, Security & Operations

**Overview**: Practices to keep the system healthy, secure, and resilient to failure in a production environment.

* **Core Concepts**:
  * Failure Detection & Heartbeats
  * Circuit Breakers & Bulkheads
  * Retry Mechanisms with Exponential Backoff
  * **Consensus Protocols** (Paxos, Raft, Leader Election)
  * **Distributed Transactions** (2PC, Saga, TCC)
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
  * `05_Consensus_Protocols.md`
  * `05_Distributed_Transactions.md` ⭐ *NEW*
  * `06_Security_Basics.md`
  * `07_Cloud_and_DevOps.md`
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
  * **Search Engine**: Inverted index, BM25 ranking, Distributed search.
  * **Notification System**: Multi-channel delivery, Priority queues, Delivery guarantees.
  * **Hotel Availability Sync**: Race conditions, Optimistic locking, Channel Manager, OTA consistency.
* **Relevant Files**:
  * `09_Case_Studies_Overview.md`
  * `09_Design_1_URL_Shortener.md`
  * `09_Design_2_Uber.md`
  * `09_Design_3_WhatsApp.md`
  * `09_Design_4_Instagram.md`
  * `09_Design_5_Youtube_Netflix.md`
  * `09_Design_6_Typeahead_Autocomplete.md`
  * `09_Design_7_Web_Crawler.md`
  * `09_Design_8_Rate_Limiter_System.md` ⭐ *EXPANDED*
  * `09_Design_9_Search_Engine.md` ⭐ *NEW*
  * `09_Design_10_Notification_System.md` ⭐ *NEW*
  * `09_Design_11_Hotel_Availability_Sync.md` ⭐ *NEW*

---

## Module 7: Interview Prep & Revision

**Overview**: Quick reference materials and practice for interviews.

* **Components**:
  * Key terms and definitions.
  * Common Q&A scenarios.
  * Practice question list with difficulty levels.
  * Interview framework for structured approach.
  * Google-specific preparation tips.
* **Relevant Files (Detailed_Study)**:
  * `00_Terms_to_Remember.md`
  * `10_Common_Interview_Questions_and_Solutions.md`
  * `10_Practice_Questions_List.md` ⭐ *EXPANDED*
* **Relevant Files (Quick_Revision)**:
  * `Cheat_Sheet.md`
  * `Rapid_Fire_QA.md`
  * `Interview_Framework.md` ⭐ *NEW*
  * `Common_Mistakes.md` ⭐ *NEW*
  * `Google_Specific_Tips.md` ⭐ *NEW*

---

## Module 8: Templates & Reusable Frameworks ⭐ *NEW*

**Overview**: Ready-to-use templates for structuring interview answers and ensuring operational completeness.

* **Components**:
  * Interview scoring rubric (L4 vs L5 expectations)
  * Self-assessment checklist
  * SLO definition templates
  * Observability and alerting frameworks
  * Incident response templates
* **Relevant Files (Templates/)**:
  * `Design_Interview_Rubric.md` ⭐ *NEW* - Scoring guide with L4/L5 criteria
  * `SLOs_Observability_Template.md` ⭐ *NEW* - Metrics, alerting, dashboards

---

## Summary of Updates (Google L5 Enhancement)

### New Files Added

| File | Module | Description |
|------|--------|-------------|
| `05_Distributed_Transactions.md` | 5 | 2PC, 3PC, Saga, TCC patterns with trade-offs |
| `08_Data_Pipelines.md` | 4 | Batch vs Stream, Lambda/Kappa, ETL/ELT |
| `08_Idempotency_Patterns.md` | 2 | Idempotency keys, exactly-once semantics |
| `09_Design_9_Search_Engine.md` | 6 | Inverted index, BM25, distributed search |
| `09_Design_10_Notification_System.md` | 6 | Multi-channel, APNS/FCM, delivery guarantees |
| `Interview_Framework.md` | 7 | 6-step interview approach with timing |
| `Common_Mistakes.md` | 7 | 14 common pitfalls and how to avoid |
| `Google_Specific_Tips.md` | 7 | Google culture, L5 expectations, tech stack |
| `09_Design_11_Hotel_Availability_Sync.md` | 6 | Race conditions, Locking, OTA Sync |

### Expanded Files

| File | Changes |
|------|---------|
| `09_Design_1_URL_Shortener.md` | ⭐ **Full Model** - BOTE, SLOs, runbooks, threat model, diagrams, cost |
| `09_Design_2_Uber.md` | ⭐ **Full Model** - BOTE, SLOs, runbooks, threat model, diagrams, cost |
| `09_Design_3_WhatsApp.md` | ⭐ **Full Model** - BOTE, E2EE, runbooks, threat model, diagrams, cost |
| `09_Design_4_Instagram.md` | ⭐ **Full Model** - BOTE, hybrid fan-out, threat model, diagrams, cost |
| `09_Design_5_Youtube_Netflix.md` | ⭐ **Full Model** - BOTE, DRM, CDN, threat model, diagrams, cost |
| `09_Design_6_Typeahead_Autocomplete.md` | ⭐ **Full Model** - BOTE, trie, trending, threat model, diagrams, cost |
| `09_Design_7_Web_Crawler.md` | ⭐ **Full Model** - BOTE, frontier, politeness, threat model, diagrams, cost |
| `09_Design_8_Rate_Limiter_System.md` | All 5 algorithms, Redis Lua scripts, multi-tier limiting, headers |
| `09_Design_9_Search_Engine.md` | ⭐ **Full Model** - inverted index, BM25, threat model, diagrams, cost |
| `09_Design_10_Notification_System.md` | ⭐ **Full Model** - multi-channel, priority, threat model, diagrams, cost |
| `10_Practice_Questions_List.md` | 21 questions with difficulty levels, study order |
| `11_Mock_Interview_QA_Bank.md` | ⭐ **NEW** - 50+ challenging follow-ups with model answers for all designs |

### Previous Updates (Still Relevant)

| File | Status | Changes |
|------|--------|---------|
| `00_Back_of_Envelope_Estimation.md` | ⭐ | Latency numbers, QPS/Storage formulas, YouTube example |
| `02_Message_Queues_Event_Streaming.md` | ⭐ | Kafka deep-dive, partitions, consumer groups, exactly-once |
| `05_Consensus_Protocols.md` | ⭐ | Paxos, Raft, leader election, ZooKeeper/etcd |
| `04_Scalability_Patterns_Intro.md` | ⭐ | Load Balancer algorithms, L4/L7, Microservices patterns |
| `06_Security_Basics.md` | ⭐ | OAuth2 flows, JWT structure, RBAC/ABAC, TLS |
| `07_Cloud_and_DevOps.md` | ⭐ | AWS services, K8s concepts, CI/CD, observability |

---

## Study Path for Google L5

### Phase 1: Foundations (Week 1-2)

* Module 1: All files

* Module 2: API Design, Idempotency
* Quick Revision: Cheat Sheet, Interview Framework

### Phase 2: Core Concepts (Week 3-4)

* Module 3: All files

* Module 4: All files
* Case Studies: URL Shortener, Rate Limiter, Typeahead

### Phase 3: Advanced Topics (Week 5-6)

* Module 5: All files (especially Distributed Transactions, Consensus)

* Case Studies: Uber, WhatsApp, Instagram, YouTube

### Phase 4: Interview Ready (Week 7+)

* Case Studies: Search Engine, Notification System

* Quick Revision: All files
* Practice: Mock interviews with framework
