# Backtracking - Subsets & Permutations

> **Trigger**: Generate all combinations, permutations, subsets, or valid configurations

---

## Framework

```cpp
void backtrack(state, choices, result) {
    if (goal_reached) {
        result.add(copy_of_state);
        return;
    }
    
    for (choice in choices) {
        if (is_valid(choice)) {
            make_choice(state, choice);
            backtrack(state, remaining_choices, result);
            undo_choice(state, choice);  // backtrack
        }
    }
}
```

---

## Pattern 1: Subsets

**LeetCode 78** | O(n × 2ⁿ) time

```cpp
vector<vector<int>> subsets(vector<int>& nums) {
    vector<vector<int>> result;
    vector<int> current;
    backtrack(nums, 0, current, result);
    return result;
}

void backtrack(vector<int>& nums, int start, 
               vector<int>& current, vector<vector<int>>& result) {
    result.push_back(current);  // Every path is a valid subset
    
    for (int i = start; i < nums.size(); i++) {
        current.push_back(nums[i]);
        backtrack(nums, i + 1, current, result);
        current.pop_back();
    }
}
```

---

## Pattern 2: Subsets with Duplicates

**LeetCode 90** | O(n × 2ⁿ) time

```cpp
vector<vector<int>> subsetsWithDup(vector<int>& nums) {
    sort(nums.begin(), nums.end());  // Sort to group duplicates
    vector<vector<int>> result;
    vector<int> current;
    backtrack(nums, 0, current, result);
    return result;
}

void backtrack(vector<int>& nums, int start, 
               vector<int>& current, vector<vector<int>>& result) {
    result.push_back(current);
    
    for (int i = start; i < nums.size(); i++) {
        if (i > start && nums[i] == nums[i-1]) continue;  // Skip dups
        current.push_back(nums[i]);
        backtrack(nums, i + 1, current, result);
        current.pop_back();
    }
}
```

---

## Pattern 3: Permutations

**LeetCode 46** | O(n × n!) time

```cpp
vector<vector<int>> permute(vector<int>& nums) {
    vector<vector<int>> result;
    backtrack(nums, 0, result);
    return result;
}

void backtrack(vector<int>& nums, int start, vector<vector<int>>& result) {
    if (start == nums.size()) {
        result.push_back(nums);
        return;
    }
    
    for (int i = start; i < nums.size(); i++) {
        swap(nums[start], nums[i]);
        backtrack(nums, start + 1, result);
        swap(nums[start], nums[i]);  // backtrack
    }
}
```

---

## Pattern 4: Combination Sum

**LeetCode 39** | O(k × 2ⁿ) time

```cpp
vector<vector<int>> combinationSum(vector<int>& candidates, int target) {
    vector<vector<int>> result;
    vector<int> current;
    backtrack(candidates, 0, target, current, result);
    return result;
}

void backtrack(vector<int>& candidates, int start, int remaining,
               vector<int>& current, vector<vector<int>>& result) {
    if (remaining == 0) {
        result.push_back(current);
        return;
    }
    
    for (int i = start; i < candidates.size(); i++) {
        if (candidates[i] > remaining) continue;
        current.push_back(candidates[i]);
        backtrack(candidates, i, remaining - candidates[i], current, result);  // i, not i+1 (reuse)
        current.pop_back();
    }
}
```

---

## Key Differences

| Pattern | Start Index | Duplicates |
|---------|-------------|------------|
| Subsets | i + 1 | No reuse |
| Subsets II | i + 1 + skip dups | Sort first |
| Permutations | swap from start | All elements used |
| Combination Sum | i (reuse) or i+1 (no reuse) | Depends on problem |

---

## Practice Problems

| Problem | Difficulty | Key Insight |
|---------|------------|-------------|
| LC 78 - Subsets | Medium | Every path is result |
| LC 90 - Subsets II | Medium | Sort + skip dups |
| LC 46 - Permutations | Medium | Swap-based |
| LC 39 - Combination Sum | Medium | Reuse allowed |
| LC 40 - Combination Sum II | Medium | No reuse, skip dups |
| LC 51 - N-Queens | Hard | Row-by-row, validate cols/diags |
