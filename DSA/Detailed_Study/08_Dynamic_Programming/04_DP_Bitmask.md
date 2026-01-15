# DP with Bitmask (State Compression)

> **Trigger**: Small n (≤20), subset states, TSP-style, "visit all with minimum cost"

---

## Key Concept

Use binary number to represent subset state:

- Bit `i` = 1 means element `i` is included
- `mask` ranges from 0 to 2^n - 1

```cpp
// Check if bit i is set
bool isSet(int mask, int i) { return (mask >> i) & 1; }

// Set bit i
int setBit(int mask, int i) { return mask | (1 << i); }

// Iterate all subsets of mask
for (int sub = mask; sub > 0; sub = (sub - 1) & mask) {
    // process subset 'sub'
}
```

---

## Traveling Salesman Problem (TSP)

**O(n² × 2ⁿ)** time

```cpp
int tsp(vector<vector<int>>& dist) {
    int n = dist.size();
    vector<vector<int>> dp(1 << n, vector<int>(n, INT_MAX));
    
    dp[1][0] = 0;  // Start at node 0
    
    for (int mask = 1; mask < (1 << n); mask++) {
        for (int last = 0; last < n; last++) {
            if (!(mask & (1 << last))) continue;
            if (dp[mask][last] == INT_MAX) continue;
            
            for (int next = 0; next < n; next++) {
                if (mask & (1 << next)) continue;
                
                int newMask = mask | (1 << next);
                dp[newMask][next] = min(dp[newMask][next], 
                                        dp[mask][last] + dist[last][next]);
            }
        }
    }
    
    int ans = INT_MAX;
    int fullMask = (1 << n) - 1;
    for (int last = 0; last < n; last++) {
        if (dp[fullMask][last] != INT_MAX) {
            ans = min(ans, dp[fullMask][last] + dist[last][0]);
        }
    }
    return ans;
}
```

---

## Shortest Path Visiting All Nodes

**LeetCode 847** | O(n² × 2ⁿ) time

```cpp
int shortestPathLength(vector<vector<int>>& graph) {
    int n = graph.size();
    if (n == 1) return 0;
    
    int fullMask = (1 << n) - 1;
    vector<vector<bool>> visited(1 << n, vector<bool>(n, false));
    queue<tuple<int, int, int>> q; // mask, node, dist
    
    for (int i = 0; i < n; i++) {
        q.push({1 << i, i, 0});
        visited[1 << i][i] = true;
    }
    
    while (!q.empty()) {
        auto [mask, node, dist] = q.front(); q.pop();
        
        for (int neighbor : graph[node]) {
            int newMask = mask | (1 << neighbor);
            if (newMask == fullMask) return dist + 1;
            
            if (!visited[newMask][neighbor]) {
                visited[newMask][neighbor] = true;
                q.push({newMask, neighbor, dist + 1});
            }
        }
    }
    return -1;
}
```

---

## Partition to K Equal Sum Subsets

**LeetCode 698** | O(n × 2ⁿ) time

```cpp
bool canPartitionKSubsets(vector<int>& nums, int k) {
    int sum = accumulate(nums.begin(), nums.end(), 0);
    if (sum % k != 0) return false;
    
    int target = sum / k;
    int n = nums.size();
    vector<int> dp(1 << n, -1);
    dp[0] = 0;
    
    sort(nums.rbegin(), nums.rend());  // Optimization
    
    for (int mask = 0; mask < (1 << n); mask++) {
        if (dp[mask] == -1) continue;
        
        for (int i = 0; i < n; i++) {
            if (mask & (1 << i)) continue;
            if (dp[mask] + nums[i] > target) break;  // Pruning
            
            int newMask = mask | (1 << i);
            dp[newMask] = (dp[mask] + nums[i]) % target;
        }
    }
    return dp[(1 << n) - 1] == 0;
}
```

---

## When to Use

| Constraint | Suitable? |
|------------|-----------|
| n ≤ 15-20 | ✅ Yes (2²⁰ ≈ 10⁶) |
| n ≤ 25 | ⚠️ Maybe with pruning |
| n > 25 | ❌ Too slow |

---

## Practice Problems

| Problem | Difficulty | Key Insight |
|---------|------------|-------------|
| LC 847 - Shortest Path All Nodes | Hard | BFS + bitmask state |
| LC 698 - Partition to K Equal Sum | Medium | DP with subset |
| LC 1125 - Smallest Sufficient Team | Hard | Set cover with bitmask |
| LC 1349 - Maximum Students | Hard | Row-by-row bitmask |
| LC 943 - Shortest Superstring | Hard | TSP on strings |
