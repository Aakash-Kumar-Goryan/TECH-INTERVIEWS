# Miscellaneous System Design Topics

## 1. API Versioning Strategies

When breaking changes are unavoidable, you must version your API.

### A. URI Path Versioning (Most Common)

- `GET /v1/users/123`
- **Pros**: Explicit, easy to cache, easy to explore via browser.
- **Cons**: Technically different resource URLs.

### B. Header Versioning

- `Accept: application/vnd.myapi.v1+json`
- **Pros**: Cleaner URLs (`GET /users/123` stays same). REST purist approach.
- **Cons**: Harder to test manually (cannot just paste URL in browser), Varying headers breaks caching if not careful.

### C. Query Parameter

- `GET /users/123?version=1`
- **Pros**: Simple.
- **Cons**: Can get messy.

**Recommendation**: Start with **URI Versioning**. It's the standard for public APIs (Stripe, Twitter).

---

## 2. Backward & Forward Compatibility

- **Backward Compatibility**: New code can read old data/communicate with old clients.
  - *Golden Rule*: Never delete a field; only add optional fields.
  - *Example*: Adding `phone_number` to User API is fine. Removing `email` breaks old mobile apps that haven't updated.
- **Forward Compatibility**: Old code can read new data.
  - Old clients should be written to **ignore unknown fields** in JSON (Tolerant Reader pattern).

---

## 3. Feature Flags (Toggles)

Wrapping new code in a conditional statement which can be switched remotely.

```java
if (FeatureFlag.isEnabled("new-checkout-flow")) {
    return renderNewCheckout();
} else {
    return renderLegacyCheckout();
}
```

- **Use Cases**:
  - **Dark Launching**: Deploy code to production turned "off".
  - **Canary Release**: Turn on for 1% of users.
  - **Kill Switch**: If a new feature causes bugs, turn it off instantly without redeploying code.
  - **A/B Testing**: Show Feature A to Group X, Feature B to Group Y.

---

## 4. Cost Optimization (Cloud)

Designing for cost is often a hidden constraint in System Design interviews.

1. **Spot Instances (AWS)**: Use spare capacity servers for stateless workers (e.g., Image processing). 90% cheaper but can be terminated anytime.
2. **Autoscaling**: Scale down to 1 node at night. Don't pay for idle resources.
3. **Storage Tiers**: Move rarely accessed logs/backups from S3 Standard -> S3 Infrequent Access -> Glacier (Cold Storage).
4. **Data Transfer**: Avoid cross-region data transfer if possible (it costs money). Use CDNs to offload egress traffic.
