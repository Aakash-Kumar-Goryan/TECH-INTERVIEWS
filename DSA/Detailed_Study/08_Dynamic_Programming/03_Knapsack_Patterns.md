# Knapsack Patterns

> **Trigger**: Subset selection with constraint (weight/capacity), optimize value

---

## 0/1 Knapsack (Each item once)

```cpp
int knapsack01(vector<int>& weights, vector<int>& values, int capacity) {
    int n = weights.size();
    vector<vector<int>> dp(n + 1, vector<int>(capacity + 1, 0));
    
    for (int i = 1; i <= n; i++) {
        for (int w = 0; w <= capacity; w++) {
            dp[i][w] = dp[i-1][w]; // Don't take item i
            
            if (weights[i-1] <= w) {
                dp[i][w] = max(dp[i][w], 
                              dp[i-1][w - weights[i-1]] + values[i-1]);
            }
        }
    }
    return dp[n][capacity];
}
```

**Space Optimized O(W):**

```cpp
int knapsack01Optimized(vector<int>& weights, vector<int>& values, int capacity) {
    int n = weights.size();
    vector<int> dp(capacity + 1, 0);
    
    for (int i = 0; i < n; i++) {
        for (int w = capacity; w >= weights[i]; w--) { // Reverse!
            dp[w] = max(dp[w], dp[w - weights[i]] + values[i]);
        }
    }
    return dp[capacity];
}
```

---

## Unbounded Knapsack (Item can be used multiple times)

```cpp
int unboundedKnapsack(vector<int>& weights, vector<int>& values, int capacity) {
    vector<int> dp(capacity + 1, 0);
    
    for (int w = 1; w <= capacity; w++) {
        for (int i = 0; i < weights.size(); i++) {
            if (weights[i] <= w) {
                dp[w] = max(dp[w], dp[w - weights[i]] + values[i]);
            }
        }
    }
    return dp[capacity];
}
```

---

## Subset Sum (Can we make target?)

**LeetCode 416 - Partition Equal Subset Sum** | O(n × sum) time

```cpp
bool canPartition(vector<int>& nums) {
    int sum = accumulate(nums.begin(), nums.end(), 0);
    if (sum % 2 != 0) return false;
    
    int target = sum / 2;
    vector<bool> dp(target + 1, false);
    dp[0] = true;
    
    for (int num : nums) {
        for (int t = target; t >= num; t--) { // Reverse!
            dp[t] = dp[t] || dp[t - num];
        }
    }
    return dp[target];
}
```

---

## Count Subsets with Target Sum

```cpp
int countSubsets(vector<int>& nums, int target) {
    vector<int> dp(target + 1, 0);
    dp[0] = 1;
    
    for (int num : nums) {
        for (int t = target; t >= num; t--) {
            dp[t] += dp[t - num];
        }
    }
    return dp[target];
}
```

---

## Coin Change (Minimum Coins)

**LeetCode 322** | Unbounded knapsack variant

```cpp
int coinChange(vector<int>& coins, int amount) {
    vector<int> dp(amount + 1, amount + 1);
    dp[0] = 0;
    
    for (int i = 1; i <= amount; i++) {
        for (int coin : coins) {
            if (coin <= i) {
                dp[i] = min(dp[i], dp[i - coin] + 1);
            }
        }
    }
    return dp[amount] > amount ? -1 : dp[amount];
}
```

---

## Coin Change 2 (Count Ways)

**LeetCode 518** | O(n × amount) time

```cpp
int change(int amount, vector<int>& coins) {
    vector<int> dp(amount + 1, 0);
    dp[0] = 1;
    
    for (int coin : coins) { // Item loop outside!
        for (int a = coin; a <= amount; a++) {
            dp[a] += dp[a - coin];
        }
    }
    return dp[amount];
}
```

---

## Pattern Summary

| Variant | Item Loop | Amount Loop Direction |
|---------|-----------|----------------------|
| **0/1 Knapsack** | Outer | Right to Left (reverse) |
| **Unbounded** | Inner or Outer | Left to Right |
| **Count Combinations** | Outer (items) | Left to Right |
| **Count Permutations** | Inner (items) | Left to Right |

---

## Practice Problems

| Problem | Difficulty | Key Insight |
|---------|------------|-------------|
| LC 416 - Partition Equal Subset Sum | Medium | 0/1 knapsack |
| LC 494 - Target Sum | Medium | Count subsets + offset |
| LC 322 - Coin Change | Medium | Unbounded, min |
| LC 518 - Coin Change 2 | Medium | Unbounded, count |
| LC 474 - Ones and Zeroes | Medium | 2D 0/1 knapsack |
