# Algorithm Templates — Interview Ready

> Clean, readable boilerplate for whiteboard coding.

## Index

| # | Topic | Key Algorithms |
|---|-------|----------------|
| 1 | **Arrays & Search** | Binary Search, Sliding Window, QuickSelect |
| 2 | **Trees** | DFS (Traversals), BFS (Level Order), LCA |
| 3 | **Graphs** | DFS/BFS, Cycle, Union-Find, Topo Sort, Dijkstra, Bellman-Ford |
| 4 | **Trie** | Insert, Search, StartsWith |
| 5 | **String Algos** | KMP, Rolling Hash |
| 6 | **Backtracking** | Subsets, Permutations |
| 7 | **DP Patterns** | House Robber, Grid Paths, Knapsack, LIS, LCS |
| 8 | **Utilities** | Prefix Sum, Monotonic Stack |
| 9 | **Segment Tree** | Range Sum + Point Update |
| 10| **Floyd-Warshall**| All Pairs Shortest Path |
| 11| **LRU Cache** | HashMap + DLL |
| 12| **Intervals** | Merge, Insert |

---

## 1. Arrays & Search

### Binary Search — Standard

```java
public int binarySearch(int[] nums, int target) {
    int left = 0, right = nums.length - 1;
    
    while (left <= right) {
        int mid = left + (right - left) / 2;
        
        if (nums[mid] == target) {
            return mid;
        } else if (nums[mid] < target) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
    return -1;
}
```

### Binary Search — Lower Bound (First >= target)

```java
public int lowerBound(int[] nums, int target) {
    int left = 0, right = nums.length;
    
    while (left < right) {
        int mid = left + (right - left) / 2;
        if (nums[mid] >= target) {
            right = mid;
        } else {
            left = mid + 1;
        }
    }
    return left;
}
```

### Sliding Window — Variable Size

```java
public int slidingWindow(int[] nums) {
    int left = 0, ans = 0;
    // state variables here (e.g., sum, map)
    
    for (int right = 0; right < nums.length; right++) {
        // 1. Add nums[right] to window state
        
        // 2. Shrink window while condition is broken
        while (/* window invalid */) {
            // Remove nums[left] from state
            left++;
        }
        
        // 3. Update answer
        ans = Math.max(ans, right - left + 1);
    }
    return ans;
}
```

### QuickSelect (Kth Smallest Element)

```java
// Returns the k-th smallest element (0-indexed: k=0 is min)
// Average: O(N), Worst: O(N^2)
public int quickSelect(int[] nums, int k) {
    int l = 0, r = nums.length - 1;
    while (l <= r) {
        int pivot = nums[r];
        int p = l;
        for (int i = l; i < r; i++) {
            if (nums[i] <= pivot) {
                swap(nums, i, p);
                p++;
            }
        }
        swap(nums, p, r);
        
        if (p < k) l = p + 1;
        else if (p > k) r = p - 1;
        else return nums[p];
    }
    return -1;
}

private void swap(int[] nums, int i, int j) {
    int temp = nums[i];
    nums[i] = nums[j];
    nums[j] = temp;
}
```

---

## 2. Trees

### DFS — Inorder/Preorder/Postorder

```java
public void inorder(TreeNode root) {
    if (root == null) return;
    
    inorder(root.left);      // Left
    process(root.val);       // Process (move for pre/post)
    inorder(root.right);     // Right
}
// Preorder: Process, Left, Right
// Postorder: Left, Right, Process
```

### BFS — Level Order

```java
public void levelOrder(TreeNode root) {
    if (root == null) return;
    
    Queue<TreeNode> q = new LinkedList<>();
    q.offer(root);
    
    while (!q.isEmpty()) {
        int levelSize = q.size();
        
        for (int i = 0; i < levelSize; i++) {
            TreeNode node = q.poll();
            
            // Process node
            
            if (node.left != null) q.offer(node.left);
            if (node.right != null) q.offer(node.right);
        }
    }
    return;
}
```

### LCA (Lowest Common Ancestor)

```java
public TreeNode lowestCommonAncestor(TreeNode root, TreeNode p, TreeNode q) {
    if (root == null || root == p || root == q) return root;
    
    TreeNode left = lowestCommonAncestor(root.left, p, q);
    TreeNode right = lowestCommonAncestor(root.right, p, q);
    
    if (left != null && right != null) return root; // Found one in each subtree
    return left != null ? left : right;     // Both in one subtree (or neither found)
}
```

---

## 3. Graphs

### DFS — Basic

```java
List<List<Integer>> adj;
boolean[] visited;

public void dfs(int node) {
    visited[node] = true;
    
    // Process node here
    
    for (int neighbor : adj.get(node)) {
        if (!visited[neighbor]) {
            dfs(neighbor);
        }
    }
}
```

### BFS — Shortest Path (Unweighted)

```java
public int[] bfs(int start, int n) {
    int[] dist = new int[n];
    Arrays.fill(dist, -1);
    Queue<Integer> q = new LinkedList<>();
    
    dist[start] = 0;
    q.offer(start);
    
    while (!q.isEmpty()) {
        int node = q.poll();
        
        for (int neighbor : adj.get(node)) {
            if (dist[neighbor] == -1) {
                dist[neighbor] = dist[node] + 1;
                q.offer(neighbor);
            }
        }
    }
    return dist;
}
```

### Matrix Traversal (BFS)

```java
int R, C;
int[] dr = {-1, 1, 0, 0}; // Up, Down, Left, Right
int[] dc = {0, 0, -1, 1};

public void bfsGrid(int r, int c, int[][] grid) {
    Queue<int[]> q = new LinkedList<>();
    q.offer(new int[]{r, c});
    grid[r][c] = -1; // Mark visited (or use visited array)
    
    while (!q.isEmpty()) {
        int[] curr = q.poll();
        int cr = curr[0], cc = curr[1];
        
        for (int i = 0; i < 4; i++) {
            int nr = cr + dr[i];
            int nc = cc + dc[i];
            
            if (nr >= 0 && nr < R && nc >= 0 && nc < C && grid[nr][nc] != -1) {
                grid[nr][nc] = -1; 
                q.offer(new int[]{nr, nc});
            }
        }
    }
}
```

### Cycle Detection — Directed Graph (3-color DFS)

```java
// 0 = unvisited, 1 = in current path, 2 = processed
int[] color;

public boolean hasCycle(int node) {
    color[node] = 1;  // Mark as "in progress"
    
    for (int neighbor : adj.get(node)) {
        if (color[neighbor] == 1) {
            return true;  // Back edge found → cycle
        }
        if (color[neighbor] == 0 && hasCycle(neighbor)) {
            return true;
        }
    }
    
    color[node] = 2;  // Mark as "done"
    return false;
}
```

### Cycle Detection — Undirected Graph (DFS with parent)

```java
public boolean hasCycle(int node, int parent) {
    visited[node] = true;
    
    for (int neighbor : adj.get(node)) {
        if (!visited[neighbor]) {
            if (hasCycle(neighbor, node)) {
                return true;
            }
        } else if (neighbor != parent) {
            return true;  // Visited and not parent → cycle
        }
    }
    return false;
}
```

### Union-Find (Disjoint Set Union)

```java
class DSU {
    int[] parent, size;
    
    public DSU(int n) {
        parent = new int[n];
        size = new int[n];
        for (int i = 0; i < n; i++) {
            parent[i] = i;
            size[i] = 1;
        }
    }
    
    public int find(int x) {
        if (parent[x] != x) {
            parent[x] = find(parent[x]);  // Path compression
        }
        return parent[x];
    }
    
    public boolean unite(int x, int y) {
        int rootX = find(x);
        int rootY = find(y);
        
        if (rootX == rootY) return false;  // Already connected
        
        // Union by size
        if (size[rootX] < size[rootY]) {
            int temp = rootX;
            rootX = rootY;
            rootY = temp;
        }
        parent[rootY] = rootX;
        size[rootX] += size[rootY];
        return true;
    }
}
```

### Topological Sort — Kahn's Algorithm (BFS)

```java
public List<Integer> topoSort(int n, List<List<Integer>> adj) {
    int[] indegree = new int[n];
    
    // Calculate indegrees
    for (int u = 0; u < n; u++) {
        for (int v : adj.get(u)) {
            indegree[v]++;
        }
    }
    
    // Push all nodes with indegree 0
    Queue<Integer> q = new LinkedList<>();
    for (int i = 0; i < n; i++) {
        if (indegree[i] == 0) q.offer(i);
    }
    
    List<Integer> result = new ArrayList<>();
    while (!q.isEmpty()) {
        int node = q.poll();
        result.add(node);
        
        for (int neighbor : adj.get(node)) {
            indegree[neighbor]--;
            if (indegree[neighbor] == 0) {
                q.offer(neighbor);
            }
        }
    }
    
    // If result size != n, there's a cycle
    return result.size() == n ? result : new ArrayList<>();
}
```

### Dijkstra — Shortest Path (Weighted)

```java
public int[] dijkstra(int start, int n, List<List<int[]>> adj) {
    int[] dist = new int[n];
    Arrays.fill(dist, Integer.MAX_VALUE);
    PriorityQueue<int[]> pq = new PriorityQueue<>((a, b) -> a[0] - b[0]);
    
    dist[start] = 0;
    pq.offer(new int[]{0, start});  // {distance, node}
    
    while (!pq.isEmpty()) {
        int[] curr = pq.poll();
        int d = curr[0], u = curr[1];
        
        if (d > dist[u]) continue;  // Skip outdated entries
        
        for (int[] edge : adj.get(u)) {
            int v = edge[0], weight = edge[1];
            if (dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                pq.offer(new int[]{dist[v], v});
            }
        }
    }
    return dist;
}
```

### Bellman-Ford — Shortest Path (Negative Weights)

```java
// Returns null if negative cycle exists
public int[] bellmanFord(int start, int n, int[][] edges) {
    int[] dist = new int[n];
    Arrays.fill(dist, Integer.MAX_VALUE);
    dist[start] = 0;
    
    // Relax all edges (n-1) times
    for (int i = 0; i < n - 1; i++) {
        for (int[] edge : edges) {
            int u = edge[0], v = edge[1], weight = edge[2];
            if (dist[u] != Integer.MAX_VALUE && dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
            }
        }
    }
    
    // Check for negative cycle (one more relaxation)
    for (int[] edge : edges) {
        int u = edge[0], v = edge[1], weight = edge[2];
        if (dist[u] != Integer.MAX_VALUE && dist[u] + weight < dist[v]) {
            // Negative cycle detected
            return null;
        }
    }
    return dist;
}
```

---

## 4. Trie (Prefix Tree)

```java
class TrieNode {
    TrieNode[] children;
    boolean isEnd;
    
    public TrieNode() {
        children = new TrieNode[26];
        isEnd = false;
    }
}

class Trie {
    TrieNode root;
    
    public Trie() {
        root = new TrieNode();
    }
    
    public void insert(String word) {
        TrieNode node = root;
        for (char c : word.toCharArray()) {
            int idx = c - 'a';
            if (node.children[idx] == null) {
                node.children[idx] = new TrieNode();
            }
            node = node.children[idx];
        }
        node.isEnd = true;
    }
    
    public boolean search(String word) {
        TrieNode node = root;
        for (char c : word.toCharArray()) {
            int idx = c - 'a';
            if (node.children[idx] == null) {
                return false;
            }
            node = node.children[idx];
        }
        return node.isEnd;
    }
    
    public boolean startsWith(String prefix) {
        TrieNode node = root;
        for (char c : prefix.toCharArray()) {
            int idx = c - 'a';
            if (node.children[idx] == null) {
                return false;
            }
            node = node.children[idx];
        }
        return true;
    }
}
```

---

## 5. String Algorithms

### KMP Algorithm (Pattern Matching)

```java
// Returns starting indices of all occurrences of pat in txt
public int[] computeLPS(String pat) {
    int m = pat.length();
    int[] lps = new int[m];
    int len = 0, i = 1;
    
    while (i < m) {
        if (pat.charAt(i) == pat.charAt(len)) {
            len++;
            lps[i] = len;
            i++;
        } else {
            if (len != 0) {
                len = lps[len - 1];
            } else {
                lps[i] = 0;
                i++;
            }
        }
    }
    return lps;
}

public List<Integer> KMP(String txt, String pat) {
    int n = txt.length(), m = pat.length();
    int[] lps = computeLPS(pat);
    List<Integer> result = new ArrayList<>();
    int i = 0, j = 0;
    
    while (i < n) {
        if (pat.charAt(j) == txt.charAt(i)) {
            j++;
            i++;
        }
        
        if (j == m) {
            result.add(i - j);
            j = lps[j - 1];
        } else if (i < n && pat.charAt(j) != txt.charAt(i)) {
            if (j != 0) j = lps[j - 1];
            else i++;
        }
    }
    return result;
}
```

### Rolling Hash (Rabin-Karp)

```java
// Simple version for single pattern search (modulo 1e9+7)
public long computeHash(String s) {
    long p = 31, m = (long)1e9 + 9;
    long hashValue = 0, pPow = 1;
    for (char c : s.toCharArray()) {
        hashValue = (hashValue + (c - 'a' + 1) * pPow) % m;
        pPow = (pPow * p) % m;
    }
    return hashValue;
}
```

---

## 6. Backtracking

### General Template

```java
List<List<Integer>> result = new ArrayList<>();
int[] nums;

public void backtrack(int index, List<Integer> path) {
    // Base case
    if (/* reached goal */) {
        result.add(new ArrayList<>(path));
        return;
    }
    
    for (int i = index; i < nums.length; i++) {
        // Skip invalid choices (pruning)
        if (!isValid(i)) continue;
        
        path.add(nums[i]);           // Choose
        backtrack(i + 1, path);      // Explore (use i for allowing repeats)
        path.remove(path.size() - 1); // Unchoose
    }
}
```

### Subsets

```java
public void subsets(int index, List<Integer> current) {
    result.add(new ArrayList<>(current));  // Every state is a valid subset
    
    for (int i = index; i < nums.length; i++) {
        current.add(nums[i]);
        subsets(i + 1, current);
        current.remove(current.size() - 1);
    }
}
```

### Permutations

```java
public void permute(List<Integer> current, boolean[] used) {
    if (current.size() == nums.length) {
        result.add(new ArrayList<>(current));
        return;
    }
    
    for (int i = 0; i < nums.length; i++) {
        if (used[i]) continue;
        
        used[i] = true;
        current.add(nums[i]);
        
        permute(current, used);
        
        current.remove(current.size() - 1);
        used[i] = false;
    }
}
```

---

## 7. DP Patterns

### 1D DP — House Robber Style

```java
// dp[i] = max value considering elements 0..i
public int rob(int[] nums) {
    int prev2 = 0, prev1 = 0;
    
    for (int num : nums) {
        int current = Math.max(prev1, prev2 + num);
        prev2 = prev1;
        prev1 = current;
    }
    return prev1;
}
```

### 2D DP — Grid Paths

```java
public int uniquePaths(int m, int n) {
    int[][] dp = new int[m][n];
    for (int[] row : dp) Arrays.fill(row, 1);
    
    for (int i = 1; i < m; i++) {
        for (int j = 1; j < n; j++) {
            dp[i][j] = dp[i-1][j] + dp[i][j-1];
        }
    }
    return dp[m-1][n-1];
}
```

### LCS (Longest Common Subsequence)

```java
public int longestCommonSubsequence(String text1, String text2) {
    int m = text1.length(), n = text2.length();
    int[][] dp = new int[m + 1][n + 1];
    
    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= n; j++) {
            if (text1.charAt(i-1) == text2.charAt(j-1)) {
                dp[i][j] = 1 + dp[i-1][j-1];
            } else {
                dp[i][j] = Math.max(dp[i-1][j], dp[i][j-1]);
            }
        }
    }
    return dp[m][n];
}
```

### 0/1 Knapsack

```java
public int knapsack(int[] weights, int[] values, int capacity) {
    int[] dp = new int[capacity + 1];
    
    for (int i = 0; i < weights.length; i++) {
        for (int w = capacity; w >= weights[i]; w--) {  // Reverse!
            dp[w] = Math.max(dp[w], dp[w - weights[i]] + values[i]);
        }
    }
    return dp[capacity];
}
```

### LIS — O(n log n)

```java
public int lengthOfLIS(int[] nums) {
    List<Integer> dp = new ArrayList<>();
    
    for (int num : nums) {
        int pos = Collections.binarySearch(dp, num);
        if (pos < 0) pos = -(pos + 1);
        
        if (pos == dp.size()) {
            dp.add(num);
        } else {
            dp.set(pos, num);
        }
    }
    return dp.size();
}
```

---

## 8. Utilities

### Prefix Sum

```java
int[] prefix = new int[n + 1];
for (int i = 0; i < n; i++) {
    prefix[i + 1] = prefix[i] + nums[i];
}
// Sum of range [l, r] = prefix[r+1] - prefix[l]
```

### Monotonic Stack — Next Greater Element

```java
public int[] nextGreater(int[] nums) {
    int n = nums.length;
    int[] result = new int[n];
    Arrays.fill(result, -1);
    Deque<Integer> st = new ArrayDeque<>();  // indices
    
    for (int i = 0; i < n; i++) {
        while (!st.isEmpty() && nums[st.peek()] < nums[i]) {
            result[st.pop()] = nums[i];
        }
        st.push(i);
    }
    return result;
}
```

---

## 9. Segment Tree (Range Sum + Point Update)

```java
class SegmentTree {
    int[] tree;
    int n;
    
    private void build(int[] arr, int node, int start, int end) {
        if (start == end) {
            tree[node] = arr[start];
        } else {
            int mid = (start + end) / 2;
            build(arr, 2*node, start, mid);
            build(arr, 2*node+1, mid+1, end);
            tree[node] = tree[2*node] + tree[2*node+1];
        }
    }
    
    private void update(int node, int start, int end, int idx, int val) {
        if (start == end) {
            tree[node] = val;
        } else {
            int mid = (start + end) / 2;
            if (idx <= mid) {
                update(2*node, start, mid, idx, val);
            } else {
                update(2*node+1, mid+1, end, idx, val);
            }
            tree[node] = tree[2*node] + tree[2*node+1];
        }
    }
    
    private int query(int node, int start, int end, int l, int r) {
        if (r < start || end < l) return 0;  // Out of range
        if (l <= start && end <= r) return tree[node];  // Fully in range
        
        int mid = (start + end) / 2;
        return query(2*node, start, mid, l, r) + 
               query(2*node+1, mid+1, end, l, r);
    }
    
    public SegmentTree(int[] arr) {
        n = arr.length;
        tree = new int[4 * n];
        build(arr, 1, 0, n-1);
    }
    
    public void update(int idx, int val) { update(1, 0, n-1, idx, val); }
    public int query(int l, int r) { return query(1, 0, n-1, l, r); }
}
```

---

## 10. Floyd-Warshall (All Pairs Shortest Path)

```java
// dist[i][j] = shortest path from i to j
// Initialize: dist[i][j] = INF, dist[i][i] = 0, dist[u][v] = weight
public void floydWarshall(int[][] dist, int n) {
    for (int k = 0; k < n; k++) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (dist[i][k] != Integer.MAX_VALUE && dist[k][j] != Integer.MAX_VALUE) {
                    dist[i][j] = Math.min(dist[i][j], dist[i][k] + dist[k][j]);
                }
            }
        }
    }
    // Negative cycle: if dist[i][i] < 0 for any i
}
```

---

## 11. LRU Cache

```java
class LRUCache {
    private int capacity;
    private LinkedHashMap<Integer, Integer> cache;
    
    public LRUCache(int capacity) {
        this.capacity = capacity;
        this.cache = new LinkedHashMap<>(capacity, 0.75f, true) {
            @Override
            protected boolean removeEldestEntry(Map.Entry<Integer, Integer> eldest) {
                return size() > LRUCache.this.capacity;
            }
        };
    }
    
    public int get(int key) {
        return cache.getOrDefault(key, -1);
    }
    
    public void put(int key, int value) {
        cache.put(key, value);
    }
}

// Alternative: Manual implementation with HashMap + DoublyLinkedList
class LRUCacheManual {
    class Node {
        int key, value;
        Node prev, next;
        Node(int k, int v) { key = k; value = v; }
    }
    
    private int capacity;
    private Map<Integer, Node> map;
    private Node head, tail;  // Dummy nodes
    
    public LRUCacheManual(int capacity) {
        this.capacity = capacity;
        map = new HashMap<>();
        head = new Node(0, 0);
        tail = new Node(0, 0);
        head.next = tail;
        tail.prev = head;
    }
    
    private void remove(Node node) {
        node.prev.next = node.next;
        node.next.prev = node.prev;
    }
    
    private void addToFront(Node node) {
        node.next = head.next;
        node.prev = head;
        head.next.prev = node;
        head.next = node;
    }
    
    public int get(int key) {
        if (!map.containsKey(key)) return -1;
        Node node = map.get(key);
        remove(node);
        addToFront(node);
        return node.value;
    }
    
    public void put(int key, int value) {
        if (map.containsKey(key)) {
            Node node = map.get(key);
            node.value = value;
            remove(node);
            addToFront(node);
        } else {
            if (map.size() == capacity) {
                Node lru = tail.prev;
                remove(lru);
                map.remove(lru.key);
            }
            Node node = new Node(key, value);
            addToFront(node);
            map.put(key, node);
        }
    }
}
```

---

## 12. Intervals

### Merge Intervals

```java
public int[][] merge(int[][] intervals) {
    Arrays.sort(intervals, (a, b) -> a[0] - b[0]);
    List<int[]> result = new ArrayList<>();
    
    for (int[] interval : intervals) {
        if (result.isEmpty() || result.get(result.size() - 1)[1] < interval[0]) {
            result.add(interval);
        } else {
            result.get(result.size() - 1)[1] = Math.max(result.get(result.size() - 1)[1], interval[1]);
        }
    }
    return result.toArray(new int[result.size()][]);
}
```

### Insert Interval

```java
public int[][] insert(int[][] intervals, int[] newInterval) {
    List<int[]> result = new ArrayList<>();
    int i = 0, n = intervals.length;
    
    // Add all intervals before newInterval
    while (i < n && intervals[i][1] < newInterval[0]) {
        result.add(intervals[i++]);
    }
    
    // Merge overlapping intervals
    while (i < n && intervals[i][0] <= newInterval[1]) {
        newInterval[0] = Math.min(newInterval[0], intervals[i][0]);
        newInterval[1] = Math.max(newInterval[1], intervals[i][1]);
        i++;
    }
    result.add(newInterval);
    
    // Add remaining intervals
    while (i < n) {
        result.add(intervals[i++]);
    }
    return result.toArray(new int[result.size()][]);
}
```
