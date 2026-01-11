# C++ to Java Syntax for DSA

A quick reference guide for switching from C++ to Java for Data Structures and Algorithms.

## 1. Boilerplate & Imports

**C++**

```cpp
#include <bits/stdc++.h>
using namespace std;
```

**Java**

```java
import java.util.*; // Covers most DSA needs (List, Map, Set, Stack, etc.)
import java.io.*;   // For fast I/O (BufferedReader) if needed

public class Main {
    public static void main(String[] args) {
        // Code here
    }
}
```

---

## 2. Input / Output

**C++**

```cpp
int x;
cin >> x;
cout << x << endl;
```

**Java**

```java
Scanner sc = new Scanner(System.in);
int x = sc.nextInt();
double y = sc.nextDouble();
String s = sc.next(); // Single word
String line = sc.nextLine(); // Whole line
System.out.println(x); // Prints with newline
System.out.print(x);   // Prints without newline
```

*Tip: `Scanner` is sufficient for 99% of interviews. Fast I/O (`BufferedReader`) is only needed for Competitive Programming with massive inputs.*

---

## 3. Arrays (Fixed Size)

**C++**

```cpp
int arr[5];
int arr[] = {1, 2, 3};
int n = sizeof(arr)/sizeof(arr[0]);
```

**Java**

```java
int[] arr = new int[5]; // Default initialized to 0
int[] arr = {1, 2, 3};
int n = arr.length; // Property, not a method
// Arrays.toString(arr) is useful for printing
```

---

## 4. Vectors vs ArrayList (Dynamic Arrays)

**C++**

```cpp
vector<int> v;
v.push_back(10);
v.pop_back();
int x = v[0];
int size = v.size();
```

**Java**

- Use `ArrayList` (only objects, so `Integer` not `int`).

```java
ArrayList<Integer> v = new ArrayList<>();
v.add(10);
v.remove(v.size() - 1); // remove index
int x = v.get(0); // CANNOT use v[0]
v.set(0, 20);     // v[0] = 20
int size = v.size();
```

---

## 5. Strings

**C++**

- Strings are mutable.

```cpp
string s = "hello";
s[0] = 'H';
s += " World";
```

**Java**

- Strings are **Immutable**. Modifying creates a new object.

```java
String s = "hello";
char c = s.charAt(0); // CANNOT use s[0]
// s.charAt(0) = 'H'; // ERROR
s = s + " World"; // Creates new string
```

- **For Mutable Strings (DSA Pattern):**

```java
StringBuilder sb = new StringBuilder("hello");
sb.setCharAt(0, 'H');
sb.append(" World");
String result = sb.toString();
```

---

## 6. Hash Maps

**C++ (`unordered_map`)**

```cpp
unordered_map<string, int> map;
map["apple"] = 1;
if (map.find("apple") != map.end()) ...
```

**Java (`HashMap`)**

```java
HashMap<String, Integer> map = new HashMap<>();
map.put("apple", 1);
map.get("apple"); // Returns value or null
map.getOrDefault("apple", 0); // Very useful!
if (map.containsKey("apple")) ...

// Iteration
for (String key : map.keySet()) {
    int val = map.get(key);
}
// Or
for (Map.Entry<String, Integer> entry : map.entrySet()) {
    // entry.getKey(), entry.getValue()
}
```

---

## 7. Hash Sets

**C++ (`unordered_set`)**

```cpp
unordered_set<int> st;
st.insert(10);
if (st.find(10) != st.end()) ...
```

**Java (`HashSet`)**

```java
HashSet<Integer> st = new HashSet<>();
st.add(10); // returns false if already present
if (st.contains(10)) ...
st.remove(10);
```

---

## 8. Stack & Queue

**C++**

```cpp
stack<int> s; s.push(1); s.top(); s.pop();
queue<int> q; q.push(1); q.front(); q.pop();
```

**Java**

- **Stack**: Use `Stack` class (legacy but standard in interviews) or `ArrayDeque`.

```java
Stack<Integer> s = new Stack<>();
s.push(1);
s.peek(); // Top element
s.pop();  // Returns and removes
s.isEmpty();
```

- **Queue**: `Queue` is an interface. Use `LinkedList` or `ArrayDeque`.

```java
Queue<Integer> q = new LinkedList<>();
q.offer(1); // Add (safer than add())
q.peek();   // Front element (safer than element())
q.poll();   // Remove front (safer than remove())
```

---

## 9. Priority Queue (Heap)

**C++**

- Default: **Max Heap**

```cpp
priority_queue<int> pq; // Max Heap
pq.push(10); pq.top(); pq.pop();

// Min Heap:
priority_queue<int, vector<int>, greater<int>> minPq;
```

**Java**

- Default: **Min Heap** (Opposite of C++!)

```java
PriorityQueue<Integer> pq = new PriorityQueue<>(); // Min Heap
pq.offer(10);
pq.peek(); // Top
pq.poll(); // Remove

// Max Heap:
PriorityQueue<Integer> maxPq = new PriorityQueue<>(Collections.reverseOrder());
```

---

## 10. Sorting

**C++**

```cpp
sort(arr, arr+n);
sort(v.begin(), v.end());
// Custom Comparator (Lambda)
sort(v.begin(), v.end(), [](int a, int b) { return a > b; });
```

**Java**

```java
// Arrays
Arrays.sort(arr); // Quicksort (primitives) or Mergesort (objects)

// Lists
Collections.sort(list);

// Custom Comparator (Lambda)
Collections.sort(list, (a, b) -> b - a); // Descending
// Or define concise structure sorting
Arrays.sort(intervals, (a, b) -> a[0] - b[0]); // Sort by first element
```

## 11. Pairs

C++ `pair<int,int>` is common. Java simple alternative:

```java
// 1. Use int[] (Most convenient for pairs)
int[] p = {1, 2}; 
// 2. Use built-in JavaFX Pair or Map.Entry (often unavailable)
// 3. Simple Class (Cleanest)
class Pair {
    int x, y;
    Pair(int x, int y) { this.x=x; this.y=y; }
}
```

## 12. Common Limits

**C++**
`INT_MAX`, `INT_MIN`

**Java**
`Integer.MAX_VALUE`, `Integer.MIN_VALUE`

---

## 13. Recursion & Passing State

**C++**

- Pass by reference `&` is common to update global answers.

```cpp
void solve(int n, int &ans) {
    if(n==0) return;
    ans++;
    solve(n-1, ans);
}
```

**Java**

- Java is **Always Pass by Value**.
- Primitives (`int`, `double`) are copies. Modifying them inside a function doesn't change the original variable in the caller.
- **Workarounds**:
  1. Use Class Member Variable (Global scope).
  2. Pass a wrapper object or Array `int[] ans = {0}`.
  3. Return the value.

```java
// Method 1: Global/Class State (Easiest for Interviews)
class Solution {
    int ans = 0;
    void solve(int n) {
        if(n == 0) return;
        ans++;
        solve(n-1);
    }
}

// Method 2: Array Hack (Simulates pointer/reference)
void solve(int n, int[] ans) {
    // ans[0] is mutable
    ans[0]++; 
}
```

---

## 14. Top-Down DP (Memoization)

**C++**

```cpp
int dp[100][100];
memset(dp, -1, sizeof(dp)); // Fast init

int solve(int n, int w) {
    if (dp[n][w] != -1) return dp[n][w];
    // logic
    return dp[n][w] = result;
}
```

**Java**

- No `memset` for multidimensional arrays.
- Initialize with loops.
- `Integer` (Object) arrays default to `null`. `int` (primitive) arrays default to `0`.

```java
int[][] dp; // primitive int defaults to 0

public int solveWrapper(int n, int w) {
    // Init (in constructor or main caller)
    dp = new int[n+1][w+1];
    
    // Fill with -1
    for(int[] row : dp) {
        Arrays.fill(row, -1);
    }
    
    return rec(n, w);
}

int rec(int n, int w) {
    if (dp[n][w] != -1) return dp[n][w];
    
    // logic
    int result = 1; // calculation
    return dp[n][w] = result;
}
```

### Example 1D DP Translation (Decode Ways)

**C++**

```cpp
// Passing vector by reference avoids copies
int numDecodingsUtil(string s, int idx, vector<int> &dp) {
    if(idx == s.size()) return 1;
    if(dp[idx]!=-1) return dp[idx];
    if(s[idx] == '0') return 0;
    int ans = 0;
    if(s.size() - idx>=2 && (s[idx]=='1' || (s[idx] =='2' && s[idx+1]<='6')) ) {
        ans = numDecodingsUtil(s, idx+2, dp);
    }
    dp[idx] = ans + numDecodingsUtil(s, idx+1,dp);
    return dp[idx];
}
```

**Java**

```java
// Java arrays are objects, so passing 'int[] dp' passes the reference.
// Updates inside the method affect the original array (same as vector<int> &dp).
int numDecodingsUtil(String s, int idx, int[] dp) {
    if(idx == s.length()) return 1;
    if(dp[idx] != -1) return dp[idx];
    if(s.charAt(idx) == '0') return 0; // Use charAt()
    
    int ans = 0;
    // Check bounds and characters
    if(s.length() - idx >= 2 && (s.charAt(idx) == '1' || (s.charAt(idx) == '2' && s.charAt(idx+1) <= '6'))) {
        ans = numDecodingsUtil(s, idx+2, dp);
    }
    
    dp[idx] = ans + numDecodingsUtil(s, idx+1, dp);
    return dp[idx];
}

public int numDecodings(String s) {
    int[] dp = new int[s.length()];
    Arrays.fill(dp, -1); // Simple 1D fill
    return numDecodingsUtil(s, 0, dp);
}
```
