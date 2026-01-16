# C++ STL Quick Syntax Revision

## 1. Vectors (`std::vector`)

Dynamic array.
**Header:** `<vector>`

```cpp
vector<int> v = {1, 2, 3};
vector<int> v2(10, 5); // 10 elements, all 5
int arr[] = {1, 2, 3};
vector<int> v3(arr, arr + 3); // From array

// Access
v[0];      // No bounds check - O(1)
v.at(0);   // Bounds check - O(1)
v.front(); // First element - O(1)
v.back();  // Last element - O(1)

// Modifiers
v.push_back(4);    // Add to end - O(1) amortized
v.pop_back();      // Remove last - O(1)
v.emplace_back(5); // Construct in place (faster than push_back for objects)
v.insert(v.begin() + 1, 99); // Insert at index 1 - O(N)
v.erase(v.begin() + 1);      // Erase at index 1 - O(N)
v.clear();                   // Remove all - O(N)
v.resize(20);                // Resize to 20

// Iteration
for(int x : v) { ... }
for(auto it = v.begin(); it != v.end(); ++it) { cout << *it; }
```

## 2. Pairs & Tuples

**Header:** `<utility>`, `<tuple>`

```cpp
// Pair
pair<int, string> p = {1, "abc"};
p = make_pair(2, "def");
cout << p.first << " " << p.second;

// Tuple
tuple<int, string, double> t = {1, "a", 3.14};
cout << get<0>(t); // Access first element
// Tie (Unpacking)
int a; string b; double c;
tie(a, b, c) = t; 
```

## 3. Stack & Queue

**Header:** `<stack>`, `<queue>`
No iterators, no random access.

```cpp
// Stack (LIFO)
stack<int> s;
s.push(1);    // O(1)
s.pop();      // O(1) - returns void!
s.top();      // O(1) - returns element
s.empty();    // O(1)
s.size();     // O(1)

// Queue (FIFO)
queue<int> q;
q.push(1);    // O(1)
q.pop();      // O(1)
q.front();    // O(1) - accesses first element
q.back();     // O(1) - accesses last element
```

## 4. Priority Queue (Heap)

**Header:** `<queue>`
By default: **Max Heap**.

```cpp
priority_queue<int> pq; // Max Heap
pq.push(10);  // O(log N)
pq.push(5);
int top = pq.top(); // 10 - O(1)
pq.pop();           // Removes 10 - O(log N)

// Min Heap
priority_queue<int, vector<int>, greater<int>> min_pq;

// Custom Comparator (e.g., Min Heap for Pairs based on second element)
struct Compare {
    bool operator()(pair<int, int> a, pair<int, int> b) {
        return a.second > b.second; // Min heap logic (reverse of normal sort)
    }
};
priority_queue<pair<int,int>, vector<pair<int,int>>, Compare> custom_pq;
```

## 5. Sets (`std::set` / `std::unordered_set`)

Unique elements only.
**Header:** `<set>`, `<unordered_set>`

| Feature | `set` | `unordered_set` |
| :--- | :--- | :--- |
| **Order** | Sorted (BST) | Random (Hash Table) |
| **Insert/Delete/Search** | O(log N) | O(1) avg, O(N) worst |
| **Header** | `<set>` | `<unordered_set>` |

```cpp
set<int> s = {3, 1, 4, 1}; // Stores {1, 3, 4}
s.insert(5);      
s.erase(3);       // Erase by value
auto it = s.find(4); // Returns iterator or s.end()
if (s.count(4)) { ... } // Returns 1 if present, 0 if not

// Lower/Upper Bound (Set only)
// lower_bound(val): First element >= val
// upper_bound(val): First element > val
auto it_lb = s.lower_bound(2); 
```

## 6. Maps (`std::map` / `std::unordered_map`)

Key-Value pairs. Keys are unique.
**Header:** `<map>`, `<unordered_map>`

| Feature | `map` | `unordered_map` |
| :--- | :--- | :--- |
| **Order** | Sorted by Key | Random |
| **Ops Complexity** | O(log N) | O(1) avg |

```cpp
unordered_map<string, int> m;
m["apple"] = 5;      // Insert or Update O(1)
m.insert({"banana", 3}); 

// Access
int x = m["apple"];  // Returns value. !!! IF KEY NOT PRESENT, CREATES IT WITH DEFAULT VALUE (0) !!!
int y = m.at("apple"); // Throws exception if key not found

// Check existence
if (m.find("apple") != m.end()) { ... }
if (m.count("apple")) { ... }

// Iterate
for (auto const& [key, val] : m) {
    cout << key << ":" << val;
}
```

## 7. Algorithms

**Header:** `<algorithm>`, `<numeric>`

```cpp
vector<int> v = {4, 2, 5, 1, 3};

// Sorting
sort(v.begin(), v.end()); // O(N log N)
sort(v.begin(), v.end(), greater<int>()); // Descending

// Binary Search (Vector must be sorted!)
bool exists = binary_search(v.begin(), v.end(), 3); // true/false
auto lb = lower_bound(v.begin(), v.end(), 3); // First element >= 3
auto ub = upper_bound(v.begin(), v.end(), 3); // First element > 3
// Index: lb - v.begin()

// Min/Max
int min_val = *min_element(v.begin(), v.end()); // O(N)
int max_val = *max_element(v.begin(), v.end());

// Sum (Header: <numeric>)
int sum = accumulate(v.begin(), v.end(), 0); // O(N)

// Permutations
next_permutation(v.begin(), v.end()); // Changes v to next lexicographical permutation

// Reverse
reverse(v.begin(), v.end());

// Count/Find
int c = count(v.begin(), v.end(), 3); // O(N)
auto it = find(v.begin(), v.end(), 3); // O(N)
```

## 8. Common Methods & Tricks

```cpp
// String to Int / Int to String
string s = to_string(123);
int i = stoi("123");

// Character functions (<cctype>)
isalnum(c); isalpha(c); isdigit(c); tolower(c); toupper(c);

// Fast I/O
ios_base::sync_with_stdio(false);
cin.tie(NULL);

// Lambda Function
auto sortRule = [](int a, int b) {
    return abs(a) < abs(b);
};
sort(v.begin(), v.end(), sortRule);
```
