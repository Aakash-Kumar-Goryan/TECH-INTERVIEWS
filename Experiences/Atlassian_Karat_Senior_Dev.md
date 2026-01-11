# Atlassian Karat Senior Developer Interview Experiences

**Role**: Senior Developer
**Platform**: Karat
**Summary**: This document aggregates multiple candidate experiences for the Atlassian Senior Developer role on Karat.

---

## Experience 1

**Result**: Cleared with positive feedback
**Structure**: 5 design-related questions + 2 data structure/coding questions.

### Questions & Best Answers

#### 1. Music Streaming Service - Distributed Servers

**Question**: A music streaming service has the songs distributed across servers. What are the potential problems?

**Optimal Response**:
Focus on the challenges of distributed data systems (CAP theorem, Partitioning, Replication).

* **Data Consistency vs Efficiency**: Ensuring all users see the same playlist/song state. Eventual consistency is usually acceptable for playlists, but strong consistency might be needed for billing.
* **Hot Partitions (Thundering Herd)**: If a new song releases (e.g., Taylor Swift), all traffic might hit one shard. **Solution**: Consistent Hashing with virtual nodes or proactive caching via CDNs.
* **Replication & Availability**: Servers fail. Data must be replicated (Master-Slave or Peer-to-Peer). Problems include replication lag and split-brain scenarios.
* **Latency**: Users are global. Centralized servers cause latency. **Solution**: Edge computing/CDNs to serve media files close to the user.

#### 2. Mobile Puzzle App - Media Content Storage

**Question**: A mobile application for playing puzzles has some media content (audio, video, images). What are the trade-offs for fetching these media online vs storing them offline in the app?

**Optimal Response**:
Compare "Bundled" (Offline) vs "On-Demand" (Online) vs "Hybrid".

* **Offline / Bundled**:
  * *Pros*: Zero latency, works without internet, no server costs.
  * *Cons*: Huge App size, difficult to update content, "stale" content.
* **Online / On-Demand**:
  * *Pros*: Small initial app size, instant content updates, analytics.
  * *Cons*: High latency, requires user data/bandwidth, higher server costs.
* **Hybrid (Best Practice)**:
  * Bundle "Core" assets.
  * Lazy load non-essential levels/media.
  * **Caching Strategy**: Use an LRU cache. Download once, store locally.

#### 3. Huge XML File Processing

**Question**: A huge XML file with sales data needs to be processed. It is huge enough that it cannot be loaded at once given the RAM limitation of the local system. How can we process it?

**Optimal Response**:
The standard solution is **Streaming Parsing**.

1. **SAX (Simple API for XML) or StAX (Streaming API for XML)**: Event-driven parsing. Reads line-by-line. Memory usage is O(1).
2. **Producer-Consumer Pattern**: Reader pushes items to a queue; Workers pull and process.
3. **Distributed Processing**: Split file into byte chunks (if possible) and process via MapReduce.

#### 4. ML Service Scaling for Sports News

**Question**: An ML based service exists for sports news app. What are the things to keep in mind when evaluating the scaling needs for the service for the next one year?

**Optimal Response**:

* **Infrastructure**: specialized hardware (TPUs/GPUs), Autoscaling based on queue depth.
* **Model Drift**: Data Drift (user preferences change) & Concept Drift (news relevance changes). Needs re-training pipelines.
* **Operational Metrics**: Inference Latency SLAs, Throughput (RPS).

#### 5. Social Media App International Expansion

**Question**: A social media app is expanding from US to international regions. What are the things to keep in mind?

**Optimal Response**:

1. **Compliance**: GDPR (Europe), data residency.
2. **Performance**: CDNs for static assets, Multi-region deployments.
3. **Localization**: Translation, RTL support, Date/Time formatting.

#### 6. Coding: Root Nodes in Adjacency Matrix

**Question**: Given an adjacency matrix, find the different distinct root nodes (nodes with 0 In-Degree).
**Algorithm**: Scan columns. If column `j` has all zeros, node `j` has no incoming edges.

---

## Experience 2

**Format**: 1st Round (Screening - Karat) - System Design Core Questions

### Questions & Best Answers

#### 1. Consistent Hashing Concerns (Music Service)

**Question**: Any concerns with Consistent Hashing for multiple servers serving music uploading/streaming service?

**Optimal Response**:

* **Uneven Data Distribution**: Without **Virtual Nodes**, data is rarely distributed evenly. Some nodes will hold much more data than others.
* **Hotspots**: Popular data items (hot keys) heavily requested can overload a single node even if data storage is balanced. Consistent hashing solves data placement, not necessarily request load balancing.
* **Rebalancing Costs**: When a node joins/leaves, keys must be moved. If not managed carefully (e.g., aggressive concurrency), this migration can saturate network bandwidth.

#### 2. Large File Handling (Single Machine Limits)

**Question**: How would you handle large files that cannot fit in on a single machine?

**Optimal Response**:

* **Chunking**: Split the file into fixed-size blocks (e.g., 64MB or 128MB).
* **Distributed Storage**: Store these blocks across multiple machines (like GFS/HDFS/S3).
* **Metadata Management**: Maintain a separate small database (Metadata Server) that maps `FileName -> [Block1_ID, Block2_ID, ...]`.
* **Streaming**: For processing/serving, stream the data chunk-by-chunk so the whole file is never loaded into RAM.

#### 3. Backend Scaling (International)

**Question**: How would you scale your backend if your app is going from a single country to international app?

**Optimal Response**:

* **Geo-Routing (DNS)**: Use Latency-Based Routing (e.g., AWS Route53) to direct users to the nearest data center.
* **Database Strategy**:
  * **Read Replicas**: Place read replicas in each region for fast reads.
  * **Write Strategy**: Either single-master (writes go to US, slower) or Multi-Master/Active-Active (complex conflict resolution, e.g., DynamoDB Global Tables).
* **Stateless Services**: Ensure backend services are stateless so they can be spun up in any region easily.

#### 4. Game Hints: Preload vs Server

**Question**: Pros and cons of preloading hints or loading them from server for a game. Hints could be text/image/video based.

**Optimal Response**:

* **Preloading (Bundled)**:
  * **Pros**: Instant availability (zero latency), works offline, no server load for hints.
  * **Cons**: Increases App Download Size (critical for user acquisition), "Spoiler" risk (users digging into assets), Content is static (can't fix typos or add new hints without app update).
* **Server (On-Demand)**:
  * **Pros**: Keeps app small, Dynamic content (update hints on the fly based on user difficulty), Analytics (track which levels are hard).
  * **Cons**: Latency (spinner while waiting for hint), Data usage cost, Server costs.
* **Recommendation**:
  * **Text/Images**: Preload (small size, high value).
  * **Video**: Stream on demand (too large to bundle). Or use **Smart Prefetching** (download the hint for Level 5 while the user is playing Level 4).
