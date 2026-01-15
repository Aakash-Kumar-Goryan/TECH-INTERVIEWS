# Heap / Priority Queue Patterns

> **Trigger**: Top K elements, K-way merge, running median, scheduling

---

## C++ Priority Queue Basics

```cpp
// Max heap (default)
priority_queue<int> maxHeap;

// Min heap
priority_queue<int, vector<int>, greater<int>> minHeap;

// Custom comparator (for pairs, sort by second element descending)
auto cmp = [](pair<int,int>& a, pair<int,int>& b) {
    return a.second < b.second; // greater means lower priority
};
priority_queue<pair<int,int>, vector<pair<int,int>>, decltype(cmp)> pq(cmp);
```

---

## Pattern 1: Top K Elements

```cpp
// Top K largest - use min heap of size K
vector<int> topKLargest(vector<int>& nums, int k) {
    priority_queue<int, vector<int>, greater<int>> minHeap;
    
    for (int num : nums) {
        minHeap.push(num);
        if (minHeap.size() > k) minHeap.pop();
    }
    
    vector<int> result;
    while (!minHeap.empty()) {
        result.push_back(minHeap.top());
        minHeap.pop();
    }
    return result; // K largest elements
}
```

---

## Example 1: Kth Largest Element

**LeetCode 215** | O(n log k) time

```cpp
int findKthLargest(vector<int>& nums, int k) {
    priority_queue<int, vector<int>, greater<int>> minHeap;
    
    for (int num : nums) {
        minHeap.push(num);
        if (minHeap.size() > k) minHeap.pop();
    }
    return minHeap.top();
}
```

---

## Example 2: Top K Frequent Elements

**LeetCode 347** | O(n log k) time

```cpp
vector<int> topKFrequent(vector<int>& nums, int k) {
    unordered_map<int, int> freq;
    for (int num : nums) freq[num]++;
    
    // Min heap by frequency
    auto cmp = [](pair<int,int>& a, pair<int,int>& b) {
        return a.second > b.second;
    };
    priority_queue<pair<int,int>, vector<pair<int,int>>, decltype(cmp)> pq(cmp);
    
    for (auto& [num, f] : freq) {
        pq.push({num, f});
        if (pq.size() > k) pq.pop();
    }
    
    vector<int> result;
    while (!pq.empty()) {
        result.push_back(pq.top().first);
        pq.pop();
    }
    return result;
}
```

---

## Pattern 2: Merge K Sorted Lists

**LeetCode 23** | O(n log k) time

```cpp
ListNode* mergeKLists(vector<ListNode*>& lists) {
    auto cmp = [](ListNode* a, ListNode* b) {
        return a->val > b->val;
    };
    priority_queue<ListNode*, vector<ListNode*>, decltype(cmp)> pq(cmp);
    
    for (auto list : lists) {
        if (list) pq.push(list);
    }
    
    ListNode dummy(0);
    ListNode* curr = &dummy;
    
    while (!pq.empty()) {
        ListNode* node = pq.top(); pq.pop();
        curr->next = node;
        curr = curr->next;
        if (node->next) pq.push(node->next);
    }
    return dummy.next;
}
```

---

## Pattern 3: Running Median (Two Heaps)

**LeetCode 295** | O(log n) per add

```cpp
class MedianFinder {
    priority_queue<int> maxHeap; // left half
    priority_queue<int, vector<int>, greater<int>> minHeap; // right half
    
public:
    void addNum(int num) {
        maxHeap.push(num);
        minHeap.push(maxHeap.top());
        maxHeap.pop();
        
        if (minHeap.size() > maxHeap.size()) {
            maxHeap.push(minHeap.top());
            minHeap.pop();
        }
    }
    
    double findMedian() {
        if (maxHeap.size() > minHeap.size()) {
            return maxHeap.top();
        }
        return (maxHeap.top() + minHeap.top()) / 2.0;
    }
};
```

---

## When to Use Which Heap

| Goal | Heap Type | Why |
|------|-----------|-----|
| K largest | Min heap of size K | Pop smallest, keep K largest |
| K smallest | Max heap of size K | Pop largest, keep K smallest |
| Merge sorted | Min heap | Always extract minimum |
| Running max | Max heap | Direct access to max |

---

## Practice Problems

| Problem | Difficulty | Key Insight |
|---------|------------|-------------|
| LC 215 - Kth Largest | Medium | Min heap size K |
| LC 347 - Top K Frequent | Medium | Heap with freq pairs |
| LC 23 - Merge K Sorted Lists | Hard | K-way merge |
| LC 295 - Find Median | Hard | Two heaps |
| LC 973 - K Closest Points | Medium | Max heap by distance |
