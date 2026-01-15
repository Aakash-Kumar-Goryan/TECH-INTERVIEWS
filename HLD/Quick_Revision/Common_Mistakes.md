# Common System Design Mistakes

Avoid these pitfalls to maximize your interview performance.

## 1. Jumping to Solution Too Quickly

### The Problem

Immediately drawing boxes without understanding requirements.

### Why It's Bad

- Miss critical requirements
- Design for wrong scale
- Appear to lack structure

### How to Avoid
>
> "Before I start designing, let me make sure I understand the requirements clearly..."

Spend **5 minutes** clarifying:

- Who are the users?
- What are the core features?
- What scale are we targeting?
- What are the top 2-3 non-functional requirements?

---

## 2. Ignoring Non-Functional Requirements

### The Problem

Only focusing on features, ignoring:

- Scalability
- Latency
- Availability
- Consistency

### Why It's Bad

FAANG interviews are about building **at scale**. A design that doesn't scale is incomplete.

### How to Avoid

After functional requirements, explicitly ask:
> "What are the availability requirements? 99.9% or higher?"
> "What latency is acceptable for the main use case?"
> "Can we tolerate eventual consistency here?"

---

## 3. Skipping Capacity Estimation

### The Problem

Not doing back-of-envelope math.

### Why It's Bad

- Can't justify design decisions
- Miss that database needs sharding
- Miss that you need a CDN

### How to Avoid

Always calculate:

1. **QPS** (queries per second)
2. **Storage** requirements
3. **Bandwidth** needs

> "Let me do some quick math. With 100M DAU and 10 requests per user, that's about 12K QPS..."

---

## 4. Over-Engineering

### The Problem

Adding complexity that isn't needed:

- Kubernetes for a prototype
- Microservices for 100 users
- ML ranking for simple lists

### Why It's Bad

- Wastes interview time
- Shows poor judgment
- Harder to explain

### How to Avoid

Start simple, then evolve:
> "I'll start with a simple design and add complexity as we identify bottlenecks..."

| Stage | Approach |
|-------|----------|
| < 1K users | Monolith, single DB |
| 1K-100K | Read replicas, caching |
| 100K-1M | Sharding, async processing |
| > 1M | Microservices, multi-region |

---

## 5. Under-Engineering

### The Problem

Design that clearly won't scale:

- Single server for billions of users
- No caching for read-heavy workloads
- No async processing for expensive operations

### Why It's Bad

- Shows lack of system thinking
- Will be challenged immediately
- Miss opportunity to show depth

### How to Avoid

After initial design, ask yourself:
> "What happens when traffic 10x? 100x?"

---

## 6. Ignoring Failure Scenarios

### The Problem

Not discussing what happens when things fail:

- Database goes down
- Cache miss storm
- Network partition

### Why It's Bad

- Real systems fail
- Senior engineers plan for failure
- Miss reliability discussion

### How to Avoid

Proactively mention:
> "Let me discuss what happens if the cache fails..."
> "We'd need circuit breakers here to prevent cascading failures..."

---

## 7. Monolithic Database Thinking

### The Problem

Assuming a single database can handle everything.

### Why It's Bad

- Single point of failure
- Can't scale writes
- Query performance degrades

### How to Avoid

| Pattern | When |
|---------|------|
| **Read replicas** | Read-heavy, can tolerate lag |
| **Sharding** | Large dataset, high write volume |
| **Polyglot** | Different data needs different stores |

> "For this scale, we'd need to shard by user_id using consistent hashing..."

---

## 8. Not Discussing Trade-offs

### The Problem

Presenting decisions as if there's only one option.

### Why It's Bad

- Shows shallow thinking
- Miss opportunity to demonstrate experience
- Interviewer will probe anyway

### How to Avoid

Always present alternatives:
> "We could use either push or pull model here. Push gives faster reads but is expensive for celebrities. I'll go with a hybrid approach..."

---

## 9. Forgetting About Data Consistency

### The Problem

Not addressing how data stays consistent across:

- Multiple replicas
- Distributed caches
- Microservices

### Why It's Bad

- Core distributed systems concept
- Real bugs come from inconsistency
- Shows lack of experience

### How to Avoid

Discuss explicitly:
> "Between services A and B, we'll use eventual consistency. If a user posts a tweet, it might take a few seconds to appear in followers' timelines. This is acceptable for our use case."

---

## 10. Silent Problem Solving

### The Problem

Thinking quietly, then presenting a final answer.

### Why It's Bad

- Interviewer can't help if you're stuck
- Miss opportunity to show reasoning
- Awkward silences

### How to Avoid

Think out loud:
> "I'm considering whether to use SQL or NoSQL here. Given the flexibility of our data model and need for horizontal scaling, I'll go with NoSQL..."

---

## 11. Not Drawing Diagrams

### The Problem

Explaining verbally without visual aids.

### Why It's Bad

- Hard to follow
- Easy to miss components
- Looks unorganized

### How to Avoid

Draw early and often:

1. Start with a simple box diagram
2. Add components as you discuss
3. Show data flow with arrows
4. Label everything clearly

---

## 12. Ignoring Cost

### The Problem

Not considering operational costs.

### Why It's Bad

- Real systems have budgets
- Shows practical thinking
- Can differentiate designs

### How to Avoid

Mention cost trade-offs:
> "We could use Lambda for this, but at high volume, dedicated servers would be more cost-effective..."

---

## 13. Not Prioritizing

### The Problem

Trying to design everything equally.

### Why It's Bad

- Run out of time
- Shallow coverage everywhere
- Miss depth where it matters

### How to Avoid

Prioritize explicitly:
> "Given time constraints, I'll focus on the timeline generation and notification system, treating search as a separate service we'll integrate with."

---

## 14. Forgetting Security

### The Problem

No mention of:

- Authentication
- Authorization
- Data encryption
- Rate limiting

### Why It's Bad

- Security is non-negotiable
- Easy points to score
- Shows maturity

### How to Avoid

At minimum mention:

- Auth at API gateway
- Encryption in transit (TLS)
- Rate limiting for abuse prevention

---

## Quick Checklist

Before concluding your design, verify:

- [ ] Requirements clarified (functional & non-functional)
- [ ] Capacity estimated (QPS, storage, bandwidth)
- [ ] High-level architecture drawn
- [ ] APIs defined for key operations
- [ ] Data model discussed
- [ ] Deep dove on 2-3 components
- [ ] Trade-offs mentioned
- [ ] Failure scenarios addressed
- [ ] Scaling strategy explained
