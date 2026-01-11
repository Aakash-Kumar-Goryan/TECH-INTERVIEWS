# Practice Design Questions

List of common system design questions to practice, categorized by their primary technical challenge.

## Real-Time & Live Interaction / Updates

*(Focus: Low Latency, WebSockets, Push Models)*

1. **Design WhatsApp** (Chat App)
    * *Core Tech*: WebSockets, Erlang/Elixir (optional), NoSQL (Cassandra/HBase).
    * *Key Challenges*: Real-time delivery, Message ordering (Sequence IDs), Last seen/Online presence, E2E Encryption.
2. **Design FB Live Comments** (High Throughput Stream)
    * *Core Tech*: WebSockets/SSE, Redis Pub/Sub, Partitioning by StreamID.
    * *Key Challenges*: Massive write throughput (millions/sec), Fan-out to viewers, Temporal ordering, Throttling/Shedding load.
3. **Design Google Docs** (Collaborative Editor)
    * *Core Tech*: WebSockets, Operational Transformation (OT) or CRDTs.
    * *Key Challenges*: Conflict resolution, syncing state across users, handling offline edits.
4. **Design Robinhood** (Stock Trading / Ticker)
    * *Core Tech*: SSE/WebSockets for prices, Time-series DB.
    * *Key Challenges*: Real-time stock prices (low latency broadcast), ACID compliance for trades, Security.
5. **Design Uber** (Ride Sharing)
    * *Core Tech*: WebSockets (Driver location updates), Geo-hashing (S2/Geohash).
    * *Key Challenges*: Real-time matching (Driver <-> Rider), High frequency location updates, Trip state management.
6. **Design Online Auction** (e.g., eBay)
    * *Core Tech*: WebSockets, Priority Queues / Redis Sorted Sets.
    * *Key Challenges*: Real-time bidding updates, Timer synchronization, Last-second bids (concurrency handling).

## Consistency & Transactional

*(Focus: ACID, Locking, Data Integrity)*

1. **Design Ticketmaster** (Booking System)
    * *Core Tech*: RDBMS (Postgres/MySQL) with Locking, Distributed Lock (Redis Redlock).
    * *Key Challenges*: Concurrency (prevent double booking), High traffic bursts (Virtual Waiting Queue), Strong Consistency.

## Data Intensive & Geo-Spatial

*(Focus: Data Ingestion, Aggregation, Analytics)*

1. **Design Strava** (Activity Tracking)
    * *Core Tech*: Time-series DB (InfluxDB), QuadTrees/R-Trees.
    * *Key Challenges*: Storing massive GPS trails, calculating stats (Segment matching), Real-time Leaderboards (Redis Sorted Sets), Feed generation.
