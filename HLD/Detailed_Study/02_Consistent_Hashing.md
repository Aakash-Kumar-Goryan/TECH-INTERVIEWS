# Consistent Hashing

## 1. The Problem: Simple Hashing

In a distributed cache system with $N$ servers, a common way to balance load is:
`server_index = hash(key) % N`

**Scenario**: You have 4 servers. `hash(key) % 4`.

- If Server 1 goes down, you now have 3 servers.
- The formula becomes `hash(key) % 3`.
- **Result**: Almost ALL keys get remapped to different servers.
- **Impact**: Cache stampede. The DB is overwhelmed because the cache is effectively flushed.

## 2. Solution: Consistent Hashing

**Goal**: When a node is added/removed, only $\frac{1}{N}$ keys (on average) should move.

### How it works (The Ring)

1. **Map the Range**: Imagine a circle (ring) where the hash space is $0$ to $2^{32}-1$.
2. **Place Servers**: Hash the server IPs/Names to place them on the ring.
3. **Place Keys**: Hash the data keys to place them on the ring.
4. **Lookup**: To find the server for a key, go clockwise on the ring from the key's position until you hit a server.

### Adding/Removing Nodes

- **Add Server**: Only keys that were previously mapping to the "next" server (clockwise) and now fall before the new server are moved.
- **Remove Server**: Keys that mapped to the removed server now map to the next server clockwise.

## 3. The Problem of Non-Uniform Distribution

In reality, hashing servers might place them unevenly on the ring (e.g., all 3 servers clumped in one quadrant). This leads to one server taking 90% load.

### Solution: Virtual Nodes (VNodes)

- Instead of mapping a server to 1 point, map it to **K points** (e.g., 100 virtual nodes).
- `Server A` exists at `Hash(A_1)`, `Hash(A_2)`, ... `Hash(A_100)`.
- This ensures statistically uniform distribution of the range even with few physical servers.

## 4. Use Cases

- **DynamoDB / Cassandra**: For data partitioning.
- **Memcached Clients**: To distribute keys among cache servers.
- **Load Balancers**: To stick users to specific backend servers.
