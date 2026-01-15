# Prefix Sum Pattern

> **Trigger**: Range sum queries, subarray sum equals K, cumulative calculations

---

## Template: Basic Prefix Sum

```cpp
// Build prefix sum array
vector<int> buildPrefixSum(vector<int>& arr) {
    int n = arr.size();
    vector<int> prefix(n + 1, 0);
    
    for (int i = 0; i < n; i++) {
        prefix[i + 1] = prefix[i] + arr[i];
    }
    return prefix;
}

// Query sum of range [left, right] (0-indexed)
int rangeSum(vector<int>& prefix, int left, int right) {
    return prefix[right + 1] - prefix[left];
}
```

---

## Example 1: Subarray Sum Equals K

**LeetCode 560** | O(n) time, O(n) space

```cpp
int subarraySum(vector<int>& nums, int k) {
    unordered_map<int, int> prefixCount;
    prefixCount[0] = 1; // empty prefix
    
    int sum = 0, count = 0;
    
    for (int num : nums) {
        sum += num;
        
        // If (sum - k) exists, we found subarrays ending here
        if (prefixCount.count(sum - k)) {
            count += prefixCount[sum - k];
        }
        
        prefixCount[sum]++;
    }
    return count;
}
```

**Key Insight**: If `prefix[j] - prefix[i] = k`, then subarray `[i+1, j]` sums to k.

---

## Example 2: Product of Array Except Self

**LeetCode 238** | O(n) time, O(1) extra space

```cpp
vector<int> productExceptSelf(vector<int>& nums) {
    int n = nums.size();
    vector<int> result(n, 1);
    
    // Left products
    int leftProduct = 1;
    for (int i = 0; i < n; i++) {
        result[i] = leftProduct;
        leftProduct *= nums[i];
    }
    
    // Right products
    int rightProduct = 1;
    for (int i = n - 1; i >= 0; i--) {
        result[i] *= rightProduct;
        rightProduct *= nums[i];
    }
    
    return result;
}
```

---

## Example 3: Continuous Subarray Sum (Multiple of K)

**LeetCode 523** | O(n) time, O(k) space

```cpp
bool checkSubarraySum(vector<int>& nums, int k) {
    unordered_map<int, int> modIndex;
    modIndex[0] = -1; // prefix sum 0 at index -1
    
    int sum = 0;
    for (int i = 0; i < nums.size(); i++) {
        sum += nums[i];
        int mod = sum % k;
        
        if (modIndex.count(mod)) {
            if (i - modIndex[mod] >= 2) return true;
        } else {
            modIndex[mod] = i;
        }
    }
    return false;
}
```

**Key Insight**: If two prefix sums have same remainder, the subarray between them is divisible by k.

---

## 2D Prefix Sum

```cpp
// Build 2D prefix sum
vector<vector<int>> build2DPrefix(vector<vector<int>>& matrix) {
    int m = matrix.size(), n = matrix[0].size();
    vector<vector<int>> prefix(m + 1, vector<int>(n + 1, 0));
    
    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= n; j++) {
            prefix[i][j] = matrix[i-1][j-1] 
                         + prefix[i-1][j] 
                         + prefix[i][j-1] 
                         - prefix[i-1][j-1];
        }
    }
    return prefix;
}

// Query sum of rectangle (r1,c1) to (r2,c2)
int rectSum(vector<vector<int>>& prefix, int r1, int c1, int r2, int c2) {
    return prefix[r2+1][c2+1] - prefix[r1][c2+1] - prefix[r2+1][c1] + prefix[r1][c1];
}
```

---

## Practice Problems

| Problem | Difficulty | Key Insight |
|---------|------------|-------------|
| LC 303 - Range Sum Query | Easy | Basic prefix sum |
| LC 560 - Subarray Sum Equals K | Medium | HashMap + prefix |
| LC 238 - Product Except Self | Medium | Left & right prefix products |
| LC 304 - Range Sum Query 2D | Medium | 2D prefix sum |
| LC 523 - Continuous Subarray Sum | Medium | Mod-based prefix |
