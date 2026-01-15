# Two Pointers Pattern

> **Trigger**: Sorted array, pair/triplet finding, partitioning, palindrome check

---

## Template: Opposite Ends (Sorted Array)

```cpp
// Find pair with target sum in sorted array
pair<int,int> twoSum(vector<int>& arr, int target) {
    int left = 0, right = arr.size() - 1;
    
    while (left < right) {
        int sum = arr[left] + arr[right];
        
        if (sum == target) return {left, right};
        else if (sum < target) left++;
        else right--;
    }
    return {-1, -1}; // not found
}
```

## Template: Same Direction (Fast & Slow)

```cpp
// Remove duplicates in-place
int removeDuplicates(vector<int>& nums) {
    if (nums.empty()) return 0;
    
    int slow = 0;
    for (int fast = 1; fast < nums.size(); fast++) {
        if (nums[fast] != nums[slow]) {
            slow++;
            nums[slow] = nums[fast];
        }
    }
    return slow + 1;
}
```

---

## Example 1: 3Sum

**LeetCode 15** | O(n²) time, O(1) space

```cpp
vector<vector<int>> threeSum(vector<int>& nums) {
    vector<vector<int>> result;
    sort(nums.begin(), nums.end());
    
    for (int i = 0; i < nums.size(); i++) {
        if (i > 0 && nums[i] == nums[i-1]) continue; // skip dups
        
        int left = i + 1, right = nums.size() - 1;
        while (left < right) {
            int sum = nums[i] + nums[left] + nums[right];
            
            if (sum == 0) {
                result.push_back({nums[i], nums[left], nums[right]});
                while (left < right && nums[left] == nums[left+1]) left++;
                while (left < right && nums[right] == nums[right-1]) right--;
                left++; right--;
            }
            else if (sum < 0) left++;
            else right--;
        }
    }
    return result;
}
```

---

## Example 2: Container With Most Water

**LeetCode 11** | O(n) time, O(1) space

```cpp
int maxArea(vector<int>& height) {
    int left = 0, right = height.size() - 1;
    int maxWater = 0;
    
    while (left < right) {
        int h = min(height[left], height[right]);
        int w = right - left;
        maxWater = max(maxWater, h * w);
        
        // Move the shorter line
        if (height[left] < height[right]) left++;
        else right--;
    }
    return maxWater;
}
```

---

## Example 3: Valid Palindrome

**LeetCode 125** | O(n) time, O(1) space

```cpp
bool isPalindrome(string s) {
    int left = 0, right = s.size() - 1;
    
    while (left < right) {
        while (left < right && !isalnum(s[left])) left++;
        while (left < right && !isalnum(s[right])) right--;
        
        if (tolower(s[left]) != tolower(s[right])) return false;
        left++; right--;
    }
    return true;
}
```

---

## Variations

| Variant | Approach |
|---------|----------|
| **4Sum** | Fix two, two-pointer on rest → O(n³) |
| **Trapping Rain Water** | Two pointers with maxLeft/maxRight |
| **Sort Colors (Dutch Flag)** | Three pointers: low, mid, high |

---

## Practice Problems

| Problem | Difficulty | Key Insight |
|---------|------------|-------------|
| LC 167 - Two Sum II | Easy | Sorted input, opposite ends |
| LC 15 - 3Sum | Medium | Fix one, two-pointer for rest |
| LC 11 - Container With Most Water | Medium | Move shorter side |
| LC 42 - Trapping Rain Water | Hard | Track max heights both sides |
| LC 75 - Sort Colors | Medium | Dutch National Flag |
