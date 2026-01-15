# Bipartite Graph Check

> **Trigger**: Two-colorable, divide into two groups, no odd cycles

---

## Key Concept

A graph is bipartite if you can color nodes with 2 colors such that no edge connects same-colored nodes.

**Equivalent to**: Graph has no odd-length cycles.

---

## BFS Coloring

**LeetCode 785** | O(V + E) time

```cpp
bool isBipartite(vector<vector<int>>& graph) {
    int n = graph.size();
    vector<int> color(n, -1);  // -1 = uncolored, 0/1 = colors
    
    for (int start = 0; start < n; start++) {
        if (color[start] != -1) continue;
        
        queue<int> q;
        q.push(start);
        color[start] = 0;
        
        while (!q.empty()) {
            int node = q.front(); q.pop();
            
            for (int neighbor : graph[node]) {
                if (color[neighbor] == -1) {
                    color[neighbor] = 1 - color[node];
                    q.push(neighbor);
                } else if (color[neighbor] == color[node]) {
                    return false;  // Same color = not bipartite
                }
            }
        }
    }
    return true;
}
```

---

## DFS Coloring

```cpp
bool isBipartite(vector<vector<int>>& graph) {
    int n = graph.size();
    vector<int> color(n, -1);
    
    function<bool(int, int)> dfs = [&](int node, int c) {
        color[node] = c;
        
        for (int neighbor : graph[node]) {
            if (color[neighbor] == -1) {
                if (!dfs(neighbor, 1 - c)) return false;
            } else if (color[neighbor] == c) {
                return false;
            }
        }
        return true;
    };
    
    for (int i = 0; i < n; i++) {
        if (color[i] == -1 && !dfs(i, 0)) return false;
    }
    return true;
}
```

---

## Possible Bipartition (Group Enemies)

**LeetCode 886** | O(V + E) time

```cpp
bool possibleBipartition(int n, vector<vector<int>>& dislikes) {
    vector<vector<int>> graph(n + 1);
    for (auto& d : dislikes) {
        graph[d[0]].push_back(d[1]);
        graph[d[1]].push_back(d[0]);
    }
    
    vector<int> color(n + 1, -1);
    
    for (int i = 1; i <= n; i++) {
        if (color[i] != -1) continue;
        
        queue<int> q;
        q.push(i);
        color[i] = 0;
        
        while (!q.empty()) {
            int node = q.front(); q.pop();
            
            for (int enemy : graph[node]) {
                if (color[enemy] == -1) {
                    color[enemy] = 1 - color[node];
                    q.push(enemy);
                } else if (color[enemy] == color[node]) {
                    return false;
                }
            }
        }
    }
    return true;
}
```

---

## Maximum Bipartite Matching (Hungarian Algorithm)

For interview: know the concept, rarely need to implement.

```cpp
// Basic idea: augmenting paths
bool dfs(int u, vector<vector<int>>& adj, vector<int>& match, 
         vector<bool>& visited) {
    for (int v : adj[u]) {
        if (visited[v]) continue;
        visited[v] = true;
        
        if (match[v] == -1 || dfs(match[v], adj, match, visited)) {
            match[v] = u;
            return true;
        }
    }
    return false;
}

int maxMatching(int n, int m, vector<vector<int>>& adj) {
    vector<int> match(m, -1);
    int result = 0;
    
    for (int u = 0; u < n; u++) {
        vector<bool> visited(m, false);
        if (dfs(u, adj, match, visited)) result++;
    }
    return result;
}
```

---

## Practice Problems

| Problem | Difficulty | Key Insight |
|---------|------------|-------------|
| LC 785 - Is Graph Bipartite | Medium | BFS/DFS coloring |
| LC 886 - Possible Bipartition | Medium | Same as bipartite |
| LC 1042 - Flower Planting | Medium | Graph coloring (4 colors) |
| LC 133 - Clone Graph | Medium | Track visited with map |
