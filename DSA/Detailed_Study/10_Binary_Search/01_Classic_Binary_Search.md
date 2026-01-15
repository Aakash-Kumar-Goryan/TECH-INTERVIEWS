# Binary Search Pattern

> **Trigger**: Sorted array, find boundary, search space reduction, "minimum/maximum that satisfies condition"

---

## Template: Classic Binary Search

```cpp
int binarySearch(vector<int>& arr, int target) {
    int left = 0, right = arr.size() - 1;
    
    while (left <= right) {
        int mid = left + (right - left) / 2;
        
        if (arr[mid] == target) return mid;
        else if (arr[mid] < target) left = mid + 1;
        else right = mid - 1;
    }
    return -1; // not found
}
```

## Template: First/Last Occurrence (Boundary Search)

```cpp
// Find first position where condition is true
int firstTrue(vector<int>& arr, function<bool(int)> condition) {
    int left = 0, right = arr.size();
    
    while (left < right) {
        int mid = left + (right - left) / 2;
        
        if (condition(arr[mid])) right = mid;
        else left = mid + 1;
    }
    return left; // first true, or arr.size() if none
}
```

## Template: Binary Search on Answer

```cpp
// Find minimum value that satisfies condition
int binarySearchOnAnswer(int lo, int hi, function<bool(int)> feasible) {
    while (lo < hi) {
        int mid = lo + (hi - lo) / 2;
        
        if (feasible(mid)) hi = mid;
        else lo = mid + 1;
    }
    return lo;
}
```

---

## Example 1: Search in Rotated Sorted Array

**LeetCode 33** | O(log n) time

```cpp
int search(vector<int>& nums, int target) {
    int left = 0, right = nums.size() - 1;
    
    while (left <= right) {
        int mid = left + (right - left) / 2;
        
        if (nums[mid] == target) return mid;
        
        // Left half is sorted
        if (nums[left] <= nums[mid]) {
            if (nums[left] <= target && target < nums[mid]) 
                right = mid - 1;
            else 
                left = mid + 1;
        }
        // Right half is sorted
        else {
            if (nums[mid] < target && target <= nums[right]) 
                left = mid + 1;
            else 
                right = mid - 1;
        }
    }
    return -1;
}
```

---

## Example 2: Koko Eating Bananas (Binary Search on Answer)

**LeetCode 875** | O(n log m) where m = max piles

```cpp
int minEatingSpeed(vector<int>& piles, int h) {
    int lo = 1, hi = *max_element(piles.begin(), piles.end());
    
    auto canFinish = [&](int speed) {
        long hours = 0;
        for (int pile : piles) {
            hours += (pile + speed - 1) / speed; // ceil division
        }
        return hours <= h;
    };
    
    while (lo < hi) {
        int mid = lo + (hi - lo) / 2;
        if (canFinish(mid)) hi = mid;
        else lo = mid + 1;
    }
    return lo;
}
```

---

## Example 3: Find Peak Element

**LeetCode 162** | O(log n) time

```cpp
int findPeakElement(vector<int>& nums) {
    int left = 0, right = nums.size() - 1;
    
    while (left < right) {
        int mid = left + (right - left) / 2;
        
        if (nums[mid] > nums[mid + 1]) right = mid;
        else left = mid + 1;
    }
    return left;
}
```

---

## Variations

| Variant | Key Insight |
|---------|-------------|
| **lower_bound** | First element >= target |
| **upper_bound** | First element > target |
| **Rotated array** | One half is always sorted |
| **2D Matrix** | Treat as 1D: idx → (idx/cols, idx%cols) |

---

## Practice Problems

| Problem | Difficulty | Key Insight |
|---------|------------|-------------|
| LC 704 - Binary Search | Easy | Classic template |
| LC 33 - Search in Rotated Sorted Array | Medium | Identify sorted half |
| LC 875 - Koko Eating Bananas | Medium | Binary search on speed |
| LC 153 - Find Minimum in Rotated Sorted Array | Medium | Boundary with rotation |
| LC 4 - Median of Two Sorted Arrays | Hard | Binary search on partition |
