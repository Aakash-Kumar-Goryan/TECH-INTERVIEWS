# Union Find (Disjoint Set Union)

> **Trigger**: Connected components, cycle detection in undirected, grouping, "are A and B connected?"

---

## Template

```cpp
class UnionFind {
public:
    vector<int> parent, rank;
    int components;
    
    UnionFind(int n) {
        parent.resize(n);
        rank.resize(n, 0);
        components = n;
        for (int i = 0; i < n; i++) parent[i] = i;
    }
    
    int find(int x) {
        if (parent[x] != x) {
            parent[x] = find(parent[x]); // path compression
        }
        return parent[x];
    }
    
    bool unite(int x, int y) {
        int px = find(x), py = find(y);
        if (px == py) return false; // already connected
        
        // union by rank
        if (rank[px] < rank[py]) swap(px, py);
        parent[py] = px;
        if (rank[px] == rank[py]) rank[px]++;
        
        components--;
        return true;
    }
    
    bool connected(int x, int y) {
        return find(x) == find(y);
    }
};
```

**Time**: O(α(n)) per operation ≈ O(1) amortized

---

## Example 1: Number of Connected Components

**LeetCode 323** | O(V+E) time

```cpp
int countComponents(int n, vector<vector<int>>& edges) {
    UnionFind uf(n);
    
    for (auto& e : edges) {
        uf.unite(e[0], e[1]);
    }
    
    return uf.components;
}
```

---

## Example 2: Redundant Connection (Cycle Detection)

**LeetCode 684** | O(n) time

```cpp
vector<int> findRedundantConnection(vector<vector<int>>& edges) {
    int n = edges.size();
    UnionFind uf(n + 1);
    
    for (auto& e : edges) {
        if (!uf.unite(e[0], e[1])) {
            return e; // this edge creates cycle
        }
    }
    return {};
}
```

---

## Example 3: Accounts Merge

**LeetCode 721** | O(n log n) time

```cpp
vector<vector<string>> accountsMerge(vector<vector<string>>& accounts) {
    int n = accounts.size();
    UnionFind uf(n);
    unordered_map<string, int> emailToId;
    
    // Unite accounts with common emails
    for (int i = 0; i < n; i++) {
        for (int j = 1; j < accounts[i].size(); j++) {
            string email = accounts[i][j];
            if (emailToId.count(email)) {
                uf.unite(i, emailToId[email]);
            } else {
                emailToId[email] = i;
            }
        }
    }
    
    // Group emails by parent
    unordered_map<int, set<string>> groups;
    for (auto& [email, id] : emailToId) {
        groups[uf.find(id)].insert(email);
    }
    
    // Build result
    vector<vector<string>> result;
    for (auto& [id, emails] : groups) {
        vector<string> acc = {accounts[id][0]};
        for (auto& e : emails) acc.push_back(e);
        result.push_back(acc);
    }
    return result;
}
```

---

## When to Use Union Find vs BFS/DFS

| Scenario | Best Choice |
|----------|-------------|
| Dynamic connectivity queries | **Union Find** |
| Static graph, single traversal | **BFS/DFS** |
| Detect cycle in undirected | **Union Find** (simpler) |
| Shortest path | **BFS** |
| Kruskal's MST | **Union Find** |

---

## Practice Problems

| Problem | Difficulty | Key Insight |
|---------|------------|-------------|
| LC 323 - Number of Connected Components | Medium | Count components |
| LC 684 - Redundant Connection | Medium | Edge creating cycle |
| LC 721 - Accounts Merge | Medium | Group by common emails |
| LC 547 - Number of Provinces | Medium | Friend groups |
| LC 1135 - Min Cost to Connect Cities | Medium | Kruskal's MST |
