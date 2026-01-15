# Hashing Patterns

> **Trigger**: O(1) lookup, counting, grouping by key, two sum variants

---

## Two Sum (Classic HashMap)

**LeetCode 1** | O(n) time, O(n) space

```cpp
vector<int> twoSum(vector<int>& nums, int target) {
    unordered_map<int, int> seen; // value -> index
    
    for (int i = 0; i < nums.size(); i++) {
        int complement = target - nums[i];
        if (seen.count(complement)) {
            return {seen[complement], i};
        }
        seen[nums[i]] = i;
    }
    return {};
}
```

---

## Group Anagrams

**LeetCode 49** | O(n × k log k) time

```cpp
vector<vector<string>> groupAnagrams(vector<string>& strs) {
    unordered_map<string, vector<string>> groups;
    
    for (string& s : strs) {
        string key = s;
        sort(key.begin(), key.end());
        groups[key].push_back(s);
    }
    
    vector<vector<string>> result;
    for (auto& [key, group] : groups) {
        result.push_back(group);
    }
    return result;
}
```

**Optimized O(n × k)** using char count as key:

```cpp
string getKey(string& s) {
    vector<int> count(26, 0);
    for (char c : s) count[c - 'a']++;
    
    string key;
    for (int i = 0; i < 26; i++) {
        key += to_string(count[i]) + "#";
    }
    return key;
}
```

---

## Contains Duplicate II (Within K Distance)

**LeetCode 219** | O(n) time

```cpp
bool containsNearbyDuplicate(vector<int>& nums, int k) {
    unordered_map<int, int> lastIndex;
    
    for (int i = 0; i < nums.size(); i++) {
        if (lastIndex.count(nums[i]) && i - lastIndex[nums[i]] <= k) {
            return true;
        }
        lastIndex[nums[i]] = i;
    }
    return false;
}
```

---

## Longest Consecutive Sequence

**LeetCode 128** | O(n) time

```cpp
int longestConsecutive(vector<int>& nums) {
    unordered_set<int> numSet(nums.begin(), nums.end());
    int longest = 0;
    
    for (int num : numSet) {
        // Only start counting from sequence start
        if (!numSet.count(num - 1)) {
            int current = num;
            int length = 1;
            
            while (numSet.count(current + 1)) {
                current++;
                length++;
            }
            longest = max(longest, length);
        }
    }
    return longest;
}
```

---

## Valid Sudoku

**LeetCode 36** | O(1) time (fixed 9x9)

```cpp
bool isValidSudoku(vector<vector<char>>& board) {
    unordered_set<string> seen;
    
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (board[i][j] == '.') continue;
            char c = board[i][j];
            
            string row = "row" + to_string(i) + c;
            string col = "col" + to_string(j) + c;
            string box = "box" + to_string(i/3) + to_string(j/3) + c;
            
            if (seen.count(row) || seen.count(col) || seen.count(box))
                return false;
            
            seen.insert(row);
            seen.insert(col);
            seen.insert(box);
        }
    }
    return true;
}
```

---

## Practice Problems

| Problem | Difficulty | Key Insight |
|---------|------------|-------------|
| LC 1 - Two Sum | Easy | HashMap complement |
| LC 49 - Group Anagrams | Medium | Sorted string as key |
| LC 128 - Longest Consecutive | Medium | Start from sequence head |
| LC 560 - Subarray Sum K | Medium | Prefix sum + count map |
| LC 380 - Insert Delete Random O(1) | Medium | HashMap + vector swap |
