# HLD Quick Revision Cheat Sheet

## 1. Important Numbers & Latency (Approximate)

| Operation | Time |
|-----------|------|
| L1 Cache Ref | 0.5 ns |
| L2 Cache Ref | 7 ns |
| Mutex Lock/Unlock | 100 ns |
| Main Memory Ref (RAM) | 100 ns (0.1 µs) |
| Compress 1KB w/ Zippy | 10 µs |
| Send 2KB over 1Gbps Net | 20 µs |
| Read 1MB sequentially from Memory | 250 µs |
| Round trip (same datacenter) | 500 µs |
| Disk Seek | 10 ms |
| Read 1MB sequentially from Net | 10 ms |
| Read 1MB sequentially from Disk | 30 ms |
| Packet roundtrip CA -> Netherlands | 150 ms |

**Rule of Thumb:**

- Memory is fast, Disk is slow.
- Network calls are expensive.
- Random usage of disk is ~100x slower than sequential.

## 2. Power of 2 Table (Capacity Estimation)

| Power | Value | Approx |
|-------|-------|--------|
| $2^{10}$ | 1,024 | 1 KB |
| $2^{20}$ | 1,048,576 | 1 MB |
| $2^{30}$ | 1,073,741,824 | 1 GB |
| $2^{40}$ | 1.099 x $10^{12}$ | 1 TB |
| $2^{50}$ | 1.125 x $10^{15}$ | 1 PB |

## 3. Availability Numbers

| Availability % | Downtime / Year |
|----------------|-----------------|
| 99% (2 9s) | 3.65 days |
| 99.9% (3 9s) | 8.76 hours |
| 99.99% (4 9s) | 52.6 minutes |
| 99.999% (5 9s) | 5.26 minutes |

## 4. Common Concepts Quick Summary

### CAP Theorem

- **Consistency**: All nodes see the same data at the same time.
- **Availability**: Every request gets a response (success/failure).
- **Partition Tolerance**: System continues to work despite message loss/network failure.
- *You can only pick 2 (usually P + C or A).*

### ACID vs BASE

- **ACID** (RDBMS): Atomicity, Consistency, Isolation, Durability.
- **BASE** (NoSQL): Basically Available, Soft state, Eventual consistency.

### Load Balancing Algorithms

- Round Robin / Weighted Round Robin
- Least Connections
- Least Response Time
- IP Hash (Sticky Sessions)
- Consistent Hashing (for Distributed Caches)

### Caching Strategies

- **Cache Aside (Lazy Loading)**: App checks cache -> if miss, read DB -> write to cache.
- **Read Through**: Apps reads cache -> cache reads DB if miss.
- **Write Through**: Write to cache and DB simultaneously (slow write, fast read).
- **Write Back (Behind)**: Write to cache, async write to DB (fast write, risk of data loss).

### Database Sharding Criteria

- **geo-based**: Based on user location.
- **range-based**: e.g., UserID 1-1000.
- **hash-based**: Hash(UserID) % N.
- **consistent hashing**: Ring topology, virtual nodes.

### Communication Protocols

- **HTTP/REST**: Request/Response, stateless, text-based (usually JSON).
- **gRPC**: Protobufs, binary, HTTP/2, high performance, streaming support.
- **WebSocket**: Full-duplex, persistent connection (Chat, Real-time).
- **GraphQL**: Client requests specific data, single endpoint.
- **WebRTC**: Peer-to-peer real-time communication (Video/Voice).

## 5. API Design Best Practices

- Use nouns for resources (`/users`, not `/getUsers`).
- Use HTTP verbs correctly (`GET` read, `POST` create, `PUT` replace, `PATCH` update, `DELETE` remove).
- Versioning (`/v1/users`).
- Pagination (`?limit=10&offset=50` or Cursor-based).
- Use standard HTTP codes.
