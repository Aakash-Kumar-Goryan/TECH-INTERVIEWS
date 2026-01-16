# Backtracking with Memoization (Caching Optimization)

> **Trigger**: Backtracking problem with **overlapping subproblems** → Add memoization to avoid recomputation

---

## When to Add Memoization

| Signal | Example |
|--------|---------|
| Same subproblem reached via different paths | Word Break: "leetcode" can split at different points reaching same suffix |
| State can be represented compactly | Index + remaining value, or index + bitmask |
| Exponential time but polynomial unique states | 2^n paths but only n×target unique states |

**Key Insight**: If you're solving the same state multiple times, cache it!

---

## Framework: Backtracking → Memoization

```cpp
// Before: Pure backtracking (exponential, repeated work)
bool backtrack(state) {
    if (base_case) return result;
    
    for (choice in choices) {
        if (backtrack(next_state)) return true;
    }
    return false;
}

// After: With memoization (polynomial, no repeated work)
unordered_map<State, bool> memo;

bool backtrackMemo(state) {
    if (base_case) return result;
    if (memo.count(state)) return memo[state];  // Cache hit
    
    for (choice in choices) {
        if (backtrackMemo(next_state)) {
            return memo[state] = true;
        }
    }
    return memo[state] = false;  // Cache result
}
```

---

## Pattern 1: Word Break

**LeetCode 139** | Without memo: O(2^n), With memo: O(n²)

```cpp
bool wordBreak(string s, vector<string>& wordDict) {
    unordered_set<string> dict(wordDict.begin(), wordDict.end());
    unordered_map<int, bool> memo;
    return backtrack(s, 0, dict, memo);
}

bool backtrack(string& s, int start, unordered_set<string>& dict, 
               unordered_map<int, bool>& memo) {
    if (start == s.size()) return true;
    if (memo.count(start)) return memo[start];  // Cache hit
    
    for (int end = start + 1; end <= s.size(); end++) {
        string word = s.substr(start, end - start);
        if (dict.count(word) && backtrack(s, end, dict, memo)) {
            return memo[start] = true;
        }
    }
    return memo[start] = false;
}
```

**State**: Just the start index (what suffix are we trying to break?)

---

## Pattern 2: Target Sum

**LeetCode 494** | O(n × sum) time with memo

```cpp
int findTargetSumWays(vector<int>& nums, int target) {
    map<pair<int, int>, int> memo;  // (index, currentSum) -> count
    return backtrack(nums, 0, 0, target, memo);
}

int backtrack(vector<int>& nums, int idx, int sum, int target,
              map<pair<int, int>, int>& memo) {
    if (idx == nums.size()) {
        return sum == target ? 1 : 0;
    }
    
    auto key = make_pair(idx, sum);
    if (memo.count(key)) return memo[key];
    
    int ways = backtrack(nums, idx + 1, sum + nums[idx], target, memo)
             + backtrack(nums, idx + 1, sum - nums[idx], target, memo);
    
    return memo[key] = ways;
}
```

**State**: (index, current sum) — two choices at each step, but many paths reach same state.

---

## Pattern 3: Palindrome Partitioning II (Min Cuts)

**LeetCode 132** | O(n²) time with memo

```cpp
int minCut(string s) {
    int n = s.size();
    vector<vector<bool>> isPalin(n, vector<bool>(n, false));
    
    // Precompute all palindrome substrings
    for (int i = n - 1; i >= 0; i--) {
        for (int j = i; j < n; j++) {
            isPalin[i][j] = (s[i] == s[j]) && 
                            (j - i < 2 || isPalin[i+1][j-1]);
        }
    }
    
    vector<int> memo(n, -1);
    return backtrack(s, 0, isPalin, memo);
}

int backtrack(string& s, int start, vector<vector<bool>>& isPalin,
              vector<int>& memo) {
    if (start == s.size()) return -1;  // -1 because we count cuts, not partitions
    if (memo[start] != -1) return memo[start];
    
    int minCuts = INT_MAX;
    for (int end = start; end < s.size(); end++) {
        if (isPalin[start][end]) {
            int cuts = 1 + backtrack(s, end + 1, isPalin, memo);
            minCuts = min(minCuts, cuts);
        }
    }
    return memo[start] = minCuts;
}
```

**State**: Start index — we're finding min cuts for suffix s[start:].

---

## Pattern 4: Partition Equal Subset Sum

**LeetCode 416** | O(n × sum) with memo

```cpp
bool canPartition(vector<int>& nums) {
    int total = accumulate(nums.begin(), nums.end(), 0);
    if (total % 2 != 0) return false;
    
    int target = total / 2;
    vector<vector<int>> memo(nums.size(), vector<int>(target + 1, -1));
    return backtrack(nums, 0, target, memo);
}

bool backtrack(vector<int>& nums, int idx, int remaining,
               vector<vector<int>>& memo) {
    if (remaining == 0) return true;
    if (idx >= nums.size() || remaining < 0) return false;
    if (memo[idx][remaining] != -1) return memo[idx][remaining];
    
    // Take or skip current element
    bool result = backtrack(nums, idx + 1, remaining - nums[idx], memo)
               || backtrack(nums, idx + 1, remaining, memo);
    
    return memo[idx][remaining] = result;
}
```

---

## State Design Guide

| Problem Type | State Components | Key Size |
|-------------|------------------|----------|
| String partition | start index | O(n) |
| Subset selection | (index, remaining) | O(n × target) |
| Two sequences | (i, j) indices | O(n × m) |
| Bitmask | visited mask | O(2^n) |

---

## Transition: Backtracking → DP

Once you identify memoization works, you can convert to bottom-up DP:

```cpp
// Word Break as bottom-up DP
bool wordBreak(string s, vector<string>& wordDict) {
    unordered_set<string> dict(wordDict.begin(), wordDict.end());
    int n = s.size();
    vector<bool> dp(n + 1, false);
    dp[0] = true;  // Empty string is breakable
    
    for (int i = 1; i <= n; i++) {
        for (int j = 0; j < i; j++) {
            if (dp[j] && dict.count(s.substr(j, i - j))) {
                dp[i] = true;
                break;
            }
        }
    }
    return dp[n];
}
```

**When to convert**: If you need space optimization or the recursive stack depth is a concern.

---

## Common Mistakes

| Mistake | Fix |
|---------|-----|
| Wrong state definition | Ensure state captures ALL info needed to solve subproblem |
| Mutable state as key | Use immutable representations (index, not actual substring) |
| Not handling base cases before memo check | Check base case first, then memo |
| Using vector for sparse states | Use hashmap for sparse, vector for dense |

---

## Practice Problems

| Problem | Difficulty | State Type |
|---------|------------|------------|
| LC 139 - Word Break | Medium | index |
| LC 140 - Word Break II | Hard | index (return all) |
| LC 494 - Target Sum | Medium | (index, sum) |
| LC 416 - Partition Equal Subset Sum | Medium | (index, remaining) |
| LC 132 - Palindrome Partitioning II | Hard | index |
| LC 698 - Partition to K Equal Sum Subsets | Medium | bitmask |
| LC 691 - Stickers to Spell Word | Hard | target string mask |
