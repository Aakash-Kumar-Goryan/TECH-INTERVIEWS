# Back-of-Envelope Estimation

Capacity estimation is asked in **every** FAANG system design interview. Master these numbers and formulas.

## 1. Numbers Every Engineer Should Know

### Latency (Approximate)

| Operation | Latency |
|-----------|---------|
| L1 Cache Reference | 0.5 ns |
| L2 Cache Reference | 7 ns |
| Main Memory Reference | 100 ns |
| SSD Random Read | 150 µs |
| HDD Seek | 10 ms |
| Network Round Trip (Same DC) | 0.5 ms |
| Network Round Trip (Cross-continent) | 150 ms |

### Storage & Throughput

| Metric | Value |
|--------|-------|
| 1 Byte | 8 bits |
| 1 KB | 1,000 Bytes (~1,024) |
| 1 MB | 1,000 KB |
| 1 GB | 1,000 MB |
| 1 TB | 1,000 GB |
| SSD Sequential Read | ~500 MB/s |
| HDD Sequential Read | ~100 MB/s |
| 1 Gbps Network | 125 MB/s |

### Time Conversions

| Period | Seconds |
|--------|---------|
| 1 Day | 86,400 (~100K) |
| 1 Month | 2.5 Million |
| 1 Year | 30 Million |

---

## 2. Common Estimation Formulas

### QPS (Queries Per Second)

```
Daily Active Users (DAU) × Actions per User per Day
─────────────────────────────────────────────────────
                    86,400 seconds
```

**Example**: Twitter with 200M DAU, 5 tweets viewed per day:

- Read QPS = (200M × 5) / 86,400 ≈ **12,000 QPS**
- Peak QPS = 2× to 5× average = **24K - 60K QPS**

### Storage Estimation

```
Users × Data per User × Retention Period
```

**Example**: Instagram photo storage for 1 year:

- 500M users, 2 photos/day, 2MB per photo, 365 days
- Storage = 500M × 2 × 2MB × 365 = **730 PB/year**

### Bandwidth Estimation

```
QPS × Average Response Size
```

**Example**: Serving 10K QPS with 500KB responses:

- Bandwidth = 10,000 × 500KB = 5 GB/s = **40 Gbps**

---

## 3. Worked Example: Design YouTube

### Requirements

- 2 Billion monthly active users
- 500 hours of video uploaded per minute
- Average video: 5 minutes, 50MB

### Upload Calculations

```
Videos/day = 500 hours/min × 60 min × 24 hours × (60/5) = 8.6M videos/day
Upload Storage/day = 8.6M × 50MB = 430 TB/day
Upload Storage/year = 430 TB × 365 ≈ 157 PB/year
```

### View Calculations

```
Assume: Each user watches 5 videos/day
Views/day = 2B users × 5 videos = 10B views/day
View QPS = 10B / 86,400 ≈ 115,000 QPS
```

### Bandwidth for Streaming

```
Assume: Average video bitrate = 5 Mbps
Peak concurrent viewers = 10M (estimate)
Bandwidth = 10M × 5 Mbps = 50 Tbps (CDN required!)
```

---

## 4. Quick Reference Cheat Sheet

| Metric | Quick Estimate |
|--------|----------------|
| 1M users, 1 req/day | ~12 QPS |
| 100M users, 1 req/day | ~1,200 QPS |
| 1B users, 1 req/day | ~12,000 QPS |
| 1 KB × 1M = | 1 GB |
| 1 MB × 1M = | 1 TB |
| 1 GB × 1M = | 1 PB |

### Rule of Thumb: "Million per Second"

If you process **1 million requests per day**, that's roughly **12 requests per second**.

---

## 5. Interview Tips

1. **State Assumptions Clearly**: "I'll assume 100M DAU..."
2. **Round Aggressively**: Use 100K instead of 86,400 for seconds per day.
3. **Show Your Work**: Write formulas, not just answers.
4. **Sanity Check**: If your number seems off, re-verify.
5. **Know Your Powers of 10**: Quickly convert between KB/MB/GB/TB.
