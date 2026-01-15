# Sliding Window Pattern

> **Trigger**: Contiguous subarray/substring with constraint (max/min sum, at most K distinct, etc.)

---

## Template (Variable Size Window)

```cpp
int slidingWindow(vector<int>& arr, int target) {
    int left = 0, result = 0;
    int windowSum = 0; // or windowState
    
    for (int right = 0; right < arr.size(); right++) {
        // Expand: add arr[right] to window
        windowSum += arr[right];
        
        // Shrink: while window invalid
        while (windowSum > target) { // or condition violated
            windowSum -= arr[left];
            left++;
        }
        
        // Update result
        result = max(result, right - left + 1);
    }
    return result;
}
```

## Template (Fixed Size Window)

```cpp
int fixedWindow(vector<int>& arr, int k) {
    int windowSum = 0, maxSum = 0;
    
    for (int i = 0; i < arr.size(); i++) {
        windowSum += arr[i];
        
        if (i >= k - 1) {
            maxSum = max(maxSum, windowSum);
            windowSum -= arr[i - k + 1];
        }
    }
    return maxSum;
}
```

---

## Example 1: Longest Substring Without Repeating Characters

**LeetCode 3** | O(n) time, O(min(n,26)) space

```cpp
int lengthOfLongestSubstring(string s) {
    unordered_set<char> window;
    int left = 0, maxLen = 0;
    
    for (int right = 0; right < s.size(); right++) {
        while (window.count(s[right])) {
            window.erase(s[left]);
            left++;
        }
        window.insert(s[right]);
        maxLen = max(maxLen, right - left + 1);
    }
    return maxLen;
}
```

---

## Example 2: Minimum Size Subarray Sum

**LeetCode 209** | O(n) time, O(1) space

```cpp
int minSubArrayLen(int target, vector<int>& nums) {
    int left = 0, sum = 0;
    int minLen = INT_MAX;
    
    for (int right = 0; right < nums.size(); right++) {
        sum += nums[right];
        
        while (sum >= target) {
            minLen = min(minLen, right - left + 1);
            sum -= nums[left++];
        }
    }
    return minLen == INT_MAX ? 0 : minLen;
}
```

---

## Variations

| Variant | Modification |
|---------|-------------|
| **At Most K distinct** | Use hashmap to track counts, shrink when map.size() > K |
| **Exactly K distinct** | atMost(K) - atMost(K-1) |
| **Max consecutive ones with K flips** | Track zeros count, shrink when > K |

---

## Practice Problems

| Problem | Difficulty | Key Insight |
|---------|------------|-------------|
| LC 3 - Longest Substring Without Repeating | Medium | Set for uniqueness |
| LC 209 - Minimum Size Subarray Sum | Medium | Variable window, shrink when valid |
| LC 76 - Minimum Window Substring | Hard | Two hashmaps, track matched count |
| LC 424 - Longest Repeating Character Replacement | Medium | maxFreq trick |
| LC 567 - Permutation in String | Medium | Fixed window with freq match |
