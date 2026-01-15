# Google-Specific System Design Tips

Insights for succeeding in Google L4/L5 system design interviews.

## What Google Values

### 1. Simplicity Over Cleverness

Google's engineering culture emphasizes **simple, maintainable solutions**.

> "Simple things should be simple, complex things should be possible."

**What this means in interviews**:

- Start with the simplest design that works
- Add complexity only when justified by requirements
- Explain *why* you're adding each component

**Good**: "I'll start with a single database and add sharding when we exceed 10M users."

**Avoid**: "Let's immediately use Kubernetes, Kafka, and ML-based routing."

---

### 2. Scale Thinking

Google operates at massive scale. Demonstrate you understand:

- **Billions of users**
- **Petabytes of data**
- **Millions of QPS**

**What this means in interviews**:

- Always ask about scale
- Do capacity estimation
- Design for horizontal scaling from the start

**Questions to ask**:

- "How many daily active users?"
- "What's the data retention period?"
- "What latency is acceptable at P99?"

---

### 3. Trade-off Articulation

Google values engineers who understand there are no perfect solutions.

**Every decision has trade-offs**:

| Decision | Trade-off |
|----------|-----------|
| Caching | Faster reads, but stale data possible |
| Sharding | Scalable, but cross-shard queries are hard |
| Microservices | Independent deployment, but operational complexity |

**How to present**:
> "We could go with either SQL or NoSQL here. SQL gives us ACID and joins, but NoSQL would give us better horizontal scaling. Given our need to scale to billions of records with simple access patterns, I'd choose NoSQL."

---

### 4. Data Model Rigor

Google is a data company. Strong data modeling skills are critical.

**What to demonstrate**:

- Schema design
- Indexing strategy
- Access patterns
- Consistency requirements

**Template for data discussion**:

1. What entities do we have?
2. What are the relationships?
3. What are the access patterns?
4. What needs to be indexed?
5. What are the consistency requirements?

---

### 5. Extensibility and Future-Proofing

Design for evolution, not just today's requirements.

**Questions to ask yourself**:

- "What if we add a new feature X?"
- "What if traffic 10x?"
- "What if we need to support mobile?"

**Good design signals**:

- Clean interfaces between components
- Configuration-driven behavior
- Modular architecture

---

## Google's Tech Stack (Know These)

| Technology | Use Case | Interview Relevance |
|------------|----------|---------------------|
| **Bigtable** | Wide-column store | Sparse data, time-series |
| **Spanner** | Global distributed SQL | Strong consistency at scale |
| **Pub/Sub** | Message queue | Event-driven architectures |
| **Cloud Storage (GCS)** | Blob storage | File uploads, backups |
| **BigQuery** | Data warehouse | Analytics, batch processing |
| **Dataflow** | Stream/batch processing | Data pipelines |
| **Kubernetes (GKE)** | Container orchestration | Microservices deployment |
| **Memorystore** | Managed Redis | Caching |

**How to use in interviews**:
> "For this, I'd use something like Bigtable for the time-series data, given its ability to handle sparse columns and range queries efficiently."

---

## L5-Specific Expectations

### Technical Depth

L5 (Senior) is expected to:

- Drive the design independently
- Identify and address edge cases
- Propose multiple solutions with trade-offs
- Deep dive into critical components

### Scope and Ambiguity

L5 handles ambiguous problems:

- Clarify requirements without excessive hand-holding
- Identify what's in/out of scope
- Prioritize effectively

### Leadership Signals

Show you can lead design discussions:

- Structure your approach clearly
- Drive the conversation
- Synthesize feedback
- Make decisions with justification

---

## Interview Flow at Google

### Typical Structure (45 min)

| Phase | Time | Focus |
|-------|------|-------|
| Problem introduction | 2-3 min | Interviewer presents problem |
| Clarification | 5-7 min | You ask questions |
| Design | 30-35 min | You drive the design |
| Q&A | 5 min | Interviewer challenges design |

### What Interviewers Evaluate

| Dimension | What They Look For |
|-----------|-------------------|
| **Problem Solving** | Structured approach, good requirements |
| **Technical Depth** | Understanding of distributed systems |
| **Design Quality** | Scalable, reliable, maintainable |
| **Communication** | Clear explanations, good whiteboarding |
| **Collaboration** | Incorporates feedback, asks good questions |

---

## Common Google System Design Questions

### Infrastructure-Focused

| Question | Key Concepts |
|----------|--------------|
| Design YouTube | CDN, video encoding, adaptive streaming |
| Design Google Drive | File sync, chunking, conflict resolution |
| Design Google Maps | Geo-indexing, routing, tile serving |
| Design Gmail | Email storage, search, spam detection |

### Data-Intensive

| Question | Key Concepts |
|----------|--------------|
| Design Web Crawler | Politeness, deduplication, URL frontier |
| Design Search Engine | Inverted index, ranking, distributed search |
| Design Ad Click Aggregation | Real-time aggregation, fraud detection |
| Design Metrics System | Time-series, downsampling, alerting |

### Real-Time

| Question | Key Concepts |
|----------|--------------|
| Design Google Docs | CRDTs/OT, WebSocket, collaboration |
| Design Google Meet | WebRTC, SFU/MCU, audio/video sync |
| Design Notification System | Multi-channel, delivery guarantees |

---

## Google-Specific Terminology

Know these terms and concepts:

| Term | Meaning |
|------|---------|
| **SLO/SLA** | Service Level Objective/Agreement |
| **Error budget** | Acceptable downtime based on SLO |
| **Toil** | Manual, repetitive operational work |
| **Chaos engineering** | Intentional failure injection |
| **Blameless postmortem** | Learning from failures without blame |
| **Site Reliability Engineering (SRE)** | Google's ops philosophy |

---

## Communication Tips

### Whiteboarding

- Draw diagrams early
- Use clear labels
- Show data flow direction
- Keep it organized (not cramped)

### Verbal

- Think out loud
- Signpost transitions ("Now let me talk about storage...")
- Summarize key points
- Check in with interviewer ("Does this make sense so far?")

### Handling Challenges

When interviewer pushes back:
> "That's a good point. You're right that this could be a bottleneck. Let me think about alternatives..."

Never be defensive. Treat it as collaboration.

---

## Pre-Interview Checklist

- [ ] Review Google's public engineering blogs (Google AI Blog, SRE Book)
- [ ] Know latency numbers cold
- [ ] Practice back-of-envelope estimation
- [ ] Prepare 2-3 systems you've designed (to discuss in behavioral)
- [ ] Review CAP theorem and consistency models
- [ ] Know difference between L4 and L7 load balancing
- [ ] Understand event-driven architectures
- [ ] Practice drawing diagrams on whiteboard/paper

---

## Resources

### Official Google Sources

- [Google SRE Book](https://sre.google/books/) (Free online)
- [Google Cloud Architecture Center](https://cloud.google.com/architecture)
- [Google Research Blog](https://research.google/blog/)

### Papers Worth Reading

| Paper | Why |
|-------|-----|
| MapReduce | Foundational distributed computing |
| Bigtable | Wide-column storage design |
| Spanner | Global distributed database |
| Borg | Cluster management (precursor to K8s) |
| Dapper | Distributed tracing |

### Key Insight

> Google interviewers aren't looking for you to know their specific systems. They want to see **how you think** about building systems at scale with good engineering principles.
