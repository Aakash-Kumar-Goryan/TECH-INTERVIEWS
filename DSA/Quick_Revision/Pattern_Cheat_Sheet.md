# DSA Pattern Cheat Sheet

Quick lookup: Pattern name → When to use → Complexity

---

## Arrays & Strings

| Pattern | Trigger | Time |
|---------|---------|------|
| **Two Pointers** | Sorted array, pair finding, palindrome | O(n) |
| **Sliding Window** | Contiguous subarray with constraint | O(n) |
| **Prefix Sum** | Range sum queries, subarray sum = K | O(n) |
| **Kadane's** | Maximum subarray sum | O(n) |
| **Dutch Flag** | Partition into 3 groups (0,1,2) | O(n) |

---

## Binary Search

| Pattern | Trigger | Time |
|---------|---------|------|
| **Classic** | Sorted array, find exact | O(log n) |
| **Boundary** | First/last occurrence, lower/upper bound | O(log n) |
| **On Answer** | Minimize/maximize feasible value | O(log n × cost) |

---

## Linked Lists

| Pattern | Trigger | Time |
|---------|---------|------|
| **Fast/Slow** | Cycle, middle, start of cycle | O(n) |
| **Reversal** | Reverse list, palindrome check | O(n) |
| **Dummy Node** | Insert/delete at head edge cases | O(n) |

---

## Trees

| Pattern | Trigger | Time |
|---------|---------|------|
| **BFS (Level Order)** | Level by level, shortest in tree | O(n) |
| **DFS (Recursive)** | Path problems, subtree queries | O(n) |
| **BST Property** | Search/insert O(log n), inorder = sorted | O(h) |

---

## Graphs

| Pattern | Trigger | Time |
|---------|---------|------|
| **BFS** | Shortest path (unweighted), levels | O(V+E) |
| **DFS** | Traversal, connected components | O(V+E) |
| **Topological Sort** | Dependencies, course schedule | O(V+E) |
| **Union Find** | Dynamic connectivity, cycle detection | O(α(n)) |
| **Dijkstra** | Shortest path (weighted, positive) | O(E log V) |

---

## Dynamic Programming

| Pattern | Trigger | Time |
|---------|---------|------|
| **1D DP** | Linear decisions (take/skip) | O(n) |
| **2D DP** | Grid paths, two sequences | O(m×n) |
| **Knapsack** | Subset with constraint optimization | O(n×W) |
| **LCS/LIS** | Sequence comparison, subsequence | O(n²) or O(n log n) |

---

## Stacks & Queues

| Pattern | Trigger | Time |
|---------|---------|------|
| **Monotonic Stack** | Next greater/smaller, histogram | O(n) |
| **Valid Parentheses** | Matching brackets, expression eval | O(n) |

---

## Heaps

| Pattern | Trigger | Time |
|---------|---------|------|
| **Top K** | K largest/smallest/frequent | O(n log k) |
| **K-way Merge** | Merge K sorted lists | O(n log k) |
| **Two Heaps** | Running median, schedule by two criteria | O(log n) |

---

## Backtracking

| Pattern | Trigger | Time |
|---------|---------|------|
| **Subsets** | All combinations, power set | O(2ⁿ) |
| **Permutations** | All arrangements | O(n!) |
| **N-Queens** | Constraint satisfaction, board placement | O(n!) |

---

## Advanced (Google L5)

| Pattern | Trigger | Time |
|---------|---------|------|
| **DP Bitmask** | n ≤ 20, subset states, TSP | O(n² × 2ⁿ) |
| **DP Intervals** | Range merge/split, burst balloons | O(n³) |
| **Segment Tree** | Range query + point update | O(log n) |
| **LRU Cache** | Design cache, O(1) get/put | O(1) |
| **Bipartite** | Two-colorable, graph partition | O(V+E) |
