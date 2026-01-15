# Shortest Path Algorithms

> **Trigger**: Minimum distance/cost in weighted graph, single source shortest path

---

## Dijkstra's Algorithm

**Use when:** Weighted graph, **non-negative weights**, single source

**Time:** O(E log V) with heap

```cpp
vector<int> dijkstra(vector<vector<pair<int,int>>>& graph, int src) {
    int n = graph.size();
    vector<int> dist(n, INT_MAX);
    priority_queue<pair<int,int>, vector<pair<int,int>>, greater<>> pq;
    
    dist[src] = 0;
    pq.push({0, src}); // {distance, node}
    
    while (!pq.empty()) {
        auto [d, u] = pq.top(); pq.pop();
        
        if (d > dist[u]) continue; // Skip outdated entries
        
        for (auto& [v, w] : graph[u]) {
            if (dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                pq.push({dist[v], v});
            }
        }
    }
    return dist;
}
```

---

## Example: Network Delay Time

**LeetCode 743** | O(E log V) time

```cpp
int networkDelayTime(vector<vector<int>>& times, int n, int k) {
    vector<vector<pair<int,int>>> graph(n + 1);
    for (auto& t : times) {
        graph[t[0]].push_back({t[1], t[2]});
    }
    
    vector<int> dist(n + 1, INT_MAX);
    priority_queue<pair<int,int>, vector<pair<int,int>>, greater<>> pq;
    
    dist[k] = 0;
    pq.push({0, k});
    
    while (!pq.empty()) {
        auto [d, u] = pq.top(); pq.pop();
        if (d > dist[u]) continue;
        
        for (auto& [v, w] : graph[u]) {
            if (dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                pq.push({dist[v], v});
            }
        }
    }
    
    int maxDist = *max_element(dist.begin() + 1, dist.end());
    return maxDist == INT_MAX ? -1 : maxDist;
}
```

---

## Bellman-Ford Algorithm

**Use when:** Graph may have **negative weights** (but no negative cycles)

**Time:** O(V × E)

```cpp
vector<int> bellmanFord(int n, vector<vector<int>>& edges, int src) {
    vector<int> dist(n, INT_MAX);
    dist[src] = 0;
    
    // Relax all edges V-1 times
    for (int i = 0; i < n - 1; i++) {
        for (auto& e : edges) {
            int u = e[0], v = e[1], w = e[2];
            if (dist[u] != INT_MAX && dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
            }
        }
    }
    
    // Check for negative cycle
    for (auto& e : edges) {
        int u = e[0], v = e[1], w = e[2];
        if (dist[u] != INT_MAX && dist[u] + w < dist[v]) {
            return {}; // Negative cycle exists
        }
    }
    return dist;
}
```

---

## 0-1 BFS (Deque)

**Use when:** Edge weights are only **0 or 1**

**Time:** O(V + E)

```cpp
vector<int> bfs01(vector<vector<pair<int,int>>>& graph, int src) {
    int n = graph.size();
    vector<int> dist(n, INT_MAX);
    deque<int> dq;
    
    dist[src] = 0;
    dq.push_front(src);
    
    while (!dq.empty()) {
        int u = dq.front(); dq.pop_front();
        
        for (auto& [v, w] : graph[u]) {
            if (dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                if (w == 0) dq.push_front(v);
                else dq.push_back(v);
            }
        }
    }
    return dist;
}
```

---

## Algorithm Comparison

| Algorithm | Time | Negative Weights | Use Case |
|-----------|------|------------------|----------|
| **BFS** | O(V+E) | No (unweighted) | Unweighted graphs |
| **Dijkstra** | O(E log V) | No | Positive weights |
| **Bellman-Ford** | O(VE) | Yes | Negative weights |
| **0-1 BFS** | O(V+E) | No (0 or 1 only) | 0/1 weights |
| **Floyd-Warshall** | O(V³) | Yes | All pairs shortest |

---

## Practice Problems

| Problem | Difficulty | Key Insight |
|---------|------------|-------------|
| LC 743 - Network Delay Time | Medium | Basic Dijkstra |
| LC 787 - Cheapest Flights Within K Stops | Medium | Modified Bellman-Ford |
| LC 1091 - Shortest Path in Binary Matrix | Medium | BFS (0-1 weights) |
| LC 1514 - Path with Maximum Probability | Medium | Max Dijkstra (use max-heap) |
| LC 778 - Swim in Rising Water | Hard | Binary search + BFS or Dijkstra |
