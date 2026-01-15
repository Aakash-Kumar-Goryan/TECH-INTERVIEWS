# Valid Parentheses & Expression Evaluation

> **Trigger**: Matching brackets, nested structures, evaluate expressions

---

## Valid Parentheses

**LeetCode 20** | O(n) time

```cpp
bool isValid(string s) {
    stack<char> st;
    unordered_map<char, char> pairs = {
        {')', '('}, {']', '['}, {'}', '{'}
    };
    
    for (char c : s) {
        if (pairs.count(c)) {
            if (st.empty() || st.top() != pairs[c]) 
                return false;
            st.pop();
        } else {
            st.push(c);
        }
    }
    return st.empty();
}
```

---

## Minimum Remove to Make Valid

**LeetCode 1249** | O(n) time

```cpp
string minRemoveToMakeValid(string s) {
    stack<int> openIdx;
    unordered_set<int> toRemove;
    
    for (int i = 0; i < s.size(); i++) {
        if (s[i] == '(') {
            openIdx.push(i);
        } else if (s[i] == ')') {
            if (openIdx.empty()) {
                toRemove.insert(i);
            } else {
                openIdx.pop();
            }
        }
    }
    
    while (!openIdx.empty()) {
        toRemove.insert(openIdx.top());
        openIdx.pop();
    }
    
    string result;
    for (int i = 0; i < s.size(); i++) {
        if (!toRemove.count(i)) result += s[i];
    }
    return result;
}
```

---

## Generate Parentheses

**LeetCode 22** | O(4^n / √n) time (Catalan number)

```cpp
vector<string> generateParenthesis(int n) {
    vector<string> result;
    backtrack(result, "", 0, 0, n);
    return result;
}

void backtrack(vector<string>& result, string current, 
               int open, int close, int n) {
    if (current.size() == 2 * n) {
        result.push_back(current);
        return;
    }
    
    if (open < n) 
        backtrack(result, current + "(", open + 1, close, n);
    if (close < open) 
        backtrack(result, current + ")", open, close + 1, n);
}
```

---

## Basic Calculator II

**LeetCode 227** | O(n) time

```cpp
int calculate(string s) {
    stack<int> nums;
    int num = 0;
    char op = '+';
    
    for (int i = 0; i <= s.size(); i++) {
        char c = (i < s.size()) ? s[i] : '+';
        
        if (isdigit(c)) {
            num = num * 10 + (c - '0');
        } else if (c != ' ') {
            if (op == '+') nums.push(num);
            else if (op == '-') nums.push(-num);
            else if (op == '*') {
                int top = nums.top(); nums.pop();
                nums.push(top * num);
            }
            else if (op == '/') {
                int top = nums.top(); nums.pop();
                nums.push(top / num);
            }
            op = c;
            num = 0;
        }
    }
    
    int result = 0;
    while (!nums.empty()) {
        result += nums.top();
        nums.pop();
    }
    return result;
}
```

---

## Longest Valid Parentheses

**LeetCode 32** | O(n) time

```cpp
int longestValidParentheses(string s) {
    stack<int> st;
    st.push(-1);  // Base for counting
    int maxLen = 0;
    
    for (int i = 0; i < s.size(); i++) {
        if (s[i] == '(') {
            st.push(i);
        } else {
            st.pop();
            if (st.empty()) {
                st.push(i);  // New base
            } else {
                maxLen = max(maxLen, i - st.top());
            }
        }
    }
    return maxLen;
}
```

---

## Practice Problems

| Problem | Difficulty | Key Insight |
|---------|------------|-------------|
| LC 20 - Valid Parentheses | Easy | Stack matching |
| LC 22 - Generate Parentheses | Medium | Backtrack with open/close count |
| LC 32 - Longest Valid | Hard | Stack stores indices |
| LC 227 - Basic Calculator II | Medium | Handle */ immediately |
| LC 1249 - Min Remove | Medium | Track unmatched indices |
