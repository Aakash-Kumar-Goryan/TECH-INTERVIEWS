# Lowest Common Ancestor

> **Trigger**: Find common ancestor of two nodes, path intersection

---

## LCA in Binary Tree

**LeetCode 236** | O(n) time

```cpp
TreeNode* lowestCommonAncestor(TreeNode* root, TreeNode* p, TreeNode* q) {
    if (!root || root == p || root == q) return root;
    
    TreeNode* left = lowestCommonAncestor(root->left, p, q);
    TreeNode* right = lowestCommonAncestor(root->right, p, q);
    
    if (left && right) return root;  // p and q on different sides
    return left ? left : right;      // both on same side
}
```

**Intuition**: If p and q are on different sides of root, root is LCA. Otherwise, LCA is on the side that has both.

---

## LCA in BST

**LeetCode 235** | O(h) time

```cpp
TreeNode* lowestCommonAncestor(TreeNode* root, TreeNode* p, TreeNode* q) {
    while (root) {
        if (p->val < root->val && q->val < root->val)
            root = root->left;
        else if (p->val > root->val && q->val > root->val)
            root = root->right;
        else
            return root;  // Split point = LCA
    }
    return nullptr;
}
```

---

## LCA with Parent Pointers

```cpp
TreeNode* lowestCommonAncestor(TreeNode* p, TreeNode* q) {
    unordered_set<TreeNode*> ancestors;
    
    // Store all ancestors of p
    while (p) {
        ancestors.insert(p);
        p = p->parent;
    }
    
    // Find first common ancestor of q
    while (q) {
        if (ancestors.count(q)) return q;
        q = q->parent;
    }
    return nullptr;
}
```

---

## Binary Lifting (For Multiple Queries)

**Preprocessing:** O(n log n), **Query:** O(log n)

```cpp
class LCABinaryLifting {
    int LOG;
    vector<int> depth;
    vector<vector<int>> up; // up[node][i] = 2^i-th ancestor
    
public:
    LCABinaryLifting(int n, vector<vector<int>>& adj) {
        LOG = log2(n) + 1;
        depth.resize(n);
        up.assign(n, vector<int>(LOG, -1));
        
        dfs(0, -1, adj);
    }
    
    void dfs(int node, int parent, vector<vector<int>>& adj) {
        up[node][0] = parent;
        for (int i = 1; i < LOG; i++) {
            if (up[node][i-1] != -1)
                up[node][i] = up[up[node][i-1]][i-1];
        }
        
        for (int child : adj[node]) {
            if (child != parent) {
                depth[child] = depth[node] + 1;
                dfs(child, node, adj);
            }
        }
    }
    
    int lca(int a, int b) {
        if (depth[a] < depth[b]) swap(a, b);
        
        // Bring a to same level as b
        int diff = depth[a] - depth[b];
        for (int i = 0; i < LOG; i++) {
            if ((diff >> i) & 1) a = up[a][i];
        }
        
        if (a == b) return a;
        
        // Binary lift both
        for (int i = LOG - 1; i >= 0; i--) {
            if (up[a][i] != up[b][i]) {
                a = up[a][i];
                b = up[b][i];
            }
        }
        return up[a][0];
    }
};
```

---

## Practice Problems

| Problem | Difficulty | Key Insight |
|---------|------------|-------------|
| LC 236 - LCA of Binary Tree | Medium | Recursive null propagation |
| LC 235 - LCA of BST | Medium | Use BST property |
| LC 1644 - LCA II (may not exist) | Medium | Track if found |
| LC 1650 - LCA III (with parent) | Medium | Two pointer / set |
| LC 865 - Smallest Subtree with Deepest Nodes | Medium | LCA of deepest leaves |
