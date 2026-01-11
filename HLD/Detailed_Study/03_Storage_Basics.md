# Storage & Data Management

## 1. File Storage vs Block Storage vs Object Storage

| Feature | **Block Storage** (SSD/HDD) | **File Storage** (NAS/NFS) | **Object Storage** (S3/GCS) |
| :--- | :--- | :--- | :--- |
| **Data Unit** | Fixed-size blocks | Files in folders | Objects (Data + Metadata) |
| **Access** | Low-level OS filesystem | Network protocols (NFS/SMB) | REST API (HTTP) |
| **Performance** | Highest (Low Latency) | Medium | Lower Latency / High Throughput |
| **Scalability** | Limited (Vertical) | Limited | Infinite (Horizontally Scalable) |
| **Use Case** | DB Hosts, Boot volumes | Shared Office Docs, Legacy Apps | Media (Images/Video), Backups, Data Lakes |

---

## 2. Blob Stores (S3, GCS, Azure Blob)

**Binary Large Object (BLOB)** storage is designed for unstructured data.

- **Key Concepts**:
  - **Buckets**: Logical containers.
  - **Immutability**: Objects are typically immutable. You overwrite, not edit.
  - **Multipart Upload**: Upload large files in chunks parallelly.
- **CDN Integration**: Often sits behind a CDN (CloudFront) to serve static assets globally.
- **Lifecycle Policies**: Auto-move data to cheaper tiers (S3 Standard -> S3 Glacier) after X days.

---

## 3. Data Modeling & Schema Design

### SQL (Relational)

- **Normalization**: Reduce redundancy. Break data into linked tables.
  - *Pro*: Consistency, data integrity, efficient writes.
  - *Con*: Complex JOINs on reads (slow at scale).
- **Denormalization**: Add redundancy to optimize reads.
  - *Pro*: Fast reads (no JOINs).
  - *Con*: Complex application logic to keep data in sync (Write Heavy).

### NoSQL

- **Schema-on-Read**: Flexible structure.
- **Aggregates**: Data meant to be read together is stored together (e.g., User Document contains Addresses field).

---

## 4. Data Consistency Models

1. **Strong Consistency** (Linearizability):
    - Every read returns the most recent write.
    - System acts like a single node.
    - *cost*: High latency (must block until all replicas update).
    - *Example*: Bank Account Balance.
2. **Eventual Consistency**:
    - Reads may return stale data.
    - If no new updates are made, eventually all replicas converge.
    - *cost*: Potential confusion for users.
    - *benefit*: High availability, low latency.
    - *Example*: YouTube View Count, Facebook Likes.
3. **Causal Consistency**:
    - "Event B caused by Event A" must be seen in that order. Unrelated events can be out of order.
    - Middle ground.

---

## 5. Storage Engines: B-Trees vs LSM Trees

(Critical FAANG Topic)

### B-Tree (Relational DBs like MySQL, Postgres)

- Optimized for **Read-Heavy** systems.
- Updates happen in-place on disk pages.
- **Read**: $O(log N)$. Very fast.
- **Write**: Random I/O can be slow. Fragmentation issues.

### Log-Structured Merge (LSM) Tree (Cassandra, RocksDB, BigTable)

- Optimized for **Write-Heavy** systems.
- **Write**: Appends to an in-memory buffer (MemTable) -> Flushed to immutable files on disk (SSTables) sequentially.
- **Read**: Can be slower. Must check MemTable + look in multiple SSTables.
  - *Optimization*: Uses **Bloom Filters** to quickly check if key exists in an SSTable.
