# Greedy - Interval Scheduling

> **Trigger**: Maximize meetings/tasks, minimize overlaps, interval selection

---

## Key Insight

**Sort by end time** → Always pick the interval that finishes earliest, leaving maximum room for future picks.

---

## Template: Activity Selection

```cpp
int maxActivities(vector<pair<int,int>>& intervals) {
    // Sort by end time
    sort(intervals.begin(), intervals.end(), [](auto& a, auto& b) {
        return a.second < b.second;
    });
    
    int count = 1;
    int lastEnd = intervals[0].second;
    
    for (int i = 1; i < intervals.size(); i++) {
        if (intervals[i].first >= lastEnd) {
            count++;
            lastEnd = intervals[i].second;
        }
    }
    return count;
}
```

---

## Example 1: Non-overlapping Intervals (Remove Minimum)

**LeetCode 435** | O(n log n) time

```cpp
int eraseOverlapIntervals(vector<vector<int>>& intervals) {
    sort(intervals.begin(), intervals.end(), [](auto& a, auto& b) {
        return a[1] < b[1];
    });
    
    int kept = 1;
    int lastEnd = intervals[0][1];
    
    for (int i = 1; i < intervals.size(); i++) {
        if (intervals[i][0] >= lastEnd) {
            kept++;
            lastEnd = intervals[i][1];
        }
    }
    return intervals.size() - kept;
}
```

---

## Example 2: Merge Intervals

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

## Example 3: Meeting Rooms II (Minimum Rooms)

**LeetCode 253** | O(n log n) time

```cpp
int minMeetingRooms(vector<vector<int>>& intervals) {
    vector<int> starts, ends;
    for (auto& i : intervals) {
        starts.push_back(i[0]);
        ends.push_back(i[1]);
    }
    sort(starts.begin(), starts.end());
    sort(ends.begin(), ends.end());
    
    int rooms = 0, endPtr = 0;
    for (int start : starts) {
        if (start < ends[endPtr]) {
            rooms++;
        } else {
            endPtr++;
        }
    }
    return rooms;
}
```

---

## Example 4: Maximize Meetings with Flexible Scheduling

```cpp
int maxMeetingPossible(vector<int> firstDay, vector<int> lastDay) {
    int n = firstDay.size();
    vector<pair<int,int>> investors;

    for (int i = 0; i < n; i++) {
        investors.push_back({lastDay[i], firstDay[i]});
    }

    sort(investors.begin(), investors.end());

    int currentDay = 0;
    int meetings = 0;

    for (auto &inv : investors) {
        int end = inv.first;
        int start = inv.second;

        int meetingDay = max(currentDay + 1, start);
        if (meetingDay <= end) {
            meetings++;
            currentDay = meetingDay;
        }
    }
    return meetings;
}
```

---

## Practice Problems

| Problem | Difficulty | Key Insight |
|---------|------------|-------------|
| LC 435 - Non-overlapping Intervals | Medium | Max kept = n - removed |
| LC 56 - Merge Intervals | Medium | Sort by start, merge |
| LC 253 - Meeting Rooms II | Medium | Two-pointer on sorted starts/ends |
| LC 452 - Minimum Arrows | Medium | Like activity selection |
| LC 1353 - Maximum Events | Medium | Priority queue + greedy |
