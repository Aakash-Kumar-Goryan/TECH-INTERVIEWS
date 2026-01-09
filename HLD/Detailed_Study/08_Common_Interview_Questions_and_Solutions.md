# Common HLD Interview Questions & Solutions

## 1. How do you handle "Hot Partitions" (the "Celebrity Problem")?

**Scenario**: You shard a database by `User_ID`. Justin Bieber (a celebrity) has millions of followers. All queries/writes for his data go to one shard, overloading it.
**Solutions**:

- **Consistent Hashing**: Helps distribute data, but doesn't solve hot keys.
- **Salting/Virtual Shards**: Append a random number (0-N) to the key for "hot" records. This spreads the data across multiple shards. When reading, query all N shards and aggregate.
- **Dedicated Caching**: Heavily cache the hot data to offload the database.
- **Separate Handling**: If a user is identified as a "VIP" or "Hot", treat them differently (e.g., logic to fan-out content differs for them).

## 2. When to choose SQL vs NoSQL?

**Choose SQL (Relational) when**:

- You need ACID compliance (e.g., Financial transactions).
- You have structured data with strict schema.
- Your data is highly relational (complex JOINs required).
- You need a mature ecosystem and standard query language.
**Choose NoSQL when**:
- you need massive scale (horizontal scaling is easier).
- Data is unstructured or semi-structured (documents, graphs, key-values).
- You need extremely high write throughput.
- Flexible schema usage is preferred (e.g., analyzing logs).

## 3. How do you design a Rate Limiter?

**Algorithms**:

- **Token Bucket**: Tokens are added at a rate. If bucket usually empty, request dropped. Allows bursts.
- **Leaky Bucket**: Requests processed at constant rate. Smooths out bursts.
- **Fixed Window Counter**: Count requests in 1-min windows. Problem: Spike at edges of window (e.g., 59th sec and 1st sec).
- **Sliding Window Log**: Stores timestamps. Accurate but high memory usage.
- **Sliding Window Counter**: Hybrid approach/approximation. Good balance of memory and accuracy.
**Implementation**: Use Redis (INCR, EXPIRE) or Lua scripts for atomicity.

## 4. Push (Fan-out on Write) vs Pull (Fan-out on Read) for News Feed?

*Example: Designing Twitter*
**Push (Fan-out on Write)**:

- *Pro*: Fast reads. User's feed is pre-computed in a dedicated list.
- *Con*: Slow writes. Posting a tweet requires writing to millions of follower lists (Celebrity problem).
**Pull (Fan-out on Read)**:
- *Pro*: Fast writes. Just write to user's DB.
- *Con*: Slow reads. Must query all followings' tweets and merge/sort them at read time.
**Hybrid**:
- Use Push for normal users.
- Use Pull for celebrities (users with >X followers).

## 5. How to handle Distributed Transactions across Microservices?

**Two-Phase Commit (2PC)**:

- Coordinator asks everyone to "prepare". If all say yes, tells everyone to "commit".
- *Problem*: Blocking, single point of failure, slow.
**Saga Pattern (Preferred)**:
- Sequence of local transactions. Each service updates its own DB and publishes an event.
- If a step fails, execute **Compensation Transactions** to undo previous steps.
- Async and non-blocking.

## 6. What is a Bloom Filter and where is it used?

**Definition**: A probabilistic data structure that tells you if an element is *definitely not* in the set or *maybe* in the set.
**Use Cases**:

- Checking if a username is taken (fast "no" check before querying DB).
- Reducing disk lookups in databases (like Cassandra/HBase) by checking if a row exists in an SSTable.
- CDN cache filtering (don't cache "one-hit wonders").

## 7. How to ensure Idempotency in APIs?

**Problem**: Client retries a payment request due to timeout. Design must ensure payment processes only once.
**Solution**:

- Client sends a unique `idempotency_key` (UUID) with the request.
- Server checks if key exists in DB/Cache.
  - If yes: Return the previous successful response.
  - If no: Process payment, store key + response, return result.

## 8. Long Polling vs WebSockets vs Server-Sent Events (SSE)

- **Long Polling**: Client requests -> Server holds until data available -> returns -> Client requests again. High overhead for headers.
- **WebSockets**: Bi-directional, persistent. Good for Chat, Gaming.
- **SSE**: Server-to-Client only. Persistent. Good for Stock tickers, Live scores, Notifications. Simpler than WebSockets (over HTTP).
