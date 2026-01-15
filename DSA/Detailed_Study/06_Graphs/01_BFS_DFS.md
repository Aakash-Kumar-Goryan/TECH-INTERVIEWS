# BFS & DFS on Graphs

> **Trigger**: Traversal, shortest path (unweighted), connected components, level-order processing

---

## Graph Representation

```cpp
// Adjacency List (most common)
vector<vector<int>> graph(n);  // n nodes
graph[u].push_back(v);         // edge u → v

// Adjacency List with weights
vector<vector<pair<int,int>>> graph(n);
graph[u].push_back({v, weight});

// Edge List (for Union Find problems)
vector<tuple<int,int,int>> edges; // u, v, weight
```

---

## BFS Template

```cpp
vector<int> bfs(vector<vector<int>>& graph, int start) {
    int n = graph.size();
    vector<int> dist(n, -1);
    queue<int> q;
    
    dist[start] = 0;
    q.push(start);
    
    while (!q.empty()) {
        int node = q.front(); q.pop();
        
        for (int neighbor : graph[node]) {
            if (dist[neighbor] == -1) {
                dist[neighbor] = dist[node] + 1;
                q.push(neighbor);
            }
        }
    }
    return dist; // dist[i] = shortest distance from start
}
```

**Use**: Shortest path (unweighted), level-by-level, nearest X

---

## DFS Template (Recursive)

```cpp
void dfs(vector<vector<int>>& graph, int node, vector<bool>& visited) {
    visited[node] = true;
    
    for (int neighbor : graph[node]) {
        if (!visited[neighbor]) {
            dfs(graph, neighbor, visited);
        }
    }
}
```

## DFS Template (Iterative)

```cpp
void dfsIterative(vector<vector<int>>& graph, int start) {
    int n = graph.size();
    vector<bool> visited(n, false);
    stack<int> st;
    
    st.push(start);
    while (!st.empty()) {
        int node = st.top(); st.pop();
        
        if (visited[node]) continue;
        visited[node] = true;
        
        for (int neighbor : graph[node]) {
            if (!visited[neighbor]) {
                st.push(neighbor);
            }
        }
    }
}
```

---

## Example 1: Number of Islands

**LeetCode 200** | O(m×n) time

```cpp
int numIslands(vector<vector<char>>& grid) {
    int m = grid.size(), n = grid[0].size();
    int count = 0;
    
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            if (grid[i][j] == '1') {
                count++;
                dfs(grid, i, j);
            }
        }
    }
    return count;
}

void dfs(vector<vector<char>>& grid, int i, int j) {
    if (i < 0 || i >= grid.size() || j < 0 || j >= grid[0].size()) return;
    if (grid[i][j] != '1') return;
    
    grid[i][j] = '0'; // mark visited
    dfs(grid, i+1, j);
    dfs(grid, i-1, j);
    dfs(grid, i, j+1);
    dfs(grid, i, j-1);
}
```

---

## Example 2: Clone Graph

**LeetCode 133** | O(V+E) time

```cpp
Node* cloneGraph(Node* node) {
    if (!node) return nullptr;
    
    unordered_map<Node*, Node*> cloned;
    return dfs(node, cloned);
}

Node* dfs(Node* node, unordered_map<Node*, Node*>& cloned) {
    if (cloned.count(node)) return cloned[node];
    
    Node* copy = new Node(node->val);
    cloned[node] = copy;
    
    for (Node* neighbor : node->neighbors) {
        copy->neighbors.push_back(dfs(neighbor, cloned));
    }
    return copy;
}
```

---

## 4/8 Direction Movement

```cpp
// 4 directions
int dx[] = {0, 0, 1, -1};
int dy[] = {1, -1, 0, 0};

// 8 directions
int dx[] = {0, 0, 1, -1, 1, 1, -1, -1};
int dy[] = {1, -1, 0, 0, 1, -1, 1, -1};

for (int d = 0; d < 4; d++) {
    int nx = x + dx[d], ny = y + dy[d];
    if (nx >= 0 && nx < m && ny >= 0 && ny < n) {
        // valid neighbor
    }
}
```

---

## Practice Problems

| Problem | Difficulty | Key Insight |
|---------|------------|-------------|
| LC 200 - Number of Islands | Medium | DFS to mark connected |
| LC 994 - Rotting Oranges | Medium | Multi-source BFS |
| LC 133 - Clone Graph | Medium | HashMap + DFS |
| LC 127 - Word Ladder | Hard | BFS level = transformations |
| LC 695 - Max Area of Island | Medium | DFS + count |
