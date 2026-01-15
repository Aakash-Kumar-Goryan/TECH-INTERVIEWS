# Monotonic Stack Pattern

> **Trigger**: Next greater/smaller element, histogram problems, "visible elements"

---

## Key Insight

Maintain stack in **monotonic order** (increasing or decreasing). When new element breaks order, pop and process.

---

## Template: Next Greater Element

```cpp
vector<int> nextGreater(vector<int>& nums) {
    int n = nums.size();
    vector<int> result(n, -1);
    stack<int> st; // indices
    
    for (int i = 0; i < n; i++) {
        // Pop smaller elements, current is their "next greater"
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

## Template: Next Smaller Element

```cpp
vector<int> nextSmaller(vector<int>& nums) {
    int n = nums.size();
    vector<int> result(n, -1);
    stack<int> st;
    
    for (int i = 0; i < n; i++) {
        while (!st.empty() && nums[st.top()] > nums[i]) {
            result[st.top()] = nums[i];
            st.pop();
        }
        st.push(i);
    }
    return result;
}
```

---

## Example 1: Daily Temperatures

**LeetCode 739** | O(n) time

```cpp
vector<int> dailyTemperatures(vector<int>& temperatures) {
    int n = temperatures.size();
    vector<int> result(n, 0);
    stack<int> st;
    
    for (int i = 0; i < n; i++) {
        while (!st.empty() && temperatures[st.top()] < temperatures[i]) {
            int idx = st.top(); st.pop();
            result[idx] = i - idx;
        }
        st.push(i);
    }
    return result;
}
```

---

## Example 2: Largest Rectangle in Histogram

**LeetCode 84** | O(n) time

```cpp
int largestRectangleArea(vector<int>& heights) {
    stack<int> st;
    int maxArea = 0;
    int n = heights.size();
    
    for (int i = 0; i <= n; i++) {
        int h = (i == n) ? 0 : heights[i];
        
        while (!st.empty() && h < heights[st.top()]) {
            int height = heights[st.top()]; st.pop();
            int width = st.empty() ? i : (i - st.top() - 1);
            maxArea = max(maxArea, height * width);
        }
        st.push(i);
    }
    return maxArea;
}
```

---

## Example 3: Trapping Rain Water

**LeetCode 42** | O(n) time using stack

```cpp
int trap(vector<int>& height) {
    stack<int> st;
    int water = 0;
    
    for (int i = 0; i < height.size(); i++) {
        while (!st.empty() && height[i] > height[st.top()]) {
            int bottom = st.top(); st.pop();
            if (st.empty()) break;
            
            int width = i - st.top() - 1;
            int h = min(height[i], height[st.top()]) - height[bottom];
            water += width * h;
        }
        st.push(i);
    }
    return water;
}
```

---

## Stack Direction Summary

| Problem | Stack Type | When to Pop |
|---------|------------|-------------|
| Next Greater | Monotonic Decreasing | num > top |
| Next Smaller | Monotonic Increasing | num < top |
| Histogram Area | Monotonic Increasing | Bar shorter than top |

---

## Practice Problems

| Problem | Difficulty | Key Insight |
|---------|------------|-------------|
| LC 496 - Next Greater Element I | Easy | Use hashmap + stack |
| LC 739 - Daily Temperatures | Medium | Next greater with distance |
| LC 84 - Largest Rectangle Histogram | Hard | Width = right - left - 1 |
| LC 42 - Trapping Rain Water | Hard | Horizontal layer accumulation |
| LC 85 - Maximal Rectangle | Hard | Histogram per row |
