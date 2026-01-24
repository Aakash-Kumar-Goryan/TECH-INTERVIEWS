# System Design Interview Rubric

A scoring guide showing what interviewers evaluate at each level. Use this to self-assess your practice sessions and identify gaps.

---

## Overview: What Interviewers Look For

| Dimension | Weight | L4 Expectation | L5 Expectation |
|-----------|--------|----------------|----------------|
| **Problem Exploration** | 15% | Clarifies requirements, identifies key constraints | Proactively uncovers edge cases, challenges assumptions |
| **Technical Breadth** | 20% | Covers all major components | Demonstrates awareness of alternatives & emerging tech |
| **Technical Depth** | 25% | Deep dive into 1-2 areas | Deep expertise in 2-3 areas with battle-tested insights |
| **Trade-off Analysis** | 20% | Identifies trade-offs | Quantifies trade-offs with data, makes informed decisions |
| **Communication** | 10% | Clear explanation | Leads the discussion, adapts to interviewer cues |
| **Operational Maturity** | 10% | Basic failure handling | SLOs, observability, runbooks, incident response |

---

## Detailed Scoring Rubric

### 1. Problem Exploration (15%)

| Score | Criteria |
|-------|----------|
| **1-2** | Jumps to solution, minimal questions |
| **3-4** | Asks basic questions (users, scale, features) |
| **5-6** | Clarifies functional + non-functional requirements, defines scope |
| **7-8** | Identifies edge cases, challenges assumptions, prioritizes features |
| **9-10** | Uncovers hidden constraints, proposes phased approach, aligns with business goals |

**L4 Target**: 6-7 | **L5 Target**: 8-9

**Key Questions to Ask**:

```
Functional:
- Who are the users? (consumers, businesses, internal)
- What are the core use cases? What's MVP vs nice-to-have?
- What operations need to be supported? (CRUD, search, real-time)

Non-Functional:
- What's the expected scale? (DAU, QPS, data volume)
- What's the latency requirement? (p50, p99)
- Availability target? (99.9% = 8.7h downtime/year, 99.99% = 52min)
- Consistency needs? (strong vs eventual)
- Retention policy? (7 days, 1 year, forever)

Scope:
- Should I design auth/payments or assume they exist?
- Single region or global?
- What's out of scope for this discussion?
```

---

### 2. Technical Breadth (20%)

| Score | Criteria |
|-------|----------|
| **1-2** | Missing major components (no caching, no queue, wrong DB) |
| **3-4** | Basic architecture with obvious components |
| **5-6** | Complete architecture covering all major needs |
| **7-8** | Considers CDN, rate limiting, API gateway, monitoring |
| **9-10** | Proposes alternative architectures, knows when to use each |

**L4 Target**: 6-7 | **L5 Target**: 8-9

**Component Checklist**:

| Layer | Components to Consider |
|-------|----------------------|
| **Clients** | Web, Mobile, IoT, Internal |
| **Edge** | CDN, DNS, DDoS protection |
| **Ingress** | Load Balancer, API Gateway, Rate Limiter |
| **Compute** | Microservices, Serverless, Workers |
| **Caching** | Local cache, Distributed cache (Redis), CDN cache |
| **Data** | SQL (ACID), NoSQL (scale), Search (Elasticsearch), Time-series |
| **Async** | Message Queue, Event Streaming (Kafka), Pub/Sub |
| **Storage** | Blob storage (S3), File system, Data lake |
| **Observability** | Metrics, Logs, Traces, Alerting |

---

### 3. Technical Depth (25%)

| Score | Criteria |
|-------|----------|
| **1-2** | Surface-level explanations, no deep dive |
| **3-4** | Basic understanding of chosen components |
| **5-6** | Solid deep dive into 1 area with data model, algorithm |
| **7-8** | Deep expertise in 2 areas, discusses internals and edge cases |
| **9-10** | Battle-tested insights, knows failure modes, has built similar systems |

**L4 Target**: 6-7 | **L5 Target**: 8-9

**Deep Dive Checklist** (for each critical component):

```markdown
□ Data Model
  - Schema design with specific fields and types
  - Relationships (1:1, 1:N, M:N)
  - Indexes (primary, secondary, composite)
  - Estimated row count and growth rate

□ Algorithm/Approach
  - Step-by-step flow
  - Time/space complexity
  - Alternatives considered

□ Scaling Strategy
  - Horizontal vs vertical
  - Sharding key selection
  - Replication strategy (sync/async)
  - Estimated shard count with math

□ Failure Handling
  - What if this component fails?
  - Retry policy, circuit breaker
  - Degraded mode behavior
```

---

### 4. Trade-off Analysis (20%)

| Score | Criteria |
|-------|----------|
| **1-2** | No trade-offs mentioned |
| **3-4** | Mentions trade-offs superficially ("it's a trade-off") |
| **5-6** | Clearly articulates 2-3 trade-offs with pros/cons |
| **7-8** | Quantifies impact, makes reasoned decisions |
| **9-10** | Provides data/experience to support decisions, considers business context |

**L4 Target**: 6-7 | **L5 Target**: 8-9

**Common Trade-off Pairs**:

| Decision Point | Option A | Option B |
|---------------|----------|----------|
| Consistency vs Availability | Strong consistency (banking) | Eventual consistency (social) |
| Push vs Pull | Fast reads, expensive writes | Simple writes, expensive reads |
| SQL vs NoSQL | ACID, complex queries | Horizontal scale, flexible schema |
| Sync vs Async | Simple, immediate feedback | Decoupled, resilient, complex |
| Monolith vs Microservices | Simple, fast initial dev | Scalable, independent deployment |
| Cache-aside vs Write-through | Simple, potential inconsistency | Consistent, more complex |

**How to Present Trade-offs**:

```
"The trade-off here is between [A] and [B].

Option 1: [Approach]
  - Pros: [benefits with numbers if possible]
  - Cons: [drawbacks with impact]

Option 2: [Alternative]
  - Pros: [benefits]
  - Cons: [drawbacks]

I recommend Option [X] because [specific reason tied to requirements].
For example, since we need <200ms p99 latency and can tolerate 
eventual consistency, the push model gives us the read performance 
we need at the cost of higher write amplification."
```

---

### 5. Communication (10%)

| Score | Criteria |
|-------|----------|
| **1-2** | Silent, unclear, jumps around randomly |
| **3-4** | Explains when asked, somewhat organized |
| **5-6** | Clear structure, verbalizes thinking |
| **7-8** | Leads discussion, checks in with interviewer, adapts |
| **9-10** | Masterful storytelling, handles curveballs gracefully |

**L4 Target**: 6-7 | **L5 Target**: 8-9

**Communication Tips**:

```markdown
✅ DO:
- "Let me structure my approach..."
- "Before diving into the solution, I want to clarify..."
- "I'm going to make an assumption here: [X]. Does that work?"
- "Let me pause and see if you want me to go deeper on this"
- "To summarize what we've covered so far..."
- "The key insight here is..."

❌ DON'T:
- Silent whiteboarding for minutes
- Jumping between topics without transitions
- Ignoring interviewer's hints/questions
- Over-explaining basics (shows lack of experience)
- Under-explaining critical decisions
```

---

### 6. Operational Maturity (10%)

| Score | Criteria |
|-------|----------|
| **1-2** | No mention of operations, monitoring, or failures |
| **3-4** | Basic failure handling ("we add redundancy") |
| **5-6** | Discusses monitoring, alerting, basic incident response |
| **7-8** | Defines SLOs, key metrics, escalation procedures |
| **9-10** | Complete operational model: SLOs, runbooks, chaos engineering, cost |

**L4 Target**: 5-6 | **L5 Target**: 7-9

**Operational Checklist**:

```markdown
□ SLOs/SLAs
  - Availability target (e.g., 99.9%)
  - Latency targets (p50, p99)
  - Error budget and burn rate

□ Key Metrics (per service)
  - Request rate (QPS)
  - Error rate (4xx, 5xx)
  - Latency distribution
  - Saturation (CPU, memory, connections)

□ Alerting
  - What triggers a page?
  - Threshold definitions
  - Escalation policy

□ Failure Scenarios
  - Single node failure
  - Entire AZ failure
  - Database failure
  - Upstream dependency failure
  - Degraded mode behavior

□ Runbook Actions
  - How to diagnose
  - How to mitigate
  - How to rollback
```

---

## Quick Self-Assessment Template

After each practice session, score yourself:

```markdown
## Design: [System Name]
## Date: [Date]
## Time: [Duration]

### Scores (1-10)

| Dimension | Score | Notes |
|-----------|-------|-------|
| Problem Exploration | /10 | |
| Technical Breadth | /10 | |
| Technical Depth | /10 | |
| Trade-off Analysis | /10 | |
| Communication | /10 | |
| Operational Maturity | /10 | |

### Weighted Total
(Exploration × 0.15) + (Breadth × 0.20) + (Depth × 0.25) + 
(Trade-offs × 0.20) + (Communication × 0.10) + (Operations × 0.10)

= ___/10

### L4 Threshold: 6.5+ | L5 Threshold: 7.5+

### Top 3 Areas to Improve
1. 
2. 
3. 

### Key Learnings
-
```

---

## Red Flags That Hurt Your Score

| Red Flag | Impact | How to Fix |
|----------|--------|------------|
| No capacity estimation | -2 points | Always do BOTE, even if brief |
| Single point of failure | -2 points | Add redundancy to every component |
| Wrong database choice | -2 points | Know when SQL vs NoSQL vs specialized |
| No caching strategy | -1 point | Always consider read/write patterns |
| Ignoring consistency requirements | -2 points | Clarify early, design accordingly |
| No failure handling | -2 points | Discuss what happens when X fails |
| Over-engineering for scale | -1 point | Start simple, scale incrementally |
| Under-engineering for scale | -2 points | Know when scale matters (>10K QPS) |

---

## Level Comparison Summary

| Aspect | L4 (Senior) | L5 (Staff) |
|--------|-------------|------------|
| **Scope** | Single system/service | Cross-system, org-wide impact |
| **Depth** | 1-2 deep dives | 2-3 deep dives with battle scars |
| **Trade-offs** | Identifies and explains | Quantifies and decides with data |
| **Operations** | Basic monitoring/alerting | Full operational model, SLOs |
| **Communication** | Clear and structured | Leads discussion, influences direction |
| **Experience** | "This is how it works" | "This is what we learned when we built X" |

**L5 Differentiators**:

- Mentions real-world experience: "At my previous company, we tried X and learned..."
- Discusses organizational aspects: "This design requires a team of N engineers over M months"
- Considers cost: "This architecture costs approximately $X/month at this scale"
- Proposes phased rollout: "We'd start with Y and migrate to Z once we hit 10K QPS"
