# Database Sharding & Partitioning

## 1. Vertical Partitioning (Scaling)

Splitting data by columns.

- **Example**: A table `User(ID, Name, Bio, PhotoBlob)`.
- Split into:
  - `User_Core(ID, Name)` -> Fast DB.
  - `User_Bio(ID, Bio, PhotoBlob)` -> Blob Store / Slower DB.
- **Pros**: Good when some columns are accessed much more frequently than others.
- **Cons**: Joins become application-level logic.

## 2. Horizontal Partitioning (Sharding)

Splitting data by rows.

- **Example**: `Users` table with 100M rows.
- Split into:
  - `Shard_1`: Users 1 to 1M.
  - `Shard_2`: Users 1M to 2M.

## 3. Sharding Strategies

### A. Range Based

- Shard by ranges of key (e.g., UserID 1-1000, 1001-2000).
- **Pros**: Easy to implement. Easy range queries.
- **Cons**: **Uniform distribution is rare**. "Hot spots" if creating IDs sequentially (Shard 100 gets all writes, Shard 1-99 idle).

### B. Hash Based (Key Based)

- `Shard_ID = Hash(Key) % Total_Shards`.
- **Pros**: Uniform distribution.
- **Cons**: Resharding is painful (changing `Total_Shards` moves minimal data if Consistent Hashing used, but still requires data movement). No range queries across shards.

### C. Directory Based (Lookup Service)

- A separate DB tracks `Key -> Shard_ID` mapping.
- **Pros**: Flexible. Can move individual keys.
- **Cons**: Look-up DB is a Single Point of Failure and performance bottleneck. Caching required.

## 4. Problems with Sharding

1. **Joins and Denormalization**: Can't JOIN across shards efficiently. Must aggregate in code or optimize schema (denormalize).
2. **Referential Integrity**: Foreign keys don't work across shards.
3. **Resharding**: Moving data from 10 shards to 20 shards without downtime is extremely complex.

## 5. Famous Implementations

- **Pinterest**: Custom sharding logic on MySQL.
- **Instagram**: ID-based sharding where ID contains the logical shard ID.
- **Vitess**: Database clustering system for horizontal scaling of MySQL (used by YouTube/Slack).
