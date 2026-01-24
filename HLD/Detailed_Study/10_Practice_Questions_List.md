# Practice Design Questions

List of common system design questions to practice, categorized by their primary technical challenge. Questions are marked with difficulty level: 🟢 (L4), 🟡 (L5), 🔴 (L5+/Staff).

---

## Real-Time & Live Interaction / Updates

*(Focus: Low Latency, WebSockets, Push Models)*

1. **Design WhatsApp** 🟡 (Chat App)
    * *Core Tech*: WebSockets, Erlang/Elixir (optional), NoSQL (Cassandra/HBase).
    * *Key Challenges*: Real-time delivery, Message ordering (Sequence IDs), Last seen/Online presence, E2E Encryption.

2. **Design FB Live Comments** 🟡 (High Throughput Stream)
    * *Core Tech*: WebSockets/SSE, Redis Pub/Sub, Partitioning by StreamID.
    * *Key Challenges*: Massive write throughput (millions/sec), Fan-out to viewers, Temporal ordering, Throttling/Shedding load.

3. **Design Google Docs** 🔴 (Collaborative Editor)
    * *Core Tech*: WebSockets, Operational Transformation (OT) or CRDTs.
    * *Key Challenges*: Conflict resolution, syncing state across users, handling offline edits.

4. **Design Robinhood** 🟡 (Stock Trading / Ticker)
    * *Core Tech*: SSE/WebSockets for prices, Time-series DB.
    * *Key Challenges*: Real-time stock prices (low latency broadcast), ACID compliance for trades, Security.

5. **Design Uber** 🟡 (Ride Sharing)
    * *Core Tech*: WebSockets (Driver location updates), Geo-hashing (S2/Geohash).
    * *Key Challenges*: Real-time matching (Driver <-> Rider), High frequency location updates, Trip state management.

6. **Design Online Auction** 🟡 (e.g., eBay)
    * *Core Tech*: WebSockets, Priority Queues / Redis Sorted Sets.
    * *Key Challenges*: Real-time bidding updates, Timer synchronization, Last-second bids (concurrency handling).

7. **Design Collaborative Spreadsheet** 🔴 (Google Sheets)
    * *Core Tech*: WebSockets, CRDTs, Dependency graphs.
    * *Key Challenges*: Cell dependency resolution, formula propagation, concurrent edits, real-time sync.

---

## Consistency & Transactional

*(Focus: ACID, Locking, Data Integrity)*

1. **Design Ticketmaster** 🔴 (Booking System)
    * *Core Tech*: RDBMS (Postgres/MySQL) with Locking, Distributed Lock (Redis Redlock).
    * *Key Challenges*: Concurrency (prevent double booking), High traffic bursts (Virtual Waiting Queue), Strong Consistency.

2. **Design Payment System** 🔴 (Stripe/PayPal)
    * *Core Tech*: RDBMS, Distributed transactions (Saga pattern), Idempotency keys.
    * *Key Challenges*: ACID compliance, Reconciliation, Fraud detection, Multi-currency, Refunds, Idempotency.

3. **Design Distributed Task Scheduler** 🔴 (Cron at Scale)
    * *Core Tech*: Consistent hashing, Leader election, Distributed locks.
    * *Key Challenges*: Exactly-once execution, Handling worker failures, Job priority, Delayed/recurring jobs.

4. **Design Hotel Channel Manager** 🟡 (Inventory Sync)
    * *Core Tech*: RDBMS (Optimistic Locking), Kafka / RabbitMQ (Fan-out), Third-party API integration.
    * *Key Challenges*: Race conditions (Double booking), Cross-platform consistency, API rate limiting, Fault tolerance (OTA downtime).

---

## Data Intensive & Geo-Spatial

*(Focus: Data Ingestion, Aggregation, Analytics)*

1. **Design Strava** 🟡 (Activity Tracking)
    * *Core Tech*: Time-series DB (InfluxDB), QuadTrees/R-Trees.
    * *Key Challenges*: Storing massive GPS trails, calculating stats (Segment matching), Real-time Leaderboards (Redis Sorted Sets), Feed generation.

2. **Design Google Maps** 🔴 (Navigation)
    * *Core Tech*: Graph databases, Dijkstra/A*, Tile serving, Geo-indexing.
    * *Key Challenges*: Routing algorithms, Real-time traffic updates, Map tile generation and caching, Offline support.

3. **Design Ad Click Aggregation** 🔴 (Real-time Analytics)
    * *Core Tech*: Kafka, Flink/Spark Streaming, OLAP databases.
    * *Key Challenges*: Real-time aggregation at scale, Click fraud detection, Deduplication, Exactly-once counting.

---

## Search & Discovery

*(Focus: Indexing, Ranking, Information Retrieval)*

1. **Design Search Engine** 🔴 (Google Search)
    * *Core Tech*: Inverted index, BM25/TF-IDF, Distributed search, PageRank.
    * *Key Challenges*: Index construction, Ranking algorithm, Query processing, Distributed search architecture.

2. **Design Typeahead / Autocomplete** 🟡
    * *Core Tech*: Trie, Elasticsearch, Redis.
    * *Key Challenges*: Prefix matching at scale, Personalization, Update frequency, Caching popular queries.

---

## Content Delivery & Media

*(Focus: Storage, CDN, Streaming)*

1. **Design YouTube / Netflix** 🟡 (Video Streaming)
    * *Core Tech*: CDN, Adaptive streaming (DASH/HLS), Video transcoding.
    * *Key Challenges*: Video chunking, Multiple resolutions, Global distribution, Copyright detection.

2. **Design Instagram** 🟡 (Photo Sharing)
    * *Core Tech*: S3/Blob storage, CDN, Fan-out for feed.
    * *Key Challenges*: Image resizing/compression, Feed generation (push vs pull), Explore/recommendations.

3. **Design Dropbox / Google Drive** 🔴 (File Sync)
    * *Core Tech*: Chunking, Delta sync, Conflict resolution, Metadata service.
    * *Key Challenges*: File chunking and deduplication, Version history, Real-time sync, Sharing and permissions.

---

## Messaging & Notifications

*(Focus: Delivery Guarantees, Multi-channel)*

1. **Design Email System** 🔴 (Gmail)
    * *Core Tech*: SMTP, Full-text search, Spam detection (ML).
    * *Key Challenges*: Email storage at scale, Threading, Search, Spam/phishing detection, Labels and filters.

2. **Design Notification System** 🟡
    * *Core Tech*: Priority queues, APNS/FCM, Multi-channel routing.
    * *Key Challenges*: Delivery guarantees, User preferences, Throttling, Analytics (open rates).

---

## Infrastructure

*(Focus: Reliability, Distributed Systems Primitives)*

1. **Design Rate Limiter** 🟡
    * *Core Tech*: Token bucket, Redis Lua scripts, Consistent hashing.
    * *Key Challenges*: Distributed counting, Multiple tiers, Fail-open vs fail-closed.

2. **Design Web Crawler** 🟡
    * *Core Tech*: URL frontier, Politeness, Bloom filters, Distributed queues.
    * *Key Challenges*: Duplicate detection, Respecting robots.txt, Prioritization, Distributed crawling.

3. **Design URL Shortener** 🟢 (Bit.ly)
    * *Core Tech*: Base62 encoding, Key-value store, CDN for redirects.
    * *Key Challenges*: Unique ID generation, High read:write ratio, Analytics tracking.

4. **Design Metrics/Monitoring System** 🔴 (Prometheus/Datadog)
    * *Core Tech*: Time-series DB, Downsampling, Alerting rules.
    * *Key Challenges*: High cardinality, Aggregation at ingestion, Alert fatigue, Dashboarding.

5. **Design Distributed File System** 🔴 (HDFS/GFS)
    * *Core Tech*: Master-worker architecture, Chunking, Replication.
    * *Key Challenges*: Metadata management, Consistency, Fault tolerance, Large file handling.

---

## Quick Reference Table

| Question | Category | Difficulty | Key Focus |
|----------|----------|------------|-----------|
| URL Shortener | Infrastructure | 🟢 L4 | Basics, ID generation |
| Rate Limiter | Infrastructure | 🟡 L5 | Distributed counting |
| Typeahead | Search | 🟡 L5 | Trie, caching |
| WhatsApp | Real-time | 🟡 L5 | WebSocket, ordering |
| Instagram | Content | 🟡 L5 | Feed, fan-out |
| YouTube | Content | 🟡 L5 | CDN, streaming |
| Uber | Real-time | 🟡 L5 | Geo-indexing |
| Notification System | Messaging | 🟡 L5 | Multi-channel |
| Web Crawler | Infrastructure | 🟡 L5 | Politeness, dedup |
| Ticketmaster | Transactional | 🔴 L5+ | Concurrency |
| Payment System | Transactional | 🔴 L5+ | ACID, idempotency |
| Google Docs | Real-time | 🔴 L5+ | CRDTs/OT |
| Search Engine | Search | 🔴 L5+ | Inverted index |
| Google Maps | Geo-Spatial | 🔴 L5+ | Routing, tiles |
| Gmail | Messaging | 🔴 L5+ | Search, spam |
| Task Scheduler | Transactional | 🔴 L5+ | Exactly-once |
| Channel Manager | Transactional | 🟡 L5 | Race conditions |
| Metrics System | Infrastructure | 🔴 L5+ | Time-series |
| Distributed FS | Infrastructure | 🔴 L5+ | Replication |
| Ad Aggregation | Analytics | 🔴 L5+ | Real-time, fraud |
| Google Drive | Content | 🔴 L5+ | Sync, conflict |
| Spreadsheet | Real-time | 🔴 L5+ | Dependencies |

---

## Study Order Recommendation

### Week 1-2: Foundations (🟢)

1. URL Shortener
2. Rate Limiter
3. Typeahead

### Week 3-4: Core Patterns (🟡)

1. WhatsApp
2. Instagram Feed
3. YouTube
4. Uber
5. Notification System

### Week 5-6: Advanced (🔴)

1. Ticketmaster
2. Payment System
3. Search Engine
4. Google Docs
5. Task Scheduler
6. Hotel Channel Manager

### Week 7+: Expert (🔴)

1. Google Maps
2. Distributed File System
3. Metrics System
4. Ad Click Aggregation
