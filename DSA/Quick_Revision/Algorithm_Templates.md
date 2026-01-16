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

```cpp
int binarySearch(vector<int>& nums, int target) {
    int left = 0, right = nums.size() - 1;
    
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

```cpp
int lowerBound(vector<int>& nums, int target) {
    int left = 0, right = nums.size();
    
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

```cpp
int slidingWindow(vector<int>& nums) {
    int left = 0, ans = 0;
    // state variables here (e.g., sum, map)
    
    for (int right = 0; right < nums.size(); right++) {
        // 1. Add nums[right] to window state
        
        // 2. Shrink window while condition is broken
        while (/* window invalid */) {
            // Remove nums[left] from state
            left++;
        }
        
        // 3. Update answer
        ans = max(ans, right - left + 1);
    }
    return ans;
}

### QuickSelect (Kth Smallest Element)
```cpp
// Returns the k-th smallest element (0-indexed: k=0 is min)
// Average: O(N), Worst: O(N^2)
int quickSelect(vector<int>& nums, int k) {
    int l = 0, r = nums.size() - 1;
    while (l <= r) {
        int pivot = nums[r];
        int p = l;
        for (int i = l; i < r; i++) {
            if (nums[i] <= pivot) {
                swap(nums[i], nums[p]);
                p++;
            }
        }
        swap(nums[p], nums[r]);
        
        if (p < k) l = p + 1;
        else if (p > k) r = p - 1;
        else return nums[p];
    }
    return -1;
}
```

```

---

## 2. Trees

### DFS — Inorder/Preorder/Postorder

```cpp
void inorder(TreeNode* root) {
    if (root == nullptr) return;
    
    inorder(root->left);      // Left
    process(root->val);       // Process (move for pre/post)
    inorder(root->right);     // Right
}
// Preorder: Process, Left, Right
// Postorder: Left, Right, Process
```

### BFS — Level Order

```cpp
void levelOrder(TreeNode* root) {
    if (root == nullptr) return;
    
    queue<TreeNode*> q;
    q.push(root);
    
    while (!q.empty()) {
        int levelSize = q.size();
        
        for (int i = 0; i < levelSize; i++) {
            TreeNode* node = q.front();
            q.pop();
            
            // Process node
            
            if (node->left) q.push(node->left);
            if (node->right) q.push(node->right);
        }
    }
    return;
}

### LCA (Lowest Common Ancestor)
```cpp
TreeNode* lowestCommonAncestor(TreeNode* root, TreeNode* p, TreeNode* q) {
    if (!root || root == p || root == q) return root;
    
    TreeNode* left = lowestCommonAncestor(root->left, p, q);
    TreeNode* right = lowestCommonAncestor(root->right, p, q);
    
    if (left && right) return root; // Found one in each subtree
    return left ? left : right;     // Both in one subtree (or neither found)
}
```

```

---

## 3. Graphs

### DFS — Basic

```cpp
vector<vector<int>> adj;
vector<bool> visited;

void dfs(int node) {
    visited[node] = true;
    
    // Process node here
    
    for (int neighbor : adj[node]) {
        if (!visited[neighbor]) {
            dfs(neighbor);
        }
    }
}
```

### BFS — Shortest Path (Unweighted)

```cpp
vector<int> bfs(int start, int n) {
    vector<int> dist(n, -1);
    queue<int> q;
    
    dist[start] = 0;
    q.push(start);
    
    while (!q.empty()) {
        int node = q.front();
        q.pop();
        
        for (int neighbor : adj[node]) {
            if (dist[neighbor] == -1) {
                dist[neighbor] = dist[node] + 1;
                q.push(neighbor);
            }
        }
    }
    return dist;
}
```

### Matrix Traversal (BFS)

```cpp
int R, C;
int dr[] = {-1, 1, 0, 0}; // Up, Down, Left, Right
int dc[] = {0, 0, -1, 1};

void bfsGrid(int r, int c, vector<vector<int>>& grid) {
    queue<pair<int, int>> q;
    q.push({r, c});
    grid[r][c] = -1; // Mark visited (or use visited array)
    
    while (!q.empty()) {
        auto [cr, cc] = q.front(); q.pop();
        
        for (int i = 0; i < 4; i++) {
            int nr = cr + dr[i];
            int nc = cc + dc[i];
            
            if (nr >= 0 && nr < R && nc >= 0 && nc < C && grid[nr][nc] != -1) {
                grid[nr][nc] = -1; 
                q.push({nr, nc});
            }
        }
    }
}
```

### Cycle Detection — Directed Graph (3-color DFS)

```cpp
// 0 = unvisited, 1 = in current path, 2 = processed
vector<int> color;

bool hasCycle(int node) {
    color[node] = 1;  // Mark as "in progress"
    
    for (int neighbor : adj[node]) {
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

```cpp
bool hasCycle(int node, int parent) {
    visited[node] = true;
    
    for (int neighbor : adj[node]) {
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

```cpp
class DSU {
    vector<int> parent, size;
public:
    DSU(int n) : parent(n), size(n, 1) {
        for (int i = 0; i < n; i++) {
            parent[i] = i;
        }
    }
    
    int find(int x) {
        if (parent[x] != x) {
            parent[x] = find(parent[x]);  // Path compression
        }
        return parent[x];
    }
    
    bool unite(int x, int y) {
        int rootX = find(x);
        int rootY = find(y);
        
        if (rootX == rootY) return false;  // Already connected
        
        // Union by size
        if (size[rootX] < size[rootY]) swap(rootX, rootY);
        parent[rootY] = rootX;
        size[rootX] += size[rootY];
        return true;
    }
};
```

### Topological Sort — Kahn's Algorithm (BFS)

```cpp
vector<int> topoSort(int n, vector<vector<int>>& adj) {
    vector<int> indegree(n, 0);
    
    // Calculate indegrees
    for (int u = 0; u < n; u++) {
        for (int v : adj[u]) {
            indegree[v]++;
        }
    }
    
    // Push all nodes with indegree 0
    queue<int> q;
    for (int i = 0; i < n; i++) {
        if (indegree[i] == 0) q.push(i);
    }
    
    vector<int> result;
    while (!q.empty()) {
        int node = q.front();
        q.pop();
        result.push_back(node);
        
        for (int neighbor : adj[node]) {
            indegree[neighbor]--;
            if (indegree[neighbor] == 0) {
                q.push(neighbor);
            }
        }
    }
    
    // If result size != n, there's a cycle
    return result.size() == n ? result : vector<int>();
}
```

### Dijkstra — Shortest Path (Weighted)

```cpp
vector<int> dijkstra(int start, int n, vector<vector<pair<int,int>>>& adj) {
    vector<int> dist(n, INT_MAX);
    priority_queue<pair<int,int>, vector<pair<int,int>>, greater<pair<int,int>>> pq;
    
    dist[start] = 0;
    pq.push({0, start});  // {distance, node}
    
    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();
        
        if (d > dist[u]) continue;  // Skip outdated entries
        
        for (auto [v, weight] : adj[u]) {
            if (dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                pq.push({dist[v], v});
            }
        }
    }
    return dist;
}
```

### Bellman-Ford — Shortest Path (Negative Weights)

```cpp
// Returns true if negative cycle exists
vector<int> bellmanFord(int start, int n, vector<tuple<int,int,int>>& edges) {
    vector<int> dist(n, INT_MAX);
    dist[start] = 0;
    
    // Relax all edges (n-1) times
    for (int i = 0; i < n - 1; i++) {
        for (auto [u, v, weight] : edges) {
            if (dist[u] != INT_MAX && dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
            }
        }
    }
    
    // Check for negative cycle (one more relaxation)
    for (auto [u, v, weight] : edges) {
        if (dist[u] != INT_MAX && dist[u] + weight < dist[v]) {
            // Negative cycle detected
            return {};
        }
    }
    return dist;
}
```

---

## 4. Trie (Prefix Tree)

```cpp
class TrieNode {
public:
    TrieNode* children[26];
    bool isEnd;
    
    TrieNode() {
        isEnd = false;
        for (int i = 0; i < 26; i++) {
            children[i] = nullptr;
        }
    }
};

class Trie {
    TrieNode* root;
public:
    Trie() {
        root = new TrieNode();
    }
    
    void insert(string word) {
        TrieNode* node = root;
        for (char c : word) {
            int idx = c - 'a';
            if (node->children[idx] == nullptr) {
                node->children[idx] = new TrieNode();
            }
            node = node->children[idx];
        }
        node->isEnd = true;
    }
    
    bool search(string word) {
        TrieNode* node = root;
        for (char c : word) {
            int idx = c - 'a';
            if (node->children[idx] == nullptr) {
                return false;
            }
            node = node->children[idx];
        }
        return node->isEnd;
    }
    
    bool startsWith(string prefix) {
        TrieNode* node = root;
        for (char c : prefix) {
            int idx = c - 'a';
            if (node->children[idx] == nullptr) {
                return false;
            }
            node = node->children[idx];
        }
        return true;
    }
};
```

---

## 5. String Algorithms

### KMP Algorithm (Pattern Matching)

```cpp
// Returns starting indices of all occurrences of pat in txt
vector<int> computeLPS(string& pat) {
    int m = pat.size();
    vector<int> lps(m, 0);
    int len = 0, i = 1;
    
    while (i < m) {
        if (pat[i] == pat[len]) {
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

vector<int> KMP(string txt, string pat) {
    int n = txt.size(), m = pat.size();
    vector<int> lps = computeLPS(pat);
    vector<int> result;
    int i = 0, j = 0;
    
    while (i < n) {
        if (pat[j] == txt[i]) {
            j++;
            i++;
        }
        
        if (j == m) {
            result.push_back(i - j);
            j = lps[j - 1];
        } else if (i < n && pat[j] != txt[i]) {
            if (j != 0) j = lps[j - 1];
            else i++;
        }
    }
    return result;
}
```

### Rolling Hash (Rabin-Karp)

```cpp
// Simple version for single pattern search (modulo 1e9+7)
long long computeHash(string& s) {
    long long p = 31, m = 1e9 + 9;
    long long hash_value = 0, p_pow = 1;
    for (char c : s) {
        hash_value = (hash_value + (c - 'a' + 1) * p_pow) % m;
        p_pow = (p_pow * p) % m;
    }
    return hash_value;
}
```

---

## 6. Backtracking

### General Template

```cpp
void backtrack(int index, vector<int>& path) {
    // Base case
    if (/* reached goal */) {
        result.push_back(path);
        return;
    }
    
    for (int i = index; i < nums.size(); i++) {
        // Skip invalid choices (pruning)
        if (!isValid(i)) continue;
        
        path.push_back(nums[i]);    // Choose
        backtrack(i + 1, path);     // Explore (use i for allowing repeats)
        path.pop_back();            // Unchoose
    }
}
```

### Subsets

```cpp
void subsets(int index, vector<int>& current) {
    result.push_back(current);  // Every state is a valid subset
    
    for (int i = index; i < nums.size(); i++) {
        current.push_back(nums[i]);
        subsets(i + 1, current);
        current.pop_back();
    }
}
```

### Permutations

```cpp
void permute(vector<int>& current, vector<bool>& used) {
    if (current.size() == nums.size()) {
        result.push_back(current);
        return;
    }
    
    for (int i = 0; i < nums.size(); i++) {
        if (used[i]) continue;
        
        used[i] = true;
        current.push_back(nums[i]);
        
        permute(current, used);
        
        current.pop_back();
        used[i] = false;
    }
}
```

---

## 7. DP Patterns

### 1D DP — House Robber Style

```cpp
// dp[i] = max value considering elements 0..i
int rob(vector<int>& nums) {
    int prev2 = 0, prev1 = 0;
    
    for (int num : nums) {
        int current = max(prev1, prev2 + num);
        prev2 = prev1;
        prev1 = current;
    }
    return prev1;
}
```

### 2D DP — Grid Paths

```cpp
int uniquePaths(int m, int n) {
    vector<vector<int>> dp(m, vector<int>(n, 1));
    
    for (int i = 1; i < m; i++) {
        for (int j = 1; j < n; j++) {
            dp[i][j] = dp[i-1][j] + dp[i][j-1];
        }
    }
    return dp[m-1][n-1];
}

### LCS (Longest Common Subsequence)
```cpp
int longestCommonSubsequence(string text1, string text2) {
    int m = text1.size(), n = text2.size();
    vector<vector<int>> dp(m + 1, vector<int>(n + 1, 0));
    
    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= n; j++) {
            if (text1[i-1] == text2[j-1]) {
                dp[i][j] = 1 + dp[i-1][j-1];
            } else {
                dp[i][j] = max(dp[i-1][j], dp[i][j-1]);
            }
        }
    }
    return dp[m][n];
}
```

```

### 0/1 Knapsack

```cpp
int knapsack(vector<int>& weights, vector<int>& values, int capacity) {
    vector<int> dp(capacity + 1, 0);
    
    for (int i = 0; i < weights.size(); i++) {
        for (int w = capacity; w >= weights[i]; w--) {  // Reverse!
            dp[w] = max(dp[w], dp[w - weights[i]] + values[i]);
        }
    }
    return dp[capacity];
}
```

### LIS — O(n log n)

```cpp
int lengthOfLIS(vector<int>& nums) {
    vector<int> dp;
    
    for (int num : nums) {
        auto it = lower_bound(dp.begin(), dp.end(), num);
        if (it == dp.end()) {
            dp.push_back(num);
        } else {
            *it = num;
        }
    }
    return dp.size();
}
```

---

## 8. Utilities

### Prefix Sum

```cpp
vector<int> prefix(n + 1, 0);
for (int i = 0; i < n; i++) {
    prefix[i + 1] = prefix[i] + nums[i];
}
// Sum of range [l, r] = prefix[r+1] - prefix[l]
```

### Monotonic Stack — Next Greater Element

```cpp
vector<int> nextGreater(vector<int>& nums) {
    int n = nums.size();
    vector<int> result(n, -1);
    stack<int> st;  // indices
    
    for (int i = 0; i < n; i++) {
        while (!st.empty() && nums[st.top()] < nums[i]) {
            result[st.top()] = nums[i];
            st.pop();
        }
        st.push(i);
    }
    return result;
}
```

---

## 9. Segment Tree (Range Sum + Point Update)

```cpp
class SegmentTree {
    vector<int> tree;
    int n;
    
    void build(vector<int>& arr, int node, int start, int end) {
        if (start == end) {
            tree[node] = arr[start];
        } else {
            int mid = (start + end) / 2;
            build(arr, 2*node, start, mid);
            build(arr, 2*node+1, mid+1, end);
            tree[node] = tree[2*node] + tree[2*node+1];
        }
    }
    
    void update(int node, int start, int end, int idx, int val) {
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
    
    int query(int node, int start, int end, int l, int r) {
        if (r < start || end < l) return 0;  // Out of range
        if (l <= start && end <= r) return tree[node];  // Fully in range
        
        int mid = (start + end) / 2;
        return query(2*node, start, mid, l, r) + 
               query(2*node+1, mid+1, end, l, r);
    }
    
public:
    SegmentTree(vector<int>& arr) {
        n = arr.size();
        tree.resize(4 * n);
        build(arr, 1, 0, n-1);
    }
    
    void update(int idx, int val) { update(1, 0, n-1, idx, val); }
    int query(int l, int r) { return query(1, 0, n-1, l, r); }
};
```

---

## 10. Floyd-Warshall (All Pairs Shortest Path)

```cpp
// dist[i][j] = shortest path from i to j
// Initialize: dist[i][j] = INF, dist[i][i] = 0, dist[u][v] = weight
void floydWarshall(vector<vector<int>>& dist, int n) {
    for (int k = 0; k < n; k++) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (dist[i][k] != INT_MAX && dist[k][j] != INT_MAX) {
                    dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
                }
            }
        }
    }
    // Negative cycle: if dist[i][i] < 0 for any i
}
```

---

## 11. LRU Cache

```cpp
class LRUCache {
    int capacity;
    list<pair<int, int>> cache;  // {key, value}
    unordered_map<int, list<pair<int,int>>::iterator> map;
    
public:
    LRUCache(int cap) : capacity(cap) {}
    
    int get(int key) {
        if (map.find(key) == map.end()) return -1;
        
        // Move to front (most recently used)
        cache.splice(cache.begin(), cache, map[key]);
        return map[key]->second;
    }
    
    void put(int key, int value) {
        if (map.find(key) != map.end()) {
            // Update existing
            map[key]->second = value;
            cache.splice(cache.begin(), cache, map[key]);
            return;
        }
        
        // Evict LRU if at capacity
        if (cache.size() == capacity) {
            int lruKey = cache.back().first;
            cache.pop_back();
            map.erase(lruKey);
        }
        
        // Insert new
        cache.push_front({key, value});
        map[key] = cache.begin();
    }
};
```

---

## 12. Intervals

### Merge Intervals

```cpp
vector<vector<int>> merge(vector<vector<int>>& intervals) {
    sort(intervals.begin(), intervals.end());
    vector<vector<int>> result;
    
    for (auto& interval : intervals) {
        if (result.empty() || result.back()[1] < interval[0]) {
            result.push_back(interval);
        } else {
            result.back()[1] = max(result.back()[1], interval[1]);
        }
    }
    return result;
}
```

### Insert Interval

```cpp
vector<vector<int>> insert(vector<vector<int>>& intervals, vector<int>& newInterval) {
    vector<vector<int>> result;
    int i = 0, n = intervals.size();
    
    // Add all intervals before newInterval
    while (i < n && intervals[i][1] < newInterval[0]) {
        result.push_back(intervals[i++]);
    }
    
    // Merge overlapping intervals
    while (i < n && intervals[i][0] <= newInterval[1]) {
        newInterval[0] = min(newInterval[0], intervals[i][0]);
        newInterval[1] = max(newInterval[1], intervals[i][1]);
        i++;
    }
    result.push_back(newInterval);
    
    // Add remaining intervals
    while (i < n) {
        result.push_back(intervals[i++]);
    }
    return result;
}
```
