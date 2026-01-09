# Deployment Strategies

## 1. Blue-Green Deployment

- **Concept**: Maintain TWO identical production environments (Blue and Green).
  - **Blue**: Currently running live traffic (Version 1).
  - **Green**: Idle or staging. You deploy Version 2 here.
- **Switch**: Once Green is tested and ready, you switch the Load Balancer to point to Green.
- **Rollback**: Instant. Just switch LB back to Blue.
- **Cost**: High (Double infrastructure).

## 2. Canary Deployment

- **Concept**: Roll out the update to a small subset of users (e.g., 5%) first.
- **Process**:
    1. Deploy V2 to 5% of servers.
    2. Monitor metrics (Errors, Latency).
    3. If good, increase to 25%, then 50%, then 100%.
- **Pros**: Low risk. Real-world testing.
- **Cons**: Slow rollout. Managing two versions simultaneously (DB schema compatibility is key).

## 3. Rolling Deployment

- **Concept**: Update servers one by one (or in batches).
- **Process**:
  - Take Server A out of LB -> Update -> Put back.
  - Repeat for Server B.
- **Pros**: No extra infrastructure cost.
- **Cons**: Slow rollback.

## 4. A/B Testing vs Feature Flags

- **A/B Testing**: Marketing/Product experiment (Does blue button convert better than red?).
- **Feature Flags**: Engineering technique. Deploy code but keep it hidden/disabled via a config flag (`if (feature.enabled) ...`). allows "Dark Launching".
