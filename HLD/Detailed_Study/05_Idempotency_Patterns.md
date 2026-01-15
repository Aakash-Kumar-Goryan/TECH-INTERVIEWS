# Idempotency Patterns

Idempotency ensures that performing the same operation multiple times has the same effect as performing it once. Critical for building reliable distributed systems.

## 1. Why Idempotency Matters

In distributed systems, clients may retry requests due to:

- Network timeouts
- Load balancer retries
- Client-side retry logic
- Message queue redelivery

**Without idempotency**:

```
Client: "Transfer $100 to Bob"
Server: <processes, but response lost>
Client: "Transfer $100 to Bob" (retry)
Server: <processes again>
Result: Bob gets $200 instead of $100 ❌
```

**With idempotency**:

```
Client: "Transfer $100 to Bob" (id=abc123)
Server: <processes, stores id=abc123>
Client: "Transfer $100 to Bob" (id=abc123, retry)
Server: <sees abc123 already processed, returns cached result>
Result: Bob gets $100 ✅
```

---

## 2. Naturally Idempotent Operations

Some operations are inherently safe to retry:

| Operation | Why Idempotent | Example |
|-----------|----------------|---------|
| **GET (read)** | No state change | `GET /users/123` |
| **PUT (replace)** | Sets to specific value | `PUT /users/123 {name: "Bob"}` |
| **DELETE** | Already gone stays gone | `DELETE /users/123` |
| **SET (absolute)** | `balance = 100` | Same value each time |

### Non-Idempotent Operations (Need Protection)

| Operation | Why NOT Idempotent | Example |
|-----------|-------------------|---------|
| **POST (create)** | Creates new resource each time | `POST /orders` |
| **INCREMENT** | Adds each time | `balance += 100` |
| **APPEND** | Adds to list | Add item to cart |

---

## 3. Idempotency Key Pattern

The most common solution for making non-idempotent operations safe.

### How It Works

1. Client generates a unique key (UUID) for each logical operation
2. Client sends key with request
3. Server stores key after processing
4. On retry, server recognizes key and returns cached response

### API Design

```http
POST /v1/payments
Idempotency-Key: a1b2c3d4-e5f6-7890-abcd-ef1234567890
Content-Type: application/json

{
  "amount": 10000,
  "currency": "USD",
  "recipient": "acct_bob"
}
```

### Server Implementation

```python
def process_payment(request):
    idempotency_key = request.headers.get('Idempotency-Key')
    
    # 1. Check if already processed
    cached = redis.get(f"idem:{idempotency_key}")
    if cached:
        return json.loads(cached)  # Return cached response
    
    # 2. Acquire lock to prevent concurrent processing
    lock = acquire_lock(f"lock:{idempotency_key}", timeout=30)
    if not lock:
        raise ConflictError("Request in progress")
    
    try:
        # 3. Double-check after acquiring lock
        cached = redis.get(f"idem:{idempotency_key}")
        if cached:
            return json.loads(cached)
        
        # 4. Process the payment
        result = execute_payment(request.body)
        
        # 5. Store result with TTL
        redis.setex(
            f"idem:{idempotency_key}",
            86400 * 7,  # 7 days TTL
            json.dumps(result)
        )
        
        return result
    finally:
        release_lock(lock)
```

### Storage Options for Idempotency Keys

| Storage | TTL | Pros | Cons |
|---------|-----|------|------|
| **Redis** | Easy TTL | Fast, simple | Not durable, limited space |
| **Database** | Manual cleanup | Durable, queryable | Slower, schema needed |
| **Hybrid** | Short Redis, long DB | Best of both | Complexity |

---

## 4. Database-Level Idempotency

### Unique Constraints

Use database unique constraints to prevent duplicates:

```sql
CREATE TABLE payments (
    id SERIAL PRIMARY KEY,
    idempotency_key VARCHAR(100) UNIQUE,
    amount DECIMAL,
    status VARCHAR(20),
    created_at TIMESTAMP
);

-- Insert with conflict handling (PostgreSQL)
INSERT INTO payments (idempotency_key, amount, status)
VALUES ('abc123', 100.00, 'completed')
ON CONFLICT (idempotency_key) DO NOTHING
RETURNING *;
```

### UPSERT Pattern

```sql
-- If exists, update; if not, insert
INSERT INTO user_settings (user_id, theme, notifications)
VALUES (123, 'dark', true)
ON CONFLICT (user_id) 
DO UPDATE SET theme = EXCLUDED.theme, notifications = EXCLUDED.notifications;
```

### Conditional Writes (Optimistic Locking)

```sql
-- Only update if version matches
UPDATE accounts
SET balance = balance - 100, version = version + 1
WHERE id = 123 AND version = 5;

-- Check rows affected
IF rows_affected == 0:
    RAISE ConflictError("Concurrent modification")
```

---

## 5. Message Queue Idempotency

### The Problem

Message queues deliver **at-least-once**, not exactly-once:

```
Queue → Worker (processes) → ACK lost → Queue redelivers
```

### Solution: Consumer-Side Idempotency

```python
def handle_message(message):
    message_id = message['id']
    
    # Check if already processed
    if redis.sismember('processed_messages', message_id):
        return  # Skip duplicate
    
    try:
        # Process the message
        process(message['body'])
        
        # Mark as processed (with TTL for cleanup)
        redis.sadd('processed_messages', message_id)
        redis.expire('processed_messages', 86400)
    except Exception:
        # Don't mark as processed, allow retry
        raise
```

### Exactly-Once Semantics (Kafka)

Kafka supports exactly-once with:

1. **Idempotent Producer**: Deduplicates at broker level (producer ID + sequence number)
2. **Transactional API**: Atomic writes across partitions

```python
# Kafka idempotent producer
producer = KafkaProducer(
    enable_idempotence=True,  # Enable idempotent producer
    acks='all',
    retries=10
)
```

---

## 6. Designing for Idempotency

### Convert Increments to Absolute Sets

**Non-idempotent**:

```
UPDATE balance SET amount = amount + 100 WHERE user_id = 123;
```

**Idempotent**:

```
-- Calculate new balance in application, then set absolutely
UPDATE balance SET amount = 250 WHERE user_id = 123 AND version = 5;
```

### Use Immutable Event Logs

Instead of updating state, append events:

```sql
-- Non-idempotent: UPDATE account balance
-- Idempotent: INSERT event with unique ID

INSERT INTO account_events (event_id, account_id, type, amount)
VALUES ('evt_abc123', 'acct_1', 'CREDIT', 100)
ON CONFLICT (event_id) DO NOTHING;

-- Balance is computed from events
SELECT SUM(CASE WHEN type='CREDIT' THEN amount ELSE -amount END)
FROM account_events WHERE account_id = 'acct_1';
```

---

## 7. Handling Edge Cases

### What If Request Body Differs on Retry?

Two approaches:

**1. Reject with error**:

```python
cached_body = redis.hget(f"idem:{key}", 'request_body')
if cached_body != current_body:
    raise BadRequest("Idempotency key reused with different request")
```

**2. Ignore new body, return cached response** (Stripe's approach):

- Simpler for clients
- Request body mismatch is usually a client bug

### What About Partial Failures?

If processing partially completes before crash:

```python
def process_order(request, idempotency_key):
    # Start transaction
    with db.transaction():
        # Store idempotency record first (pending)
        db.execute("""
            INSERT INTO idempotency_records (key, status)
            VALUES (?, 'pending')
        """, idempotency_key)
        
        # Do the work
        order = create_order(request)
        charge_payment(order)
        
        # Mark complete
        db.execute("""
            UPDATE idempotency_records 
            SET status = 'complete', response = ?
            WHERE key = ?
        """, json.dumps(order), idempotency_key)
    
    return order
```

On retry:

- If status = 'pending': Previous attempt crashed, can retry
- If status = 'complete': Return cached response

---

## 8. Idempotency in REST APIs

### HTTP Methods and Idempotency

| Method | Idempotent? | Safe? | Notes |
|--------|-------------|-------|-------|
| GET | ✅ | ✅ | Read-only |
| HEAD | ✅ | ✅ | Same as GET |
| OPTIONS | ✅ | ✅ | Metadata only |
| PUT | ✅ | ❌ | Replace resource |
| DELETE | ✅ | ❌ | Remove resource |
| POST | ❌ | ❌ | Creates new resource |
| PATCH | ❌ | ❌ | Partial update |

### Making POST Idempotent

Always require idempotency key for POST:

```http
POST /v1/orders
Idempotency-Key: ord_unique_123
Content-Type: application/json

{"product_id": "abc", "quantity": 2}
```

---

## 9. Real-World Examples

### Stripe

- Requires `Idempotency-Key` header
- Keys valid for 24 hours
- Same key returns same response
- Different request body with same key → error

### AWS

- Many APIs accept `ClientToken` parameter
- Token scoped to specific operation
- Ensures create operations are idempotent

### Google Cloud

- Uses request IDs for operation deduplication
- Stored in operation metadata

---

## 10. Interview Talking Points

### When Asked About Reliability

> "We make all write operations idempotent using idempotency keys. Clients generate UUIDs, we store them in Redis with TTL, and return cached responses on retry."

### Trade-offs to Mention

| Trade-off | Options |
|-----------|---------|
| TTL duration | Short (less storage) vs Long (more protection) |
| Storage choice | Redis (fast) vs DB (durable) |
| Body comparison | Strict (safe) vs Ignore (simple) |

### Key Implementation Details

1. **Lock before processing**: Prevent concurrent processing of same key
2. **Store after commit**: Only store key after transaction succeeds
3. **Return full response**: Cached response should be complete
4. **Clean up expired keys**: Prevent unbounded growth

### Common Follow-ups

1. **What if Redis goes down?** → Fallback to DB lookup or fail-open (allow retry)
2. **How long to keep keys?** → Depends on client retry window (typically 24h-7d)
3. **What about distributed transactions?** → Each participant needs its own idempotency
