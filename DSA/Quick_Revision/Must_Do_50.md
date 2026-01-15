# Google L5 Must-Do 50 Problems

> **Goal**: Master these 50 problems for L5-level pattern recognition.  
> **Strategy**: Solve each in 25 min, then optimize. Explain trade-offs aloud.

---

## 🔴 Arrays & Hashing (10)

| # | Problem | Pattern | Key Insight |
|---|---------|---------|-------------|
| 1 | [LC 1 - Two Sum](https://leetcode.com/problems/two-sum/) | HashMap | Complement lookup O(n) |
| 2 | [LC 49 - Group Anagrams](https://leetcode.com/problems/group-anagrams/) | Hashing | Sorted string as key |
| 3 | [LC 128 - Longest Consecutive](https://leetcode.com/problems/longest-consecutive-sequence/) | HashSet | Start from sequence head |
| 4 | [LC 238 - Product Except Self](https://leetcode.com/problems/product-of-array-except-self/) | Prefix/Suffix | Left-right products |
| 5 | [LC 42 - Trapping Rain Water](https://leetcode.com/problems/trapping-rain-water/) | Two Pointers | Track max left/right |
| 6 | [LC 239 - Sliding Window Max](https://leetcode.com/problems/sliding-window-maximum/) | Monotonic Deque | Maintain decreasing order |
| 7 | [LC 76 - Min Window Substring](https://leetcode.com/problems/minimum-window-substring/) | Sliding Window | Two hashmaps, shrink when valid |
| 8 | [LC 56 - Merge Intervals](https://leetcode.com/problems/merge-intervals/) | Intervals | Sort by start, merge |
| 9 | [LC 253 - Meeting Rooms II](https://leetcode.com/problems/meeting-rooms-ii/) | Sweep Line | Start/end events |
| 10 | [LC 295 - Find Median](https://leetcode.com/problems/find-median-from-data-stream/) | Two Heaps | Max + Min heap |

---

## 🔴 Trees (8)

| # | Problem | Pattern | Key Insight |
|---|---------|---------|-------------|
| 11 | [LC 236 - LCA Binary Tree](https://leetcode.com/problems/lowest-common-ancestor-of-a-binary-tree/) | DFS | Null propagation |
| 12 | [LC 124 - Max Path Sum](https://leetcode.com/problems/binary-tree-maximum-path-sum/) | Tree DP | Global max, return single path |
| 13 | [LC 297 - Serialize Tree](https://leetcode.com/problems/serialize-and-deserialize-binary-tree/) | Preorder | Null markers |
| 14 | [LC 98 - Validate BST](https://leetcode.com/problems/validate-binary-search-tree/) | DFS | Pass min/max range |
| 15 | [LC 230 - Kth Smallest BST](https://leetcode.com/problems/kth-smallest-element-in-a-bst/) | Inorder | Counter during traversal |
| 16 | [LC 105 - Construct from Traversals](https://leetcode.com/problems/construct-binary-tree-from-preorder-and-inorder-traversal/) | Divide & Conquer | HashMap for inorder index |
| 17 | [LC 543 - Diameter](https://leetcode.com/problems/diameter-of-binary-tree/) | DFS | Left depth + right depth |
| 18 | [LC 987 - Vertical Order](https://leetcode.com/problems/vertical-order-traversal-of-a-binary-tree/) | BFS + Sort | (col, row, val) ordering |

---

## 🔴 Graphs (10)

| # | Problem | Pattern | Key Insight |
|---|---------|---------|-------------|
| 19 | [LC 200 - Number of Islands](https://leetcode.com/problems/number-of-islands/) | DFS/BFS | Mark visited |
| 20 | [LC 133 - Clone Graph](https://leetcode.com/problems/clone-graph/) | DFS + HashMap | Track cloned nodes |
| 21 | [LC 207 - Course Schedule](https://leetcode.com/problems/course-schedule/) | Topological Sort | Cycle = impossible |
| 22 | [LC 210 - Course Schedule II](https://leetcode.com/problems/course-schedule-ii/) | Kahn's Algorithm | Return order |
| 23 | [LC 127 - Word Ladder](https://leetcode.com/problems/word-ladder/) | BFS | Level = transformations |
| 24 | [LC 743 - Network Delay](https://leetcode.com/problems/network-delay-time/) | Dijkstra | Weighted shortest path |
| 25 | [LC 785 - Is Bipartite](https://leetcode.com/problems/is-graph-bipartite/) | BFS Coloring | Two colors |
| 26 | [LC 684 - Redundant Connection](https://leetcode.com/problems/redundant-connection/) | Union Find | First edge creating cycle |
| 27 | [LC 269 - Alien Dictionary](https://leetcode.com/problems/alien-dictionary/) | Topo Sort | Build graph from words |
| 28 | [LC 847 - Shortest Path All Nodes](https://leetcode.com/problems/shortest-path-visiting-all-nodes/) | BFS + Bitmask | State = (mask, node) |

---

## 🔴 Dynamic Programming (12)

| # | Problem | Pattern | Key Insight |
|---|---------|---------|-------------|
| 29 | [LC 70 - Climbing Stairs](https://leetcode.com/problems/climbing-stairs/) | 1D DP | Fibonacci |
| 30 | [LC 198 - House Robber](https://leetcode.com/problems/house-robber/) | 1D DP | Take/skip |
| 31 | [LC 300 - LIS](https://leetcode.com/problems/longest-increasing-subsequence/) | Binary Search | O(n log n) with patience sort |
| 32 | [LC 1143 - LCS](https://leetcode.com/problems/longest-common-subsequence/) | 2D DP | Match → diagonal+1 |
| 33 | [LC 72 - Edit Distance](https://leetcode.com/problems/edit-distance/) | 2D DP | Insert/delete/replace |
| 34 | [LC 322 - Coin Change](https://leetcode.com/problems/coin-change/) | Unbounded Knapsack | Min coins |
| 35 | [LC 416 - Partition Equal Subset](https://leetcode.com/problems/partition-equal-subset-sum/) | 0/1 Knapsack | Target = sum/2 |
| 36 | [LC 312 - Burst Balloons](https://leetcode.com/problems/burst-balloons/) | Interval DP | Last balloon in range |
| 37 | [LC 1000 - Merge Stones](https://leetcode.com/problems/minimum-cost-to-merge-stones/) | Interval DP | k-way constraint |
| 38 | [LC 1125 - Smallest Team](https://leetcode.com/problems/smallest-sufficient-team/) | Bitmask DP | Set cover |
| 39 | [LC 139 - Word Break](https://leetcode.com/problems/word-break/) | 1D DP | Can form prefix |
| 40 | [LC 91 - Decode Ways](https://leetcode.com/problems/decode-ways/) | 1D DP | 1-digit or 2-digit |

---

## 🔴 Binary Search & Others (5)

| # | Problem | Pattern | Key Insight |
|---|---------|---------|-------------|
| 41 | [LC 33 - Search Rotated Array](https://leetcode.com/problems/search-in-rotated-sorted-array/) | Binary Search | Identify sorted half |
| 42 | [LC 875 - Koko Eating Bananas](https://leetcode.com/problems/koko-eating-bananas/) | Binary Search on Answer | Feasibility check |
| 43 | [LC 84 - Largest Rectangle](https://leetcode.com/problems/largest-rectangle-in-histogram/) | Monotonic Stack | Width = right - left - 1 |
| 44 | [LC 22 - Generate Parentheses](https://leetcode.com/problems/generate-parentheses/) | Backtracking | open < n, close < open |
| 45 | [LC 46 - Permutations](https://leetcode.com/problems/permutations/) | Backtracking | Swap-based |

---

## 🔴 Design (5)

| # | Problem | Pattern | Key Insight |
|---|---------|---------|-------------|
| 46 | [LC 146 - LRU Cache](https://leetcode.com/problems/lru-cache/) | HashMap + DLL | O(1) get/put |
| 47 | [LC 460 - LFU Cache](https://leetcode.com/problems/lfu-cache/) | HashMap + Freq DLLs | Track min frequency |
| 48 | [LC 380 - Insert Delete Random](https://leetcode.com/problems/insert-delete-getrandom-o1/) | HashMap + Array | Swap to end for delete |
| 49 | [LC 981 - Time Based KV Store](https://leetcode.com/problems/time-based-key-value-store/) | HashMap + Binary Search | Find latest timestamp |
| 50 | [LC 208 - Implement Trie](https://leetcode.com/problems/implement-trie-prefix-tree/) | Trie | Prefix search O(m) |

---

## 📊 Study Strategy

1. **Week 1**: Arrays #1-10
2. **Week 2**: Trees #11-18
3. **Week 3**: Graphs #19-28
4. **Week 4**: DP #29-40
5. **Week 5**: Binary Search + Design #41-50
6. **Week 6-8**: Revisit weak areas + mock interviews

---

## ✅ Tracking

```
Arrays:     □□□□□□□□□□ (0/10)
Trees:      □□□□□□□□ (0/8)
Graphs:     □□□□□□□□□□ (0/10)
DP:         □□□□□□□□□□□□ (0/12)
Others:     □□□□□□□□□□ (0/10)
```
