# Reliability & Fault Tolerance

In distributed systems, **failure is inevitable**. The goal is not to prevent failure, but to build systems that are resilient to it.

## 1. Redundancy, Failover, Replication

### Redundancy

Avoiding Single Points of Failure (SPOF) by duplicating critical components.

- **Active-Passive (Master-Slave)**: One node handles traffic; the other waits. on failure, traffic switches to the passive node.
  - *Pro*: Simple data consistency.
  - *Con*: Waste of resources (passive node sits idle).
- **Active-Active (Master-Master)**: Both nodes handle traffic.
  - *Pro*: Scalable, higher utilization.
  - *Con*: Complex conflict resolution for data synchronization.

### Failover

The process of switching to a redundant node.

- **Graceful Failover**: Planned/Manual switching (e.g., for maintenance).
- **Automatic Failover**: System detects failure (via heartbeats/health checks) and switches.
  - *Risk*: **Split Brain** (both nodes think they are master).

---

## 2. Resilience Patterns (The "Java Hystrix" Patterns)

### A. Circuit Breaker

Prevents an application from repeatedly trying to execute an operation that's likely to fail.

- **States**:
  - **Closed**: Normal operation. Requests pass through.
  - **Open**: Error threshold reached. Requests fail immediately (Fail Fast) without calling the downstream service.
  - **Half-Open**: After a timeout, allow a limited number of test requests. If successful -> Close; if failed -> Open again.
- **Use Case**: Downstream service is overloaded or down. Don't waste threads/resources waiting for timeouts.

### B. Bulkhead Pattern

Isolates elements of an application into pools so that if one fails, the others continue to function (like ship bulkheads).

- **Implementation**: Separate thread pools for Service A and Service B.
- **Benefit**: If Service A hangs and consumes all its threads, Service B still has its own pool and remains responsive.

### C. Retry with Exponential Backoff & Jitter

- **Retry**: Try again if a temporary failure (network blip) occurs.
- **Exponential Backoff**: Wait 1s, then 2s, then 4s, then 8s. Prevents hammering a recovering service.
- **Jitter**: Add randomness ($Wait = 2^n + random\_noise$).
  - *Why?* Prevents **Thundering Herd** problem where all retrying clients hit the server at the exact same synchronized time.

### D. Timeouts

Never make a remote call without a timeout.

- **Connection Timeout**: Time to establish TCP handshake.
- **Read/Socket Timeout**: Time waiting for response data.

---

## 3. Health Checks & Probes (Kubernetes Concepts)

- **Liveness Probe**: "Is the process running?" If no, restart the container. (e.g., Deadlock detection).
- **Readiness Probe**: "Is the service ready to take traffic?" (e.g., Database connection established, cache warmed). If no, remove from Load Balancer.

---

## 4. Graceful Degradation

If a non-critical dependency fails, the system should still function with reduced capability.

- **Fallback**: Return a default value or cached data.
  - *Example*: Recommendation engine is down -> Show "Most Popular Globally" items instead of personalized picks.
  - *Example*: User profile photo fails to load -> Show a placeholder avatar.

---

## 5. Monitoring, Alerting, Observability

- **Metrics**: "What happened?" (CPU usage 90%, Error Rate 5%). Time-series data (Prometheus/Grafana).
- **Logs**: "Why did it happen?" (Stack trace, Error message). (ELK Stack, Splunk).
- **Traces**: "Where did it happen?" (Distributed Tracing, Span ID). Follows a request across microservices. (Jaeger, Zipkin).
- **Golden Signals** (Google SRE):
  1. **Latency**: Time to service a request.
  2. **Traffic**: Demand on system (RPS).
  3. **Errors**: Rate of failed requests.
  4. **Saturation**: Fullness of the system (Memory/CPU).

---

## 6. Chaos Engineering

Discipline of experimenting on a system to build confidence in its capability to withstand turbulent conditions.

- **Principle**: "Break things on purpose in production (carefully)."
- **Tool**: Netflix Chaos Monkey (randomly kills instances).
- **Goal**: Verify that if a server dies, the failover works, the circuit breaker opens, and the user notices nothing.
