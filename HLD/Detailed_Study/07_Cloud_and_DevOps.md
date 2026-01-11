# Cloud & DevOps Essentials

Understanding cloud primitives and container orchestration is expected in FAANG-level system design.

## 1. AWS Core Services (Quick Reference)

| Service | Category | One-Line Description |
|---------|----------|---------------------|
| **EC2** | Compute | Virtual machines (instances). Choose instance type for CPU/RAM. |
| **Lambda** | Compute | Serverless functions. Pay per invocation. Max 15 min runtime. |
| **S3** | Storage | Object storage. Infinite scale. 11 9's durability. |
| **EBS** | Storage | Block storage for EC2. Persistent disk. |
| **RDS** | Database | Managed relational DB (MySQL, Postgres, Aurora). |
| **DynamoDB** | Database | Managed NoSQL (Key-Value). Single-digit ms latency. |
| **ElastiCache** | Cache | Managed Redis/Memcached. |
| **SQS** | Messaging | Managed queue. Decouples services. |
| **SNS** | Messaging | Pub/Sub notifications. Fan-out pattern. |
| **CloudFront** | CDN | Content delivery. Caches at edge locations. |
| **Route 53** | DNS | Managed DNS. Supports health checks, failover. |
| **ELB/ALB** | Networking | Load balancer. L4 (NLB) or L7 (ALB). |
| **VPC** | Networking | Virtual private network. Subnets, security groups. |
| **IAM** | Security | Users, roles, policies. Principle of least privilege. |
| **CloudWatch** | Monitoring | Metrics, logs, alarms. |
| **KMS** | Security | Managed encryption keys. |

---

## 2. Kubernetes Concepts

Kubernetes (K8s) is the industry-standard container orchestration platform.

### Core Objects

| Object | Purpose |
|--------|---------|
| **Pod** | Smallest deployable unit. One or more containers sharing network/storage. |
| **Deployment** | Manages ReplicaSets. Declarative updates, rollbacks. |
| **ReplicaSet** | Ensures N replicas of a Pod are running. |
| **Service** | Stable network endpoint for Pods (ClusterIP, NodePort, LoadBalancer). |
| **Ingress** | L7 HTTP routing. Routes external traffic to Services. |
| **ConfigMap** | Store non-sensitive configuration. Mounted as files or env vars. |
| **Secret** | Store sensitive data (passwords, API keys). Base64 encoded. |
| **StatefulSet** | For stateful apps (databases). Stable network identity, ordered deployment. |
| **DaemonSet** | Runs one Pod per node (logging agents, monitoring). |
| **Job / CronJob** | Run-to-completion tasks. Scheduled or one-time. |

### Service Types

```
ClusterIP ──→ Internal only (default)
NodePort  ──→ Exposes on each node's IP:Port
LoadBalancer ──→ Provisions cloud LB (AWS ELB)
```

### Pod Lifecycle

```
Pending → Running → Succeeded/Failed
              ↓
          CrashLoopBackOff (if container crashes repeatedly)
```

### Resource Requests & Limits

```yaml
resources:
  requests:
    memory: "256Mi"  # Guaranteed minimum
    cpu: "250m"      # 0.25 CPU cores
  limits:
    memory: "512Mi"  # Max allowed (OOMKilled if exceeded)
    cpu: "500m"
```

---

## 3. Container Networking & Service Discovery

### How Pods Communicate

- Each Pod gets a unique IP within the cluster.
- Pods can reach other Pods directly by IP (flat network).
- **Services** provide stable DNS names: `my-service.namespace.svc.cluster.local`

### DNS in Kubernetes

- CoreDNS runs as a cluster add-on.
- Service `order-service` in namespace `prod` → `order-service.prod.svc.cluster.local`

### Ingress Controllers

Common choices:

- **NGINX Ingress**: Most popular, feature-rich.
- **Traefik**: Auto-discovery, Let's Encrypt integration.
- **AWS ALB Ingress**: Provisions AWS Application Load Balancers.

---

## 4. CI/CD & Deployment Strategies

### GitOps Model

- Infrastructure and application config stored in Git.
- Changes via Pull Requests → Reviewed → Merged → Auto-deployed.
- Tools: **ArgoCD**, **Flux**.

### Deployment Strategies (Kubernetes)

| Strategy | How It Works | Risk |
|----------|--------------|------|
| **Rolling Update** | Replace pods gradually (default). | Brief mixed versions. |
| **Recreate** | Kill all old, then start new. | Downtime. |
| **Blue-Green** | Two identical envs. Switch traffic. | Requires 2x resources. |
| **Canary** | Route % of traffic to new version. | Complex traffic splitting. |

### Blue-Green in K8s

```yaml
# Switch Service selector from v1 to v2
selector:
  app: myapp
  version: v2  # Was v1
```

### Canary with Istio/Service Mesh

```yaml
# Traffic splitting
- route:
  - destination:
      host: myapp
      subset: v1
    weight: 90
  - destination:
      host: myapp
      subset: v2
    weight: 10
```

---

## 5. Observability Stack

The three pillars: **Metrics, Logs, Traces**.

### Prometheus + Grafana (Metrics)

- **Prometheus**: Pull-based metrics collection. Stores time-series data.
- **Grafana**: Visualization dashboards. Query Prometheus with PromQL.

```promql
# Example: CPU usage
rate(container_cpu_usage_seconds_total{pod="myapp"}[5m])
```

### ELK / EFK Stack (Logs)

- **Elasticsearch**: Search and analytics engine.
- **Logstash / Fluentd**: Log collection and processing.
- **Kibana**: Log visualization and exploration.

### Distributed Tracing (Traces)

- **Jaeger / Zipkin**: Trace requests across microservices.
- **OpenTelemetry**: Vendor-neutral instrumentation.

Each request gets a **Trace ID**. Each service call is a **Span**.

```
Trace ID: abc123
├── Span 1: API Gateway (10ms)
├── Span 2: Order Service (50ms)
│   └── Span 3: Database Query (30ms)
└── Span 4: Payment Service (100ms)
```

---

## 6. Infrastructure as Code (IaC)

### Terraform

- Declarative. Define desired state, Terraform figures out changes.
- Provider-agnostic (AWS, GCP, Azure).

```hcl
resource "aws_instance" "web" {
  ami           = "ami-12345678"
  instance_type = "t3.micro"
  tags = {
    Name = "WebServer"
  }
}
```

### Terraform vs CloudFormation

| Aspect | Terraform | CloudFormation |
|--------|-----------|----------------|
| Provider | Multi-cloud | AWS only |
| State | External (S3 + DynamoDB) | Managed by AWS |
| Language | HCL | YAML/JSON |

---

## 7. Interview Talking Points

- **Why Kubernetes?** Container orchestration, self-healing, declarative config, portability.
- **Stateless vs Stateful in K8s**: Prefer Deployments for stateless. Use StatefulSets for databases (but often better to use managed DB like RDS).
- **Service Mesh (Istio, Linkerd)**: Handles mTLS, traffic management, observability between services. Adds sidecar proxy to each Pod.
- **Serverless Trade-offs**: Fast scaling, no infra management. But: cold starts, vendor lock-in, 15-min limit.
