# Interval Scheduling - Maximize Meetings

> **Tags:** `#greedy` `#intervals` `#scheduling` `#sorting`
>
> **Category:** Greedy Scheduling | **Difficulty:** Medium | **Pattern:** Earliest Finish First

---

## Problem Statement

Each investor is available from `firstDay[i]` to `lastDay[i]` (inclusive).

**Constraints:**

- Owner can do **only one meeting per day**
- Each investor needs **exactly one meeting**

**Goal:** Maximize the number of meetings.

You can choose **any one day** within each investor's range.

---

## Key Idea 💡 (Greedy)

To maximize total meetings:

> **Always schedule meetings as early as possible.**

### Why?

If you delay a meeting unnecessarily, you might block another investor who has a **tighter range**.

---

## Correct Greedy Strategy

1. Create intervals: `(firstDay[i], lastDay[i])`
2. **Sort investors by `lastDay`** (earliest finish first)
3. Maintain a variable `currentDay`
4. For each investor:
   - Schedule meeting on `max(currentDay, firstDay)`
   - If this day ≤ `lastDay`, accept the meeting and move to next day

---

## Walkthrough Example

```
firstDay = [1, 2, 3]
lastDay  = [2, 2, 3]

Sorted by lastDay:
(1,2), (2,2), (3,3)

Day 1 → investor 1 ✅
Day 2 → investor 2 ✅
Day 3 → investor 3 ✅

Answer = 3
```

---

## C++ Implementation ✅

```cpp
int maxMeetingPossible(vector<int> firstDay, vector<int> lastDay) {
    int n = firstDay.size();
    vector<pair<int,int>> investors;

    for (int i = 0; i < n; i++) {
        investors.push_back({lastDay[i], firstDay[i]});
    }

    // Sort by earliest lastDay
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

## Time & Space Complexity

| Metric | Complexity |
|--------|------------|
| **Time** | O(n log n) — sorting dominates |
| **Space** | O(n) — storing investor pairs |

---

## Related Problems

- **LeetCode 435:** Non-overlapping Intervals
- **LeetCode 452:** Minimum Number of Arrows to Burst Balloons
- **Activity Selection Problem** (Classic Greedy)

---

## Interview Tips 🎯

1. **Recognize the pattern:** When you see "maximize selections from intervals" → think **Greedy + Sort by end time**
2. **Explain the intuition:** Picking earliest-ending intervals leaves maximum room for future picks
3. **Edge cases to mention:**
   - Empty input
   - All intervals overlap on same day
   - Non-overlapping intervals (answer = n)
