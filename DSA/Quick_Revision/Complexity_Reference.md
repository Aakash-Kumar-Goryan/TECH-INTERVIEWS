# Complexity Reference

Quick lookup for time/space complexity

---

## Data Structure Operations

| Data Structure | Access | Search | Insert | Delete |
|----------------|--------|--------|--------|--------|
| **Array** | O(1) | O(n) | O(n) | O(n) |
| **Linked List** | O(n) | O(n) | O(1)* | O(1)* |
| **Stack / Queue** | O(n) | O(n) | O(1) | O(1) |
| **Hash Table** | - | O(1) avg | O(1) avg | O(1) avg |
| **BST (balanced)** | O(log n) | O(log n) | O(log n) | O(log n) |
| **Heap** | O(n) | O(n) | O(log n) | O(log n) |

*With pointer to node

---

## Sorting Algorithms

| Algorithm | Best | Average | Worst | Space | Stable |
|-----------|------|---------|-------|-------|--------|
| **Bubble Sort** | O(n) | O(n²) | O(n²) | O(1) | ✅ |
| **Selection Sort** | O(n²) | O(n²) | O(n²) | O(1) | ❌ |
| **Insertion Sort** | O(n) | O(n²) | O(n²) | O(1) | ✅ |
| **Merge Sort** | O(n log n) | O(n log n) | O(n log n) | O(n) | ✅ |
| **Quick Sort** | O(n log n) | O(n log n) | O(n²) | O(log n) | ❌ |
| **Heap Sort** | O(n log n) | O(n log n) | O(n log n) | O(1) | ❌ |
| **Counting Sort** | O(n+k) | O(n+k) | O(n+k) | O(k) | ✅ |
| **Radix Sort** | O(nk) | O(nk) | O(nk) | O(n+k) | ✅ |

---

## Graph Algorithms

| Algorithm | Time | Space |
|-----------|------|-------|
| **BFS / DFS** | O(V+E) | O(V) |
| **Topological Sort** | O(V+E) | O(V) |
| **Dijkstra** (heap) | O(E log V) | O(V) |
| **Bellman-Ford** | O(VE) | O(V) |
| **Floyd-Warshall** | O(V³) | O(V²) |
| **Union Find** | O(α(n)) ≈ O(1) | O(n) |
| **Kruskal's MST** | O(E log E) | O(V) |
| **Prim's MST** (heap) | O(E log V) | O(V) |

---

## Common Patterns

| Pattern | Time | Space |
|---------|------|-------|
| Two Pointers | O(n) | O(1) |
| Sliding Window | O(n) | O(k) |
| Binary Search | O(log n) | O(1) |
| Prefix Sum (build) | O(n) | O(n) |
| Monotonic Stack | O(n) | O(n) |
| Top K (heap) | O(n log k) | O(k) |

---

## DP Patterns

| Pattern | Time | Space |
|---------|------|-------|
| 1D DP | O(n) | O(n) or O(1) |
| 2D DP | O(m×n) | O(m×n) |
| LCS | O(m×n) | O(m×n) |
| 0/1 Knapsack | O(n×W) | O(n×W) |
| LIS (optimized) | O(n log n) | O(n) |

---

## Power of 2 Reference

| Power | Value | Approx |
|-------|-------|--------|
| 2¹⁰ | 1,024 | ~ 1 Thousand |
| 2²⁰ | 1,048,576 | ~ 1 Million |
| 2³⁰ | 1,073,741,824 | ~ 1 Billion |
| 2³² | 4.29 × 10⁹ | ~ 4 Billion (int max) |
| 2⁶⁴ | 1.84 × 10¹⁹ | ~ long max |

---

## Interview Time Limits

| n | O(n²) | O(n log n) | O(n) |
|---|-------|------------|------|
| 10³ | ✅ 1ms | ✅ | ✅ |
| 10⁴ | ✅ 100ms | ✅ | ✅ |
| 10⁵ | ⚠️ 10s | ✅ | ✅ |
| 10⁶ | ❌ | ✅ | ✅ |
| 10⁷ | ❌ | ⚠️ | ✅ |
