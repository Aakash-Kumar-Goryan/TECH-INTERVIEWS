# Rapid Fire HLD & System Design QA (FAANG Level)

*Deep-dive questions focusing on trade-offs, internals, and bottlenecks.*

## 1. Scaling & Bottlenecks

**Q: Vertical Scaling has a "hard limit". What exactly is that limit usually?**

* **A:** It's not just CPU/RAM cost. It's the **interconnect bottlenecks** (Message bus saturation), **OS kernel contention** (thread scheduling overhead), and the fact that specialized hardware yields diminishing returns per dollar compared to commodity hardware.

**Q: When would you CHOOSE Vertical Scaling over Horizontal in a modern system?**

* **A:** Early stage startups (simplifies ops), or for specific **high-consistency, low-latency databases** where partitioning logic (sharding) introduces too much complexity or cross-shard join latency is unacceptable.

**Q: Explain the "Thundering Herd" problem and how to mitigate it.**

* **A:** When many clients/processes wake up simultaneously to compete for a resource (e.g., cache expires).
* **Fix:** **Exponential Backoff + Jitter**, or **Cache Stampede protection** (locking the cache key so only one process computes the value).

## 2. Distributed Consensus & Networking (The Hard Stuff)

**Q: Paxos vs. Raft? Why do we need them?**

* **Purpose:** To agree on a value (Log replication) in a distributed system even if nodes fail. (Leader Election).
* **Paxos:** The original academic standard. Extremely difficult to implement correctly.
* **Raft:** Created to be *understandable*. Uses a Strong Leader model. (Used in etcd, Consul).
* **Key Concept:** **Split Votes** are handled by randomized election timeouts.

**Q: What is the "Two Generals Problem"?**

* **A:** A thought experiment proving that **Certainty is impossible** over an unreliable network. You can never perfectly acknowledge the acknowledgement of an acknowledgement forever.
* **Implication:** We accept "Eventual Consistency" or use heavy protocols like 2PC (which can block) because 100% reliable coordination is impossible.

**Q: L4 vs. L7 Load Balancing. When to use which?**

* **L4 (Transport Layer - TCP/UDP):** dumb packet forwarding based on IP+Port. Very fast, low CPU. No inspection of data. (Good for massive scale ingress).
* **L7 (Application Layer - HTTP):** Smart routing. Can inspect Headers, Cookies, URL path. Can do **SSL Termination**. (Good for Microservices routing).

**Q: What is "Byzantine Fault Tolerance"?**

* **A:** A system that tolerates not just failed nodes (crashes), but *malicious/lying* nodes (traitors).
* **Relevance:** Critical in Blockchains (crypto). In standard data centers, we usually assume nodes crash but don't lie (Fail-Stop model), so we don't usually pay the expensive cost of BFT algorithms.

## 3. Distributed Data & Consistency

**Q: Explain Quorum (R + W > N). Why is it important?**

* **A:** To guarantee strong consistency in a distributed system with replication factor $N$. If $R$ (nodes read) + $W$ (nodes written) > $N$, at least one node in the read set has the latest write.
* **Trade-off:** Higher $W$ or $R$ increases latency.

**Q: How do you handle Distributed Transactions across Microservices? (2PC vs SAGA)**

* **2PC (Two-Phase Commit):** Strong consistency (ACID). Blocking protocol. If coordinator fails, locks are held. Not scalable.
* **SAGA Pattern:** Eventual consistency (BASE). Sequence of local transactions.
  * *Choreography:* Services subscribe to events (decentralized).
  * *Orchestration:* Central coordinator tells services what to do.
  * *Rollback:* Must implement "Compensating Transactions" for every step to undo changes if a later step fails.

**Q: What is the difference between B-Trees and LSM Trees? (Storage Engines)**

* **B-Tree (SQL/Postgres):** Optimized for **Read-Heavy**. In-place updates. Good for range queries. Random writes are expensive (disk seeks).
* **LSM Tree (Log-Structured Merge - Cassandra/RocksDB/Kafka):** Optimized for **Write-Heavy**. Appends to log (sequential write). Reads are slower (check Memtable -> L1 -> L2... Bloom filter helps).

## 3. Advanced Caching & Identity

**Q: Designing a Unique ID Generator (Twitter Snowflake vs UUID). Compare.**

* **UUID:** 128-bit, collision-free, can be generated offline. **Cons:** Large size, not sortable by time, bad for B-Tree indexing (randomness causes page fragmentation).
* **Snowflake (64-bit int):** Time-sortable, smaller, fits in a generic `long`. **Cons:** Requires coordination (Zookeeper for WorkerID) and creates a dependency on a generator service/clock synchronization.

**Q: Consistent Hashing: What happens when a node is added? What is the "Hot Partition" problem?**

* **Mechanism:** keys mapped to a ring ($0-2^{32}$). Node ownership is clockwise. adding node $K$ takes keys only from immediate neighbor.
* **Hot Partition:** If one node (e.g., "Justin Bieber's shard") gets uneven traffic.
* **Fix:** **Virtual Nodes**. Each physical server is mapped to 100s of points on the ring to spread load more evenly.

**Q: Bloom Filters: What are they and what is the trade-off?**

* **A:** Probabilistic data structure to test if an element is present.
* **Guarantee:** returns "Possibly in set" or "Definitely NOT in set".
* **Trade-off:** False positives exist (never false negatives). Cannot remove items easily. Used in LSM trees (to avoid disk lookups) and CDNs.

## 4. Real-world Scenarios

**Q: How does `Redis` achieve high performance despite being single-threaded?**

* **A:** In-memory speed + **I/O Multiplexing (epoll/kqueue)**. It handles network requests asynchronously but executes commands sequentially, avoiding context-switching overhead and lock contention.

**Q: Kafka vs. RabbitMQ? (Push vs Pull)**

* **RabbitMQ (Push-based/Smart Broker):** Good for complex routing, job queues, per-message acknowledgement. Lower throughput.
* **Kafka (Pull-based/Dumb Broker):** Log-based. Consumers track their own offset. High throughput (sequential disk I/O), message replayability, stream processing.

**Q: How do you handle the "Last Room" problem (Race Condition) in a Hotel Booking System?**

* **A:** **Optimistic Locking** at the database level (`UPDATE inventory SET count = count - 1 WHERE count > 0`).
* **Edge Case:** If syncing to OTAs (Airbnb/Booking.com), you accept **overbookings** and handle them via "Compensating Transactions" (refund/relocate) because you cannot lock external systems.

**Q: Explain Isolation Levels (Read Committed vs. Serializable).**

* **Read Committed:** No dirty reads. (Standard).
* **Repeatable Read:** No non-repeatable reads (Row locks).
* **Serializable:** Highest level. No Phantom reads. (Range locks or full table locks). Drastically reduces throughput.
