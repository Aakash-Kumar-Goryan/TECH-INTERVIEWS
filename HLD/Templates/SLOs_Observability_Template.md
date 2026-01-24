# SLOs & Observability Template

A reusable template for defining Service Level Objectives, metrics, alerting, and dashboards.

---

## Quick Reference: SLO Terminology

| Term | Definition | Example |
|------|------------|---------|
| **SLI** (Indicator) | Quantitative measure of service behavior | Request latency p99 |
| **SLO** (Objective) | Target value for an SLI | p99 latency < 200ms |
| **SLA** (Agreement) | Contractual commitment with consequences | 99.9% uptime or credits |
| **Error Budget** | Allowed failures = 100% - SLO | 0.1% = 43 min/month downtime |

---

## 1. Availability Targets Reference

| Target | Downtime/Year | Downtime/Month | Use Case |
|--------|---------------|----------------|----------|
| 99% (2 nines) | 3.65 days | 7.3 hours | Internal tools |
| 99.9% (3 nines) | 8.76 hours | 43 min | Most web apps |
| 99.95% | 4.38 hours | 21.6 min | Business-critical |
| 99.99% (4 nines) | 52.6 min | 4.3 min | Financial, healthcare |
| 99.999% (5 nines) | 5.26 min | 26 sec | Life-safety systems |

---

## 2. SLO Definition Template

```markdown
### Service: [Name]
**Criticality**: P0/P1/P2

#### Availability SLO
- Target: 99.9%
- Formula: (Total - 5xx) / Total × 100%
- Error Budget: 43 min/month
- Burn Rate Alert: Page if >2%/hour

#### Latency SLO
| Percentile | Target |
|------------|--------|
| p50 | < 50ms |
| p99 | < 200ms |

#### Throughput
- Baseline: X QPS
- Peak: 5× baseline
- Provisioned capacity: 2× peak
```

---

## 3. The Four Golden Signals

| Signal | What to Measure | Alert Threshold |
|--------|-----------------|-----------------|
| **Traffic** | Requests/sec by endpoint | Sudden drop >50% |
| **Errors** | 5xx rate, timeout rate | >0.1% for 5 min |
| **Latency** | p50, p90, p99 | p99 > target for 5 min |
| **Saturation** | CPU, memory, connections | >80% for 10 min |

---

## 4. Key Metrics Per Component

### API Service

```
- http_requests_total{status, endpoint}
- http_request_duration_seconds (histogram)
- http_errors_total{type}
```

### Database

```
- db_query_duration_seconds
- db_connections_active
- db_connections_max
- db_replication_lag_seconds
```

### Cache

```
- cache_hits_total
- cache_misses_total
- cache_hit_rate (hits / (hits + misses))
- cache_evictions_total
```

### Queue

```
- queue_depth
- queue_oldest_message_age_seconds
- consumer_lag_seconds
- messages_processed_total
```

---

## 5. Alert Severity Levels

| Severity | Response | Examples |
|----------|----------|----------|
| **P0** | 5 min, war room | Complete outage |
| **P1** | 30 min | SLO breach, degradation |
| **P2** | 4 hours | Approaching limits |
| **P3** | Next day | Anomalies, warnings |

### Sample Alert Rules

```yaml
# High Error Rate
- alert: HighErrorRate
  expr: error_rate > 0.01
  for: 5m
  severity: P1
  
# High Latency
- alert: HighLatency  
  expr: histogram_quantile(0.99, http_duration) > 0.2
  for: 5m
  severity: P1

# Error Budget Burn
- alert: FastBudgetBurn
  expr: burn_rate_1h > 0.02
  severity: P1
```

---

## 6. Dashboard Layout

```
┌─────────────┬─────────────┬─────────────┬─────────────┐
│ Availability│ Error Rate  │ p99 Latency │ Current QPS │
│   99.95%    │    0.03%    │    145ms    │   12,340    │
└─────────────┴─────────────┴─────────────┴─────────────┘

┌─────────────────────────┬─────────────────────────────┐
│ Request Rate (24h)      │ Error Rate (24h)            │
└─────────────────────────┴─────────────────────────────┘

┌─────────────────────────┬─────────────────────────────┐
│ Latency Heatmap         │ p50/p90/p99 Trend           │
└─────────────────────────┴─────────────────────────────┘

┌─────────────────────────┬─────────────────────────────┐
│ Dependency Health       │ Resource Saturation         │
└─────────────────────────┴─────────────────────────────┘
```

---

## 7. Structured Logging

```json
{
  "timestamp": "2024-01-15T10:30:45.123Z",
  "level": "INFO",
  "service": "user-service",
  "trace_id": "abc123",
  "user_id": "u_789",
  "endpoint": "/api/v1/users",
  "method": "POST",
  "status": 201,
  "latency_ms": 45,
  "message": "User created"
}
```

**Log Levels**:

- ERROR: Unexpected failures (page-worthy)
- WARN: Recoverable issues (retry succeeded)
- INFO: Key business events
- DEBUG: Detailed flow (off in prod)

---

## 8. Distributed Tracing Tags

```markdown
Required:
- service.name
- http.method, http.status_code, http.url
- error (boolean)

Recommended:
- user.id, request.id
- db.statement (sanitized)
- cache.hit (boolean)
```

---

## 9. Incident Severity

| SEV | Impact | Response |
|-----|--------|----------|
| 1 | Complete outage | All hands |
| 2 | Significant degradation | On-call + backup |
| 3 | Minor degradation | On-call only |
| 4 | Minimal/cosmetic | Ticket |

---

## 10. Interview Talking Points

1. **"For SLOs, I'd start with..."**
   - Availability: 99.9% (43 min/month budget)
   - Latency: p99 < 200ms

2. **"The four golden signals are..."**
   - Traffic, Errors, Latency, Saturation

3. **"For alerting, I use error budget burn rate..."**
   - Fast burn (>2%/hr) → page immediately
   - Slow burn → ticket

4. **"In an incident, I check..."**
   - Recent deployments
   - Dependency health
   - Resource saturation

5. **"Each service dashboard shows..."**
   - Big numbers: availability, error rate, p99, QPS
   - Time series: traffic, errors, latency
   - Dependencies and saturation gauges
