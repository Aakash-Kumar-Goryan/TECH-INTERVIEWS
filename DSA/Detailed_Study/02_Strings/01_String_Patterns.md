# String Matching & Manipulation

> **Trigger**: Pattern search, palindrome, anagrams, substring problems

---

## Palindrome Check

```cpp
bool isPalindrome(string s) {
    int left = 0, right = s.size() - 1;
    while (left < right) {
        if (s[left++] != s[right--]) return false;
    }
    return true;
}
```

---

## Longest Palindromic Substring

**LeetCode 5** | O(n²) time

```cpp
string longestPalindrome(string s) {
    int n = s.size();
    int start = 0, maxLen = 1;
    
    auto expand = [&](int left, int right) {
        while (left >= 0 && right < n && s[left] == s[right]) {
            if (right - left + 1 > maxLen) {
                start = left;
                maxLen = right - left + 1;
            }
            left--;
            right++;
        }
    };
    
    for (int i = 0; i < n; i++) {
        expand(i, i);     // Odd length
        expand(i, i + 1); // Even length
    }
    
    return s.substr(start, maxLen);
}
```

---

## Valid Anagram

**LeetCode 242** | O(n) time

```cpp
bool isAnagram(string s, string t) {
    if (s.size() != t.size()) return false;
    
    vector<int> count(26, 0);
    for (int i = 0; i < s.size(); i++) {
        count[s[i] - 'a']++;
        count[t[i] - 'a']--;
    }
    
    for (int c : count) {
        if (c != 0) return false;
    }
    return true;
}
```

---

## Minimum Window Substring

**LeetCode 76** | O(m + n) time

```cpp
string minWindow(string s, string t) {
    unordered_map<char, int> need, window;
    for (char c : t) need[c]++;
    
    int left = 0, matched = 0;
    int start = 0, minLen = INT_MAX;
    
    for (int right = 0; right < s.size(); right++) {
        char c = s[right];
        if (need.count(c)) {
            window[c]++;
            if (window[c] == need[c]) matched++;
        }
        
        while (matched == need.size()) {
            if (right - left + 1 < minLen) {
                start = left;
                minLen = right - left + 1;
            }
            
            char d = s[left++];
            if (need.count(d)) {
                if (window[d] == need[d]) matched--;
                window[d]--;
            }
        }
    }
    
    return minLen == INT_MAX ? "" : s.substr(start, minLen);
}
```

---

## KMP Pattern Matching

**O(n + m)** time

```cpp
vector<int> computeLPS(string& pattern) {
    int m = pattern.size();
    vector<int> lps(m, 0);
    int len = 0, i = 1;
    
    while (i < m) {
        if (pattern[i] == pattern[len]) {
            lps[i++] = ++len;
        } else if (len > 0) {
            len = lps[len - 1];
        } else {
            lps[i++] = 0;
        }
    }
    return lps;
}

int kmpSearch(string& text, string& pattern) {
    vector<int> lps = computeLPS(pattern);
    int n = text.size(), m = pattern.size();
    int i = 0, j = 0;
    
    while (i < n) {
        if (text[i] == pattern[j]) {
            i++; j++;
            if (j == m) return i - m; // found
        } else if (j > 0) {
            j = lps[j - 1];
        } else {
            i++;
        }
    }
    return -1;
}
```

---

## Practice Problems

| Problem | Difficulty | Key Insight |
|---------|------------|-------------|
| LC 5 - Longest Palindromic Substring | Medium | Expand from center |
| LC 76 - Minimum Window Substring | Hard | Two pointers + hashmap |
| LC 438 - Find All Anagrams | Medium | Sliding window + freq |
| LC 49 - Group Anagrams | Medium | Sort or count as key |
| LC 242 - Valid Anagram | Easy | Character count |
