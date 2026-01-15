# Why This Approach - Quick Reference

> **For L5 interviews**: You must explain WHY you chose a pattern, not just WHAT you're doing.

---

## Arrays & Hashing

| Pattern | Why This | Why NOT Alternatives |
|---------|----------|---------------------|
| **Two Pointers** | O(n) single pass, O(1) space | Brute force O(n²) too slow |
| **Sliding Window** | Maintains running state O(n) | Recomputing each window O(n×k) |
| **Prefix Sum** | O(1) range queries after O(n) build | Recomputing sums O(n) per query |
| **HashMap** | O(1) lookup vs O(n) linear search | Sorting O(n log n) if not needed |

---

## Binary Search

| Pattern | Why This | Why NOT Alternatives |
|---------|----------|---------------------|
| **Classic BS** | O(log n) on sorted data | Linear O(n) too slow for large n |
| **BS on Answer** | Search space too large for iteration | Brute force O(range) infeasible |
| **Boundary BS** | Find exact position, not just existence | Classic BS stops at any match |

---

## Trees

| Pattern | Why This | Why NOT Alternatives |
|---------|----------|---------------------|
| **DFS (Recursive)** | Natural for path problems, subtrees | BFS uses more memory for deep trees |
| **BFS (Level Order)** | Level-by-level, shortest path in tree | DFS doesn't track levels naturally |
| **BST Property** | O(log n) search/insert | Regular tree O(n) search |

---

## Graphs

| Pattern | Why This | Why NOT Alternatives |
|---------|----------|---------------------|
| **BFS** | Shortest path (unweighted), layer-by-layer | DFS doesn't guarantee shortest |
| **DFS** | Path existence, cycle detection, backtracking | BFS uses more memory |
| **Dijkstra** | Weighted shortest path (positive) | BFS ignores weights |
| **Topo Sort** | Dependency ordering, course scheduling | BFS/DFS don't preserve order |
| **Union Find** | Dynamic connectivity, O(α(n)) updates | BFS/DFS O(V+E) each query |

---

## Dynamic Programming

| Pattern | Why This | Why NOT Alternatives |
|---------|----------|---------------------|
| **1D DP** | Optimal substructure, overlapping subproblems | Recursion without memo = exponential |
| **2D DP** | Two dimensions of choice (grid, two strings) | 1D can't capture both dimensions |
| **Knapsack** | Bounded/unbounded selection with capacity | Greedy doesn't guarantee optimal |
| **Bitmask DP** | n ≤ 20, need exact subset state | Regular DP can't track set membership |
| **Interval DP** | Optimal merge/split of ranges | Greedy doesn't work for order-dependent |

---

## Stacks & Queues

| Pattern | Why This | Why NOT Alternatives |
|---------|----------|---------------------|
| **Monotonic Stack** | O(n) for next greater/smaller | Brute force O(n²) |
| **Two Stacks** | LIFO ↔ FIFO conversion | Direct queue doesn't support needed ops |

---

## Heaps

| Pattern | Why This | Why NOT Alternatives |
|---------|----------|---------------------|
| **Min/Max Heap** | O(log n) insert/extract, O(1) peek | Sorted array O(n) insert |
| **Two Heaps** | O(log n) median updates | Sorting O(n log n) each query |
| **Top K with Min Heap** | O(n log k), only track K items | Sorting entire O(n log n) |

---

## Common Trade-off Discussions

### Time vs Space

| Approach | Time | Space | When to Choose |
|----------|------|-------|----------------|
| HashMap lookup | O(1) | O(n) | Speed critical, memory available |
| Binary Search | O(log n) | O(1) | Memory constrained, data sorted |
| Two Pointers | O(n) | O(1) | Best of both if applicable |

### Recursion vs Iteration

| Approach | Pros | Cons |
|----------|------|------|
| Recursion | Cleaner, natural for trees | Stack overflow, function call overhead |
| Iteration | No stack overflow, faster | More complex code |

### DFS vs BFS

| Use DFS | Use BFS |
|---------|---------|
| Path finding, backtracking | Shortest path (unweighted) |
| Deep exploration | Level-by-level processing |
| Less memory (narrow trees) | Better for wide/shallow graphs |

---

## Interview Phrases to Use

- "I'm using X because it gives O(Y) which is optimal for this constraint..."
- "I considered Z but it would be O(W) which is too slow for n = 10^5..."
- "The trade-off here is space for time: we use O(n) extra space to get O(1) lookups..."
- "This greedy approach works because of the optimal substructure property..."
