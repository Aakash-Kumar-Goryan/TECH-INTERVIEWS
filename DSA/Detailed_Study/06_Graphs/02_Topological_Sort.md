# Topological Sort

> **Trigger**: Dependencies, course scheduling, build order, DAG processing order

---

## Key Concepts

- **Only works on DAG** (Directed Acyclic Graph)
- Linear ordering where for edge u→v, u comes before v
- If cycle exists → no valid topological order

---

## Kahn's Algorithm (BFS)

```cpp
vector<int> topologicalSort(int n, vector<vector<int>>& edges) {
    vector<int> indegree(n, 0);
    vector<vector<int>> graph(n);
    
    for (auto& e : edges) {
        graph[e[0]].push_back(e[1]);
        indegree[e[1]]++;
    }
    
    queue<int> q;
    for (int i = 0; i < n; i++) {
        if (indegree[i] == 0) q.push(i);
    }
    
    vector<int> order;
    while (!q.empty()) {
        int node = q.front(); q.pop();
        order.push_back(node);
        
        for (int neighbor : graph[node]) {
            indegree[neighbor]--;
            if (indegree[neighbor] == 0) {
                q.push(neighbor);
            }
        }
    }
    
    // If order.size() != n, cycle exists
    return order.size() == n ? order : vector<int>{};
}
```

---

## DFS-based Topological Sort

```cpp
vector<int> topologicalSortDFS(int n, vector<vector<int>>& graph) {
    vector<int> state(n, 0); // 0=unvisited, 1=visiting, 2=visited
    vector<int> order;
    
    function<bool(int)> dfs = [&](int node) -> bool {
        state[node] = 1; // visiting
        
        for (int neighbor : graph[node]) {
            if (state[neighbor] == 1) return false; // cycle
            if (state[neighbor] == 0) {
                if (!dfs(neighbor)) return false;
            }
        }
        
        state[node] = 2; // visited
        order.push_back(node);
        return true;
    };
    
    for (int i = 0; i < n; i++) {
        if (state[i] == 0) {
            if (!dfs(i)) return {}; // cycle detected
        }
    }
    
    reverse(order.begin(), order.end());
    return order;
}
```

---

## Example 1: Course Schedule

**LeetCode 207** | O(V+E) time

```cpp
bool canFinish(int numCourses, vector<vector<int>>& prerequisites) {
    vector<int> indegree(numCourses, 0);
    vector<vector<int>> graph(numCourses);
    
    for (auto& p : prerequisites) {
        graph[p[1]].push_back(p[0]);
        indegree[p[0]]++;
    }
    
    queue<int> q;
    for (int i = 0; i < numCourses; i++) {
        if (indegree[i] == 0) q.push(i);
    }
    
    int completed = 0;
    while (!q.empty()) {
        int course = q.front(); q.pop();
        completed++;
        
        for (int next : graph[course]) {
            indegree[next]--;
            if (indegree[next] == 0) q.push(next);
        }
    }
    
    return completed == numCourses;
}
```

---

## Example 2: Course Schedule II (Return Order)

**LeetCode 210** | O(V+E) time

```cpp
vector<int> findOrder(int numCourses, vector<vector<int>>& prerequisites) {
    vector<int> indegree(numCourses, 0);
    vector<vector<int>> graph(numCourses);
    
    for (auto& p : prerequisites) {
        graph[p[1]].push_back(p[0]);
        indegree[p[0]]++;
    }
    
    queue<int> q;
    for (int i = 0; i < numCourses; i++) {
        if (indegree[i] == 0) q.push(i);
    }
    
    vector<int> order;
    while (!q.empty()) {
        int course = q.front(); q.pop();
        order.push_back(course);
        
        for (int next : graph[course]) {
            if (--indegree[next] == 0) q.push(next);
        }
    }
    
    return order.size() == numCourses ? order : vector<int>{};
}
```

---

## When to Use Which

| Algorithm | Use Case |
|-----------|----------|
| **Kahn's (BFS)** | Need cycle detection + order, level-by-level |
| **DFS** | Simpler, when just need any valid order |

---

## Practice Problems

| Problem | Difficulty | Key Insight |
|---------|------------|-------------|
| LC 207 - Course Schedule | Medium | Cycle detection |
| LC 210 - Course Schedule II | Medium | Return valid order |
| LC 269 - Alien Dictionary | Hard | Build graph from words |
| LC 310 - Minimum Height Trees | Medium | Topo sort from leaves |
