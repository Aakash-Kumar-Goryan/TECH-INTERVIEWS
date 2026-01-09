# Design WhatsApp (Chat System)

## 1. Requirements

### Functional

1. **1-on-1 Chat**: Send text, images, videos.
2. **Group Chat**: Messages delivered to all members.
3. **Status**: Sent, Delivered, Read receipts.
4. **Last Seen**: Online status.

### Non-Functional

1. **Real-time**: Low latency delivery.
2. **Consistency**: Messages ordered correctly.
3. **Availability**: High.
4. **Privacy**: End-to-End Encryption (E2EE) - Server cannot read messages.

## 2. Capacity Estimation

- 2 Billion Users. 100 Billion Messages/Day.
- **QPS**: 100B / 86400 $\approx$ 1.2M msgs/sec.
- **Throughput**: If 10% media (100KB), huge bandwidth requirement.

## 3. High-Level Architecture

### Protocol Selection

- **HTTP/REST**: Too slow (opening new connection for every message).
- **Polling/Long Polling**: Better, but server load high.
- **WebSockets (Preferred)**: Persistent bi-directional connection.
- **XMPP**: Extensible Messaging and Presence Protocol (used by WhatsApp initially).

### System Components

1. **Chat Server**: Maintains WebSocket connections. Distributed.
2. **User Service**: User profile data.
3. **Group Service**: Manages group metadata (members info).
4. **Message Store**: Stores messages (temporarily or permanently depending on design).

### Architecture Diagram

```mermaid
graph TD
    UserA[User A] <-->|WebSocket| Gateway[Chat Gateway]
    UserB[User B] <-->|WebSocket| Gateway
    
    Gateway <-->|Check Status| Session[Session Service \n (Redis)]
    Gateway -->|Persist Msg| MsgSvc[Message Service]
    
    MsgSvc -->|Write| DB[(Cassandra DB)]
    
    subgraph "Offline Handling"
    MsgSvc -- User Offline --> Queue[Offline Queue]
    Queue -->|Retry later| Gateway
    end
    
    Gateway -->|Get Participants| GroupSvc[Group Service]
```

## 4. Key Design Decisions

### A. Message Flow (1-on-1)

1. User A sends msg to Chat Server.
2. Server finds which machine User B is connected to (using a Session Service/Redis).
3. If User B is **Online**: Push msg via WebSocket.
4. If User B is **Offline**: Store in "Unread Messages" DB (MOM - Message Oriented Middleware). Deliver when B connects.

### B. Group Chat

- **Fan-out**: User A sends to Group G (500 members).
- Server looks up 500 members.
- For each member: Check online status and dispatch/queue.
- **Optimization**: For huge groups, limits exist. Don't use a single "Group Server" instance (bottleneck). Partition groups by GroupID.

### C. Database Choice

- Need extremely high write throughput.
- **NoSQL (Wide-Column)**: HBase or Cassandra.
- **Cassandra (Discord/Meta use this)**: Good for write-heavy w/ time-series data.
- **Schema**:
  - `Partition Key`: `ChatID` (allows efficient retrieval of chat history).
  - `Clustering Key`: `MessageID` (TimeUUID) (keeps messages sorted by time).

### D. Last Seen / Presence

- Heartbeat mechanism. Client sends "I'm alive" every 5 sec.
- Server updates Redis `User:LastSeen:Timestamp`.
- If no heartbeat for 30 sec, mark offline.
- **Optimization**: Don't update DB on every heartbeat. Only update if state changes (Online->Offline).

## 5. End-to-End Encryption (E2EE)

- **Signal Protocol**.
- Keys generated on client side (Private/Public pairs).
- Server only relays encrypted blobs. Server has NO generic "master key".

## 6. Bottlenecks

- **Connection Handling**: Maintaining millions of TCP connections (Epimeral port limits).
  - Solution: Load Balancers (L7) + specialized kernel tuning for high connection counts (C10M problem).
