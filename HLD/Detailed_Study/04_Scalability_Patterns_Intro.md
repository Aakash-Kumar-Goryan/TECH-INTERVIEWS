# Scalability & Performance Patterns

## 1. Horizontal vs Vertical Scaling

- **Vertical (Scale Up)**: Bigger machine (More RAM, CPU). Limit: Hardware ceiling, expensive.
- **Horizontal (Scale Out)**: More machines. Unlimited scale. Complexity: Distributed systems problems (Data partitioning, Consistency).

## 2. Stateless vs Stateful Services

- **Stateless**: Server keeps no client context.
  - Any request can go to any server (Round-robin LB is fine).
  - Easy to scale out.
- **Stateful**: Server remembers client (e.g., Session in memory).
  - "Sticky Sessions" required on Load Balancer.
  - Hard to scale. If server dies, session is lost.
  - **Solution**: Move state to external store (Redis) → Make service stateless.

---

## 3. Load Balancing (Critical for FAANG)

### L4 vs L7 Load Balancing

| Layer | Operates On | Can Inspect | Example |
|-------|-------------|-------------|---------|
| **L4 (Transport)** | TCP/UDP packets | IP, Port | AWS NLB, HAProxy (TCP mode) |
| **L7 (Application)** | HTTP requests | URL, Headers, Cookies | AWS ALB, NGINX, HAProxy (HTTP mode) |

**L7 Benefits**: Content-based routing, SSL termination, sticky sessions, request modification.

### Load Balancing Algorithms

| Algorithm | Description | Best For |
|-----------|-------------|----------|
| **Round Robin** | Rotate through servers sequentially | Homogeneous servers, equal load |
| **Weighted Round Robin** | Servers with higher capacity get more | Mixed server capacities |
| **Least Connections** | Send to server with fewest active connections | Long-lived connections (WebSocket) |
| **Least Response Time** | Send to fastest responding server | Latency-sensitive apps |
| **IP Hash** | Hash client IP to always route to same server | Session persistence without cookies |
| **Consistent Hashing** | Minimize redistribution when servers change | Distributed caches |

### Health Checks

- Load balancers periodically check backend health.
- **Active**: LB sends probe requests (HTTP GET /health).
- **Passive**: LB monitors real traffic for errors.
- Unhealthy servers removed from pool automatically.

### Common Load Balancers

| Tool | Type | Notes |
|------|------|-------|
| **NGINX** | L7 (and L4) | Reverse proxy, widely used |
| **HAProxy** | L4/L7 | High performance, TCP focus |
| **AWS ALB** | L7 | Managed, integrates with ECS/EKS |
| **AWS NLB** | L4 | Ultra-low latency, static IPs |
| **Envoy** | L7 | Service mesh sidecar proxy |

---

## 4. Caching Strategies (The "Big 3")

### A. Cache-Aside (Lazy Loading)

- App checks Cache → Miss → App queries DB → App updates Cache.
- *Pro*: Only requested data is cached. Resilient to cache failure.
- *Con*: Cache miss latency. Stale data if DB updated separately.

### B. Write-Through

- App updates Cache → Cache synchronously updates DB.
- *Pro*: Cache always coherent with DB.
- *Con*: Slow writes (double write).

### C. Write-Back (Write-Behind)

- App updates only Cache → return success. Cache async updates DB later.
- *Pro*: Extreme write speed.
- *Con*: **Data Loss risk** if cache crashes before syncing.

### Eviction Policies

- **LRU (Least Recently Used)**: Discard items not used for longest time. (Most common choice).
- **LFU (Least Frequently Used)**: Discard items deeply unpopular.
- **FIFO (First In First Out)**: Simple queue.
- **TTL (Time To Live)**: Expire after fixed duration.

### Cache Invalidation (Hard Problem)

> "There are only two hard things in Computer Science: cache invalidation and naming things."

| Strategy | How It Works | Consistency |
|----------|--------------|-------------|
| **TTL-based** | Cache expires after X seconds | Eventual (stale window) |
| **Event-based** | Publish invalidation events on data change | Near real-time |
| **Version tags** | Include version in cache key (e.g., `user:123:v5`) | Immediate (new key) |

---

## 5. Content Delivery Network (CDN) & Edge

- **Push CDN**: You actively upload content.
- **Pull CDN**: Lazy load (like Cache-Aside).
- **Edge Computing**: Running logic (AWS Lambda@Edge) closer to user (e.g., Image resizing, Authentication check).

### CDN Cache Headers

```http
Cache-Control: public, max-age=31536000  # Cache for 1 year
Cache-Control: private, no-cache         # Don't cache (auth tokens)
```

---

## 6. Microservices Patterns

### Service Communication

| Pattern | Description | Use Case |
|---------|-------------|----------|
| **Synchronous (REST/gRPC)** | Request blocks until response | Real-time queries |
| **Asynchronous (Message Queue)** | Fire and forget, eventual processing | Background jobs, decoupling |

### API Gateway Pattern

Single entry point for all clients. Handles:

- Routing to backend services
- Authentication/Authorization
- Rate limiting
- Request/Response transformation
- SSL termination

**Tools**: Kong, AWS API Gateway, Apigee, Nginx

### Service Discovery

How services find each other in a dynamic environment.

- **Client-Side**: Client queries registry (Consul, Eureka), then calls service directly.
- **Server-Side**: Load balancer queries registry, routes request.
- **DNS-Based**: Services registered as DNS entries (Kubernetes Services).

### Circuit Breaker & Bulkhead

(See `05_Reliability_Patterns.md` for details)

Prevent cascading failures in microservices.

### Service Mesh (Advanced)

Dedicated infrastructure layer for service-to-service communication.

- **Sidecar Proxy**: Each service has a proxy (Envoy) handling traffic.
- **Features**: mTLS, traffic management, observability, retries.
- **Tools**: Istio, Linkerd, Consul Connect.

```
Service A → [Envoy Proxy] → [Envoy Proxy] → Service B
                    ↓              ↓
               Control Plane (Istio)
```

---

## 7. Connection Pooling

Reuse database connections instead of creating new ones per request.

### Problem Without Pooling

```
Request 1 → Open DB connection → Query → Close
Request 2 → Open DB connection → Query → Close  # Expensive!
```

### With Connection Pool

```
Pool maintains N open connections
Request 1 → Borrow connection → Query → Return to pool
Request 2 → Borrow connection → Query → Return to pool
```

**Benefits**: Reduced latency (no handshake), controlled resource usage.

**Tools**: HikariCP (Java), pgBouncer (Postgres), ProxySQL (MySQL).

---

## 8. Performance Bottlenecks

- **CPU Bound**: Optimization: Profiling, Caching results, Async processing.
- **Memory Bound**: Optimization: Pagination, Stream processing (don't load whole file).
- **I/O Bound (Disk/Network)**: Optimization: Batching, Compression, Asynchronous I/O.

### Amdahl's Law

> The speedup from parallelization is limited by the sequential portion of the task.

If 10% of work is sequential, max speedup is 10x (even with infinite parallelism).
