# Latency vs Throughput

This document expands on the section from the [System Design Fundamentals Explanation](./Explanation.md) and provides deeper insights into these key performance metrics in system design.

## Latency
Latency is the time taken to process a single request or operation. It is typically measured in milliseconds (ms) or seconds (s).

- **Low latency:** Essential for real-time applications (e.g., gaming, trading, chat).
- **Sources of latency:** Network delays, disk I/O, processing time, queuing, serialization/deserialization.
- **Measurement:** End-to-end (client to server and back), or component-level (e.g., database query time).
- **Optimization strategies:** Caching, efficient algorithms, reducing network hops, parallel processing, minimizing serialization overhead.

## Throughput
Throughput is the number of requests or operations processed per unit time (e.g., requests/sec, transactions/min).

- **High throughput:** Important for batch processing, data pipelines, APIs serving many users.
- **Measurement:** Aggregate over a time window (e.g., 10,000 requests/min).
- **Optimization strategies:** Load balancing, horizontal scaling, batching, asynchronous processing, resource pooling.

## Relationship and Trade-offs
- **Latency vs Throughput:** Reducing latency can improve user experience, while increasing throughput improves system capacity. Sometimes, optimizing for one can impact the other (e.g., batching increases throughput but may add latency).
- **Use case considerations:** Choose optimization strategies based on application needs (e.g., prioritize latency for interactive apps, throughput for data processing).

## Design Considerations
- Monitor both metrics to identify bottlenecks and optimize system performance.
- Use appropriate tools (e.g., APMs, distributed tracing) for measurement and analysis.
- Balance resources to meet service-level objectives (SLOs) for both latency and throughput.

---
For foundational concepts, see the [System Design Fundamentals Explanation](./Explanation.md).
