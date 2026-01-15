# Segment Tree

> **Trigger**: Range queries (sum/min/max) with point or range updates

---

## When to Use

| Operation | Array | Prefix Sum | Segment Tree |
|-----------|-------|------------|--------------|
| Build | O(n) | O(n) | O(n) |
| Point Update | O(1) | O(n) | O(log n) |
| Range Query | O(n) | O(1) | O(log n) |
| Range Update | O(n) | O(n) | O(log n)* |

*With lazy propagation

---

## Basic Segment Tree (Point Update, Range Query)

```cpp
class SegmentTree {
    vector<int> tree;
    int n;
    
    void build(vector<int>& arr, int node, int start, int end) {
        if (start == end) {
            tree[node] = arr[start];
            return;
        }
        int mid = (start + end) / 2;
        build(arr, 2*node, start, mid);
        build(arr, 2*node+1, mid+1, end);
        tree[node] = tree[2*node] + tree[2*node+1]; // Sum
    }
    
    void update(int node, int start, int end, int idx, int val) {
        if (start == end) {
            tree[node] = val;
            return;
        }
        int mid = (start + end) / 2;
        if (idx <= mid)
            update(2*node, start, mid, idx, val);
        else
            update(2*node+1, mid+1, end, idx, val);
        tree[node] = tree[2*node] + tree[2*node+1];
    }
    
    int query(int node, int start, int end, int l, int r) {
        if (r < start || end < l) return 0; // Out of range
        if (l <= start && end <= r) return tree[node]; // Fully in range
        
        int mid = (start + end) / 2;
        return query(2*node, start, mid, l, r) + 
               query(2*node+1, mid+1, end, l, r);
    }
    
public:
    SegmentTree(vector<int>& arr) {
        n = arr.size();
        tree.resize(4 * n);
        build(arr, 1, 0, n-1);
    }
    
    void update(int idx, int val) { update(1, 0, n-1, idx, val); }
    int query(int l, int r) { return query(1, 0, n-1, l, r); }
};
```

---

## Range Sum Query - Mutable

**LeetCode 307** | O(log n) per operation

```cpp
class NumArray {
    vector<int> tree;
    int n;
    
public:
    NumArray(vector<int>& nums) {
        n = nums.size();
        tree.resize(2 * n);
        
        // Build: leaves at [n, 2n-1]
        for (int i = 0; i < n; i++)
            tree[n + i] = nums[i];
        for (int i = n - 1; i > 0; i--)
            tree[i] = tree[2*i] + tree[2*i+1];
    }
    
    void update(int idx, int val) {
        idx += n;
        tree[idx] = val;
        while (idx > 1) {
            idx /= 2;
            tree[idx] = tree[2*idx] + tree[2*idx+1];
        }
    }
    
    int sumRange(int left, int right) {
        left += n; right += n;
        int sum = 0;
        while (left <= right) {
            if (left % 2 == 1) sum += tree[left++];
            if (right % 2 == 0) sum += tree[right--];
            left /= 2; right /= 2;
        }
        return sum;
    }
};
```

---

## Binary Indexed Tree (Fenwick Tree)

Simpler alternative for prefix sums with point updates.

```cpp
class BIT {
    vector<int> tree;
    int n;
    
public:
    BIT(int n) : n(n), tree(n + 1, 0) {}
    
    // Add delta to index i (1-indexed)
    void update(int i, int delta) {
        for (; i <= n; i += i & (-i))
            tree[i] += delta;
    }
    
    // Sum of [1, i]
    int query(int i) {
        int sum = 0;
        for (; i > 0; i -= i & (-i))
            sum += tree[i];
        return sum;
    }
    
    // Sum of [l, r]
    int rangeQuery(int l, int r) {
        return query(r) - query(l - 1);
    }
};
```

---

## Count of Smaller Numbers After Self

**LeetCode 315** | O(n log n) with BIT

```cpp
vector<int> countSmaller(vector<int>& nums) {
    // Coordinate compression
    set<int> sorted(nums.begin(), nums.end());
    unordered_map<int, int> rank;
    int r = 1;
    for (int x : sorted) rank[x] = r++;
    
    int n = nums.size();
    vector<int> result(n);
    BIT bit(rank.size());
    
    for (int i = n - 1; i >= 0; i--) {
        result[i] = bit.query(rank[nums[i]] - 1);
        bit.update(rank[nums[i]], 1);
    }
    return result;
}
```

---

## Practice Problems

| Problem | Difficulty | Key Insight |
|---------|------------|-------------|
| LC 307 - Range Sum Query Mutable | Medium | Basic segment tree |
| LC 315 - Count Smaller After Self | Hard | BIT + coordinate compress |
| LC 493 - Reverse Pairs | Hard | Merge sort or BIT |
| LC 327 - Count Range Sum | Hard | Merge sort or segment tree |
