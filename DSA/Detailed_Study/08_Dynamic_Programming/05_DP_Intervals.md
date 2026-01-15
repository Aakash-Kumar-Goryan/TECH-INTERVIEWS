# DP on Intervals

> **Trigger**: Merge/split ranges, optimal parenthesization, "min/max cost to combine"

---

## Key Pattern

**State**: `dp[i][j]` = optimal answer for range `[i, j]`

**Transition**: Try all split points `k` in `[i, j-1]`:

```cpp
dp[i][j] = optimize(dp[i][k] + dp[k+1][j] + cost(i, k, j))
```

**Order**: Fill by increasing gap length (small ranges first)

---

## Template

```cpp
int intervalDP(vector<int>& arr) {
    int n = arr.size();
    vector<vector<int>> dp(n, vector<int>(n, 0));
    
    // Length 1: base case (often 0 or trivial)
    
    // Fill by increasing length
    for (int len = 2; len <= n; len++) {
        for (int i = 0; i + len - 1 < n; i++) {
            int j = i + len - 1;
            dp[i][j] = INT_MAX; // or INT_MIN for max
            
            for (int k = i; k < j; k++) {
                dp[i][j] = min(dp[i][j], 
                              dp[i][k] + dp[k+1][j] + cost(i, j));
            }
        }
    }
    return dp[0][n-1];
}
```

---

## Burst Balloons

**LeetCode 312** | O(n³) time

```cpp
int maxCoins(vector<int>& nums) {
    int n = nums.size();
    vector<int> arr(n + 2, 1);
    for (int i = 0; i < n; i++) arr[i + 1] = nums[i];
    
    vector<vector<int>> dp(n + 2, vector<int>(n + 2, 0));
    
    for (int len = 1; len <= n; len++) {
        for (int left = 1; left + len - 1 <= n; left++) {
            int right = left + len - 1;
            
            for (int last = left; last <= right; last++) {
                // 'last' is the last balloon to burst in [left, right]
                int coins = arr[left - 1] * arr[last] * arr[right + 1];
                coins += dp[left][last - 1] + dp[last + 1][right];
                dp[left][right] = max(dp[left][right], coins);
            }
        }
    }
    return dp[1][n];
}
```

**Key insight**: Think of `last` as the balloon burst **last** in range, not first.

---

## Minimum Cost to Merge Stones

**LeetCode 1000** | O(n³) time

```cpp
int mergeStones(vector<int>& stones, int k) {
    int n = stones.size();
    if ((n - 1) % (k - 1) != 0) return -1;
    
    vector<int> prefix(n + 1, 0);
    for (int i = 0; i < n; i++) 
        prefix[i + 1] = prefix[i] + stones[i];
    
    vector<vector<int>> dp(n, vector<int>(n, 0));
    
    for (int len = k; len <= n; len++) {
        for (int i = 0; i + len - 1 < n; i++) {
            int j = i + len - 1;
            dp[i][j] = INT_MAX;
            
            for (int mid = i; mid < j; mid += k - 1) {
                dp[i][j] = min(dp[i][j], dp[i][mid] + dp[mid + 1][j]);
            }
            
            if ((len - 1) % (k - 1) == 0) {
                dp[i][j] += prefix[j + 1] - prefix[i];
            }
        }
    }
    return dp[0][n - 1];
}
```

---

## Minimum Score Triangulation

**LeetCode 1039** | O(n³) time

```cpp
int minScoreTriangulation(vector<int>& values) {
    int n = values.size();
    vector<vector<int>> dp(n, vector<int>(n, 0));
    
    for (int len = 3; len <= n; len++) {
        for (int i = 0; i + len - 1 < n; i++) {
            int j = i + len - 1;
            dp[i][j] = INT_MAX;
            
            for (int k = i + 1; k < j; k++) {
                int cost = values[i] * values[k] * values[j];
                dp[i][j] = min(dp[i][j], dp[i][k] + dp[k][j] + cost);
            }
        }
    }
    return dp[0][n - 1];
}
```

---

## Palindrome Partitioning II

**LeetCode 132** | O(n²) time

```cpp
int minCut(string s) {
    int n = s.size();
    vector<vector<bool>> isPalin(n, vector<bool>(n, false));
    
    // Precompute palindromes
    for (int i = n - 1; i >= 0; i--) {
        for (int j = i; j < n; j++) {
            isPalin[i][j] = (s[i] == s[j]) && 
                            (j - i < 2 || isPalin[i + 1][j - 1]);
        }
    }
    
    vector<int> dp(n, 0);
    for (int i = 0; i < n; i++) {
        if (isPalin[0][i]) {
            dp[i] = 0;
        } else {
            dp[i] = i;  // Max cuts
            for (int j = 1; j <= i; j++) {
                if (isPalin[j][i]) {
                    dp[i] = min(dp[i], dp[j - 1] + 1);
                }
            }
        }
    }
    return dp[n - 1];
}
```

---

## Practice Problems

| Problem | Difficulty | Key Insight |
|---------|------------|-------------|
| LC 312 - Burst Balloons | Hard | Last balloon in range |
| LC 1039 - Triangulation | Medium | Triangle with i, k, j |
| LC 1000 - Merge Stones | Hard | k-way merge constraint |
| LC 132 - Palindrome Partition II | Hard | Precompute + 1D DP |
| LC 87 - Scramble String | Hard | 3D interval DP |
