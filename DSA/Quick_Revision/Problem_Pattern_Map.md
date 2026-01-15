# Problem → Pattern Map

Quick lookup: "I see X in problem" → "Try Y pattern"

---

## Array Signals

| You See... | Try This Pattern |
|------------|------------------|
| Sorted array + find pair | **Two Pointers** (opposite ends) |
| Contiguous subarray with sum/count constraint | **Sliding Window** |
| Range sum queries, subarray sum = K | **Prefix Sum + HashMap** |
| Maximum subarray sum | **Kadane's Algorithm** |
| Find duplicate in [1,n] | **Floyd's Cycle Detection** on indices |
| Rotate array | **Reverse 3 times** |
| At most K distinct elements | **Sliding Window + HashMap** |

---

## Search Signals

| You See... | Try This Pattern |
|------------|------------------|
| Sorted array, find element | **Binary Search** |
| Find first/last occurrence | **Binary Search Boundary** |
| Minimum X such that condition Y | **Binary Search on Answer** |
| Search in rotated sorted array | **Modified Binary Search** (identify sorted half) |
| Peak element | **Binary Search** (compare mid with mid+1) |

---

## Linked List Signals

| You See... | Try This Pattern |
|------------|------------------|
| Detect cycle | **Fast/Slow Pointers** |
| Find middle | **Fast/Slow Pointers** |
| Cycle start position | **Floyd's** (reset + same speed) |
| Reverse (whole or partial) | **Iterative reversal** (prev, curr, next) |
| Merge two lists | **Dummy node + two pointers** |
| Palindrome check | **Find middle + reverse second half** |

---

## Tree Signals

| You See... | Try This Pattern |
|------------|------------------|
| Level by level processing | **BFS with size** |
| Path from root to leaf | **DFS with path tracking** |
| Validate BST | **DFS with min/max range** |
| Kth smallest in BST | **Inorder traversal + counter** |
| LCA (BST) | **Split point** (use BST property) |
| LCA (Binary Tree) | **Recursive DFS** (null propagation) |
| Serialize/deserialize | **Preorder + queue** |

---

## Graph Signals

| You See... | Try This Pattern |
|------------|------------------|
| Shortest path (unweighted) | **BFS** |
| Shortest path (weighted, positive) | **Dijkstra** |
| Connected components | **DFS/BFS** or **Union Find** |
| Dependencies / ordering | **Topological Sort** |
| Cycle in directed graph | **DFS with 3 colors** |
| Cycle in undirected graph | **Union Find** |
| Grid traversal (islands) | **DFS/BFS** (mark visited) |
| Minimum spanning tree | **Kruskal's + Union Find** |

---

## DP Signals

| You See... | Try This Pattern |
|------------|------------------|
| "Minimum/maximum ways to..." | **DP** |
| "Can you reach...?" | **DP** (boolean) |
| Grid with paths | **2D DP** |
| Take or skip decisions | **1D DP** (robber pattern) |
| Two sequences comparison | **2D DP** (LCS pattern) |
| Subset with target sum | **0/1 Knapsack** |
| Unlimited items | **Unbounded Knapsack** |

---

## Stack/Queue Signals

| You See... | Try This Pattern |
|------------|------------------|
| Next greater/smaller element | **Monotonic Stack** |
| Valid parentheses | **Stack** |
| Largest rectangle in histogram | **Monotonic Stack** |
| Sliding window maximum | **Monotonic Deque** |

---

## Heap Signals

| You See... | Try This Pattern |
|------------|------------------|
| K largest/smallest | **Min/Max Heap of size K** |
| K most frequent | **Heap with freq pairs** |
| Merge K sorted | **Min Heap** |
| Running median | **Two Heaps** (max + min) |

---

## Backtracking Signals

| You See... | Try This Pattern |
|------------|------------------|
| Generate all subsets | **Backtracking** (include/exclude) |
| Generate all permutations | **Backtracking** (swap) |
| Combination sum to target | **Backtracking** (with pruning) |
| N-Queens, Sudoku | **Backtracking** (row by row) |
