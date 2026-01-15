# Cycle Detection in Graphs

> **Trigger**: Check for cycles in directed/undirected graphs, detect back edges

---

## Cycle in Undirected Graph (Union Find)

**O(E × α(V))** time

```cpp
bool hasCycleUF(int n, vector<vector<int>>& edges) {
    vector<int> parent(n), rank(n, 0);
    iota(parent.begin(), parent.end(), 0);
    
    function<int(int)> find = [&](int x) {
        if (parent[x] != x) parent[x] = find(parent[x]);
        return parent[x];
    };
    
    for (auto& e : edges) {
        int px = find(e[0]), py = find(e[1]);
        if (px == py) return true;  // Cycle!
        if (rank[px] < rank[py]) swap(px, py);
        parent[py] = px;
        if (rank[px] == rank[py]) rank[px]++;
    }
    return false;
}
```

---

## Cycle in Undirected Graph (DFS)

**O(V + E)** time

```cpp
bool hasCycleDFS(vector<vector<int>>& graph) {
    int n = graph.size();
    vector<bool> visited(n, false);
    
    function<bool(int, int)> dfs = [&](int node, int parent) {
        visited[node] = true;
        
        for (int neighbor : graph[node]) {
            if (!visited[neighbor]) {
                if (dfs(neighbor, node)) return true;
            } else if (neighbor != parent) {
                return true;  // Back edge = cycle
            }
        }
        return false;
    };
    
    for (int i = 0; i < n; i++) {
        if (!visited[i] && dfs(i, -1)) return true;
    }
    return false;
}
```

---

## Cycle in Directed Graph (3-Color DFS)

**O(V + E)** time

```cpp
bool hasCycleDirected(vector<vector<int>>& graph) {
    int n = graph.size();
    vector<int> color(n, 0);  // 0=white, 1=gray, 2=black
    
    function<bool(int)> dfs = [&](int node) {
        color[node] = 1;  // Visiting (gray)
        
        for (int neighbor : graph[node]) {
            if (color[neighbor] == 1) return true;  // Back edge
            if (color[neighbor] == 0 && dfs(neighbor)) return true;
        }
        
        color[node] = 2;  // Visited (black)
        return false;
    };
    
    for (int i = 0; i < n; i++) {
        if (color[i] == 0 && dfs(i)) return true;
    }
    return false;
}
```

---

## Find the Duplicate Number (Linked List Cycle)

**LeetCode 287** | O(n) time, O(1) space

```cpp
int findDuplicate(vector<int>& nums) {
    // Treat array as linked list: i → nums[i]
    int slow = nums[0], fast = nums[0];
    
    // Find meeting point
    do {
        slow = nums[slow];
        fast = nums[nums[fast]];
    } while (slow != fast);
    
    // Find cycle start
    slow = nums[0];
    while (slow != fast) {
        slow = nums[slow];
        fast = nums[fast];
    }
    return slow;
}
```

---

## Practice Problems

| Problem | Difficulty | Key Insight |
|---------|------------|-------------|
| LC 684 - Redundant Connection | Medium | Union Find, first cycle edge |
| LC 207 - Course Schedule | Medium | Directed cycle = impossible |
| LC 287 - Find Duplicate | Medium | Array as linked list |
| LC 802 - Find Eventual Safe States | Medium | Nodes not in cycle |
