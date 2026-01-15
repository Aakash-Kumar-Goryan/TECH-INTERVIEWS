# Intervals & Merge Patterns

> **Trigger**: Overlapping intervals, merge, insert, count overlaps

---

## Merge Intervals

**LeetCode 56** | O(n log n) time

```cpp
vector<vector<int>> merge(vector<vector<int>>& intervals) {
    sort(intervals.begin(), intervals.end());
    vector<vector<int>> result;
    
    for (auto& interval : intervals) {
        if (result.empty() || result.back()[1] < interval[0]) {
            result.push_back(interval);
        } else {
            result.back()[1] = max(result.back()[1], interval[1]);
        }
    }
    return result;
}
```

---

## Insert Interval

**LeetCode 57** | O(n) time

```cpp
vector<vector<int>> insert(vector<vector<int>>& intervals, vector<int>& newInterval) {
    vector<vector<int>> result;
    int i = 0, n = intervals.size();
    
    // Add all intervals before newInterval
    while (i < n && intervals[i][1] < newInterval[0]) {
        result.push_back(intervals[i++]);
    }
    
    // Merge overlapping intervals
    while (i < n && intervals[i][0] <= newInterval[1]) {
        newInterval[0] = min(newInterval[0], intervals[i][0]);
        newInterval[1] = max(newInterval[1], intervals[i][1]);
        i++;
    }
    result.push_back(newInterval);
    
    // Add remaining intervals
    while (i < n) {
        result.push_back(intervals[i++]);
    }
    return result;
}
```

---

## Interval Intersection

**LeetCode 986** | O(m + n) time

```cpp
vector<vector<int>> intervalIntersection(
    vector<vector<int>>& A, vector<vector<int>>& B) {
    
    vector<vector<int>> result;
    int i = 0, j = 0;
    
    while (i < A.size() && j < B.size()) {
        int lo = max(A[i][0], B[j][0]);
        int hi = min(A[i][1], B[j][1]);
        
        if (lo <= hi) {
            result.push_back({lo, hi});
        }
        
        // Advance the one that ends first
        if (A[i][1] < B[j][1]) i++;
        else j++;
    }
    return result;
}
```

---

## Non-overlapping Intervals (Minimum Removals)

**LeetCode 435** | O(n log n) time

```cpp
int eraseOverlapIntervals(vector<vector<int>>& intervals) {
    sort(intervals.begin(), intervals.end(), [](auto& a, auto& b) {
        return a[1] < b[1];  // Sort by end time
    });
    
    int count = 0, prevEnd = INT_MIN;
    
    for (auto& interval : intervals) {
        if (interval[0] >= prevEnd) {
            prevEnd = interval[1];  // Keep this interval
        } else {
            count++;  // Remove this interval
        }
    }
    return count;
}
```

---

## Meeting Rooms (Can Attend All?)

**LeetCode 252** | O(n log n) time

```cpp
bool canAttendMeetings(vector<vector<int>>& intervals) {
    sort(intervals.begin(), intervals.end());
    
    for (int i = 1; i < intervals.size(); i++) {
        if (intervals[i][0] < intervals[i-1][1]) {
            return false;  // Overlap
        }
    }
    return true;
}
```

---

## Meeting Rooms II (Minimum Rooms)

**LeetCode 253** | O(n log n) time

```cpp
int minMeetingRooms(vector<vector<int>>& intervals) {
    vector<pair<int, int>> events;
    
    for (auto& i : intervals) {
        events.push_back({i[0], 1});   // Start: +1
        events.push_back({i[1], -1});  // End: -1
    }
    
    sort(events.begin(), events.end());
    
    int rooms = 0, maxRooms = 0;
    for (auto& [time, delta] : events) {
        rooms += delta;
        maxRooms = max(maxRooms, rooms);
    }
    return maxRooms;
}
```

---

## Practice Problems

| Problem | Difficulty | Key Insight |
|---------|------------|-------------|
| LC 56 - Merge Intervals | Medium | Sort by start, merge |
| LC 57 - Insert Interval | Medium | Three phases |
| LC 435 - Non-overlapping | Medium | Greedy, sort by end |
| LC 253 - Meeting Rooms II | Medium | Sweep line or heap |
| LC 986 - Interval Intersection | Medium | Two pointers |
