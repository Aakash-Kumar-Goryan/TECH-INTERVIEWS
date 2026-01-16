# Google L5 DSA Quick Reference

> Based on successful Google L5 interview patterns: **backtracking + memoization**, **sliding window**, **topological sort**

---

## High-Value Patterns for L5

### 1. Backtracking + Memoization (Caching)

**When**: Exponential backtracking with overlapping subproblems

| Problem | State to Cache | Complexity |
|---------|---------------|------------|
| Word Break | start index | O(n²) |
| Target Sum | (index, sum) | O(n × target) |
| Palindrome Partitioning II | start index | O(n²) |
| Partition to K Equal Subsets | bitmask | O(n × 2^n) |

**Template**:

```cpp
unordered_map<State, Result> memo;
Result solve(state) {
    if (base) return result;
    if (memo.count(state)) return memo[state];
    // explore choices
    return memo[state] = result;
}
```

---

### 2. Sliding Window

**When**: Contiguous subarray/substring optimization

| Variant | Trigger | Template |
|---------|---------|----------|
| Variable window | Max/min subarray with constraint | Expand right, shrink left |
| Fixed window | Exact size K | Slide by 1 |
| At most K → Exactly K | Count with exact constraint | f(K) - f(K-1) |

**Must-Know Problems**: LC 3, 76, 239, 424, 567

---

### 3. Topological Sort

**When**: Dependencies, ordering, cycle detection in DAG

| Approach | Use Case |
|----------|----------|
| Kahn's (BFS) | Need cycle detection + level order |
| DFS | Simpler, any valid order |

**Must-Know Problems**: LC 207, 210, 269

---

## Pattern Recognition Triggers

| See This | Think This |
|----------|------------|
| "All combinations/permutations" | Backtracking |
| "Minimum cuts/partitions" | Backtracking + Memo or DP |
| "Contiguous subarray" | Sliding Window or Prefix Sum |
| "Prerequisites/dependencies" | Topological Sort |
| "Shortest path (unweighted)" | BFS |
| "K largest/smallest" | Heap |
| "Range queries + updates" | Segment Tree |
| "Dynamic connectivity" | Union Find |

---

## L5 Complexity Expectations

| Category | Expected | Stretch Goal |
|----------|----------|--------------|
| Easy | < 5 min | Optimal first try |
| Medium | < 15 min | Discuss alternatives |
| Hard | < 30 min | Working solution + optimization path |

---

## Common L5 Coding Patterns

- [ ] Two Pointers (sorted array, palindrome)
- [ ] Sliding Window (variable + fixed)
- [ ] Binary Search on Answer
- [ ] BFS/DFS (graphs + trees)
- [ ] Topological Sort (Kahn's + DFS)
- [ ] Union Find with rank/path compression
- [ ] Backtracking (subsets, permutations)
- [ ] Backtracking + Memoization
- [ ] DP (1D, 2D, knapsack)
- [ ] Monotonic Stack (next greater)
- [ ] Heap (top-K, two heaps)

---

## Whiteboard Tips

1. **Clarify first** — bounds, edge cases, expected complexity
2. **State approach before coding** — "I'll use sliding window because..."
3. **Write clean code** — variable names matter, no magic numbers
4. **Trace through example** — show it works
5. **Analyze complexity** — time AND space
6. **Discuss trade-offs** — "We could also..."

---

## Quick Complexity Reference

| Pattern | Time | Space |
|---------|------|-------|
| Two Pointers | O(n) | O(1) |
| Sliding Window | O(n) | O(k) |
| Binary Search | O(log n) | O(1) |
| BFS/DFS | O(V+E) | O(V) |
| Topological Sort | O(V+E) | O(V) |
| Backtracking | O(2^n) or O(n!) | O(n) |
| Backtracking + Memo | O(n × states) | O(states) |
| DP | O(n × m) | O(n × m) |
| Heap (Top K) | O(n log k) | O(k) |
| Union Find | O(α(n)) | O(n) |

---

## L5 Must-Practice Problems

| Pattern | Problems |
|---------|----------|
| Backtracking + Memo | LC 139, 140, 494, 416, 132 |
| Sliding Window | LC 3, 76, 239, 424, 992 |
| Topological Sort | LC 207, 210, 269, 310 |
| DP Hard | LC 72, 312, 1235, 1547 |
| Graph Hard | LC 332, 743, 787, 1631 |
