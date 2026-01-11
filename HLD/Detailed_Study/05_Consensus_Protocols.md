# Consensus Protocols: Paxos & Raft

Distributed consensus is how multiple nodes agree on a single value despite failures. Critical for **leader election, configuration management, and distributed locks**.

## 1. The Problem: Distributed Agreement

In a distributed system:

- Nodes can crash.
- Network can partition.
- Messages can be delayed or lost.

**Goal**: All non-faulty nodes must agree on the same value, even if some nodes fail.

**Examples**:

- Which node is the database leader?
- What is the current cluster configuration?
- Has this transaction been committed?

---

## 2. Paxos (Theoretical Foundation)

Paxos is the foundational consensus algorithm (by Leslie Lamport). Complex to understand, but the basis for many systems.

### Roles

- **Proposer**: Proposes a value.
- **Acceptor**: Votes on proposals.
- **Learner**: Learns the agreed value.

### Two Phases

**Phase 1: Prepare**

1. Proposer sends `Prepare(n)` with a unique proposal number `n`.
2. Acceptors respond with a promise to not accept proposals < `n`, plus any previously accepted value.

**Phase 2: Accept**

1. If majority promised, Proposer sends `Accept(n, value)`.
2. Acceptors accept if they haven't promised a higher number.
3. Once majority accepts, value is chosen.

### Why It's Hard

- Multiple proposers can conflict (livelock).
- Requires careful handling of edge cases.
- Production systems use **Multi-Paxos** (leader-based optimization).

---

## 3. Raft (Designed for Understandability)

Raft achieves the same goal as Paxos but is designed to be easier to understand. Used in **etcd, Consul, CockroachDB**.

### Node States

```
Follower ──(timeout)──→ Candidate ──(wins election)──→ Leader
    ↑                       │
    └───(discovers leader)──┘
```

- **Follower**: Passive, responds to Leader/Candidate.
- **Candidate**: Requests votes during election.
- **Leader**: Handles all client requests, replicates logs.

### Leader Election

1. If Follower doesn't hear from Leader (heartbeat timeout), becomes Candidate.
2. Candidate increments **term**, votes for itself, requests votes from others.
3. Node votes for first Candidate in a term (first-come-first-served).
4. Candidate with **majority votes** becomes Leader.
5. Leader sends heartbeats to maintain authority.

### Log Replication

1. Client sends request to Leader.
2. Leader appends to local log, sends `AppendEntries` RPC to Followers.
3. Once **majority** acknowledges, entry is **committed**.
4. Leader notifies Followers to commit.

### Safety Properties

- **Election Safety**: At most one Leader per term.
- **Leader Append-Only**: Leader never overwrites its log.
- **Log Matching**: If two logs have same index and term, all prior entries are identical.

---

## 4. Comparison: Paxos vs Raft

| Aspect | Paxos | Raft |
|--------|-------|------|
| **Understandability** | Complex | Designed for simplicity |
| **Leader** | Multi-Paxos adds leader | Built-in leader concept |
| **Production Use** | Google Chubby, Spanner | etcd, Consul, TiKV |
| **Log Replication** | Separate protocol | Integrated |

---

## 5. Real-World Systems Using Consensus

| System | Protocol | Use Case |
|--------|----------|----------|
| **ZooKeeper** | ZAB (Paxos-like) | Distributed coordination, config, locks |
| **etcd** | Raft | Kubernetes control plane, service discovery |
| **Consul** | Raft | Service mesh, health checking |
| **CockroachDB** | Raft | Distributed SQL database |
| **Google Spanner** | Paxos | Global distributed database |

---

## 6. Interview Talking Points

### When Do You Need Consensus?

- **Leader Election**: Who is the primary database node?
- **Distributed Locks**: Only one service instance processes a job.
- **Configuration Management**: All nodes agree on current config.
- **Atomic Broadcast**: Total ordering of messages (Kafka uses ZooKeeper for this).

### CAP Theorem Connection

Consensus protocols typically sacrifice **Availability** during network partitions to guarantee **Consistency** (CP systems).

### Split-Brain Problem

Without consensus, partitioned nodes may both think they are leader. Consensus ensures only one leader by requiring majority quorum (> N/2 nodes).
