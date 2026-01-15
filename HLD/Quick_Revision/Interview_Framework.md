# System Design Interview Framework

A structured approach to tackle any system design question in 45-60 minutes. Follow this framework to ensure comprehensive coverage and clear communication.

## Overview: The 6-Step Framework

| Step | Time | Focus |
|------|------|-------|
| 1. Requirements | 5 min | Clarify scope and constraints |
| 2. Estimation | 5 min | Back-of-envelope calculations |
| 3. High-Level Design | 10 min | Core components and data flow |
| 4. API Design | 5 min | Key interfaces |
| 5. Deep Dive | 15-20 min | Critical components in detail |
| 6. Wrap-up | 5 min | Trade-offs, bottlenecks, future improvements |

---

## Step 1: Requirements Clarification (5 min)

**Goal**: Define scope, avoid assumptions, show communication skills.

### Functional Requirements

Ask: "What are the core features we need to support?"

**Template questions**:

- "Who are the users?" (Mobile, Web, API clients)
- "What are the primary use cases?"
- "What actions can users perform?"
- "What data needs to be stored/retrieved?"

**Example (Design Twitter)**:

- Post tweets
- Follow users
- View timeline (home and user)
- Like/retweet
- Search tweets

### Non-Functional Requirements

Ask: "What quality attributes matter most?"

| Attribute | Question to Ask |
|-----------|-----------------|
| **Scale** | "How many users? DAU? Peak concurrent?" |
| **Latency** | "What's acceptable response time?" |
| **Availability** | "How critical is uptime? 99.9%? 99.99%?" |
| **Consistency** | "Can we tolerate eventual consistency?" |
| **Durability** | "Can we afford to lose any data?" |

### Scope Boundaries

Clarify what's OUT of scope:

- "Should I design the auth system or assume it exists?"
- "Are we supporting real-time features?"
- "Should I consider multi-region deployment?"

### Pro Tips

✅ **Write requirements down** - Shows organization
✅ **Prioritize** - "I'll focus on X and Y, treating Z as future work"
✅ **Confirm understanding** - "So to summarize, we're building..."

---

## Step 2: Capacity Estimation (5 min)

**Goal**: Demonstrate quantitative thinking, inform design decisions.

### Quick Reference Numbers

| Metric | Shortcut |
|--------|----------|
| Seconds/day | ~100,000 (86,400) |
| 1M requests/day | ~12 QPS |
| 1B requests/day | ~12,000 QPS |
| 1 KB × 1M | 1 GB |
| 1 MB × 1M | 1 TB |

### What to Estimate

1. **Traffic**: QPS (average and peak)
2. **Storage**: Data size × retention
3. **Bandwidth**: QPS × response size

### Example Calculation (Instagram)

```
Given:
- 500M DAU
- 2 photos uploaded/user/day
- Average photo: 2 MB
- Photos stored for 10 years

Upload Traffic:
- 500M × 2 / 86,400 ≈ 12,000 uploads/sec (peak: 5× = 60K)

Storage:
- Daily: 500M × 2 × 2MB = 2 PB/day
- 10 years: 2 PB × 365 × 10 = 7.3 EB (huge! Need compression, CDN)
```

### How to Present

> "Let me do some quick math. If we have 100M DAU with 10 actions per day, that's roughly 12,000 QPS. At peak, maybe 5x, so we're looking at 60K QPS. This tells me we'll need horizontal scaling and caching."

---

## Step 3: High-Level Design (10 min)

**Goal**: Establish the architecture blueprint.

### Start with Data Flow

1. Draw client (mobile/web)
2. Add entry point (API Gateway / Load Balancer)
3. Add core services
4. Add data stores
5. Draw arrows showing data flow

### Core Components to Consider

| Component | When to Include |
|-----------|-----------------|
| **Load Balancer** | Always (multiple servers) |
| **CDN** | Static content, global users |
| **API Gateway** | Auth, rate limiting, routing |
| **Cache** | Read-heavy, expensive computations |
| **Message Queue** | Async processing, decoupling |
| **Database** | Persistent storage |
| **Search Service** | Full-text search needs |
| **Blob Storage** | Images, videos, files |

### Example Diagram (Twitter)

```
┌─────────┐     ┌──────────┐     ┌─────────────┐
│ Clients │────▶│   CDN    │────▶│ Load Balancer│
└─────────┘     └──────────┘     └──────┬──────┘
                                        │
        ┌─────────────────────────────┬─┴─┬─────────────────┐
        ▼                             ▼   ▼                 ▼
  ┌───────────┐               ┌───────────┐         ┌───────────┐
  │Tweet Svc  │               │User Svc   │         │Search Svc │
  └─────┬─────┘               └─────┬─────┘         └─────┬─────┘
        │                           │                     │
        ▼                           ▼                     ▼
  ┌───────────┐               ┌───────────┐         ┌───────────┐
  │Tweet DB   │               │User DB    │         │Elasticsearch│
  └───────────┘               └───────────┘         └───────────┘
        │
        ▼
  ┌───────────┐     ┌───────────┐
  │  Cache    │◀────│Timeline Svc│
  └───────────┘     └───────────┘
```

### Verbalize While Drawing

> "I'll start with clients accessing through a CDN for static content. Requests hit a load balancer, then route to microservices. Let me add the core services: Tweet Service for posting, User Service for profiles, and a Timeline Service for feeds..."

---

## Step 4: API Design (5 min)

**Goal**: Define key interfaces between client and server.

### REST API Template

```http
POST /v1/{resource}          # Create
GET /v1/{resource}/{id}      # Read
PUT /v1/{resource}/{id}      # Update (replace)
PATCH /v1/{resource}/{id}    # Update (partial)
DELETE /v1/{resource}/{id}   # Delete
GET /v1/{resource}           # List (with pagination)
```

### Example APIs (Twitter)

```http
# Post a tweet
POST /v1/tweets
Body: { "content": "...", "media_ids": [...] }
Returns: { "id": "123", "created_at": "..." }

# Get timeline
GET /v1/timeline?cursor=abc&limit=20
Returns: { "tweets": [...], "next_cursor": "xyz" }

# Follow user
POST /v1/users/{user_id}/follow
```

### Key Elements to Mention

- **Pagination**: Cursor-based for real-time data, offset for static
- **Authentication**: Bearer token, API key
- **Versioning**: `/v1/` prefix
- **Error handling**: Consistent error format

---

## Step 5: Deep Dive (15-20 min)

**Goal**: Demonstrate depth in 2-3 critical components.

### What to Deep Dive

The interviewer often guides this. If not, choose based on:

1. Most complex component
2. Core differentiator of the system
3. Scalability bottleneck

### Deep Dive Structure

For each component:

1. **Data model**: Schema, relationships
2. **Algorithm/approach**: How it works
3. **Scaling strategy**: Sharding, replication
4. **Trade-offs**: Why this approach vs alternatives

### Example: Timeline Generation (Twitter)

**Problem**: How to generate a user's home timeline efficiently?

**Approach 1: Pull Model (Fan-out on Read)**

```
When user opens app:
1. Get list of followed users
2. Fetch recent tweets from each
3. Merge and sort
4. Return top N
```

- ✅ Simple
- ❌ Slow for users following many people

**Approach 2: Push Model (Fan-out on Write)**

```
When user posts tweet:
1. Get list of followers
2. Write tweet to each follower's timeline cache
```

- ✅ Fast reads
- ❌ Expensive for celebrities (millions of followers)

**Hybrid Approach** (What Twitter does):

- Push for regular users
- Pull for celebrities (merge at read time)
- Pre-compute timelines for active users only

---

## Step 6: Wrap-up (5 min)

**Goal**: Show maturity by discussing failures, trade-offs, and evolution.

### Topics to Cover

#### Bottlenecks and Scaling

> "The main bottleneck is the timeline service. We'd scale horizontally with consistent hashing and add more cache replicas."

#### Failure Handling

> "If Redis fails, we fall back to database reads with circuit breakers. We'd also implement health checks and automatic failover."

#### Trade-offs Made

| Decision | Trade-off |
|----------|-----------|
| Push model for timeline | Fast reads, expensive writes |
| Eventual consistency | Better availability, stale data possible |
| NoSQL for tweets | Scalable, but no joins |

#### Future Improvements

- Multi-region for global users
- ML-based ranking
- Real-time notifications

---

## Common Pitfalls to Avoid

| Pitfall | How to Avoid |
|---------|--------------|
| Jumping to solution | Spend first 5 min on requirements |
| Over-engineering | Start simple, add complexity as needed |
| Ignoring scale | Always do capacity estimation |
| Monolithic thinking | Design for horizontal scaling |
| Forgetting failures | Discuss what happens when X fails |
| Silent coding | Verbalize your thinking |

---

## Quick Checklist Before Interview

- [ ] Know latency numbers (L1 cache to network round trip)
- [ ] Practice back-of-envelope math
- [ ] Know trade-offs of SQL vs NoSQL
- [ ] Understand CAP theorem implications
- [ ] Prepare 2-3 systems you've designed before
- [ ] Have examples of scaling challenges you've solved

---

## Sample Prompts by Step

| Step | What to Say |
|------|-------------|
| Start | "Before I start designing, let me clarify the requirements..." |
| Estimation | "Let me do some quick math to understand the scale..." |
| HLD | "Let me draw out the high-level architecture..." |
| Deep Dive | "Let me dive deeper into the [X] component..." |
| Trade-offs | "The trade-off here is between [A] and [B]..." |
| Wrap-up | "To summarize, the key design decisions are..." |
