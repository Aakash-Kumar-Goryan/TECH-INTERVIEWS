# Tree Traversals

> **Trigger**: Visit all nodes, level order processing, serialize/deserialize

---

## Inorder (Left → Root → Right)

```cpp
// Recursive
void inorder(TreeNode* root, vector<int>& result) {
    if (!root) return;
    inorder(root->left, result);
    result.push_back(root->val);
    inorder(root->right, result);
}

// Iterative
vector<int> inorderIterative(TreeNode* root) {
    vector<int> result;
    stack<TreeNode*> st;
    TreeNode* curr = root;
    
    while (curr || !st.empty()) {
        while (curr) {
            st.push(curr);
            curr = curr->left;
        }
        curr = st.top(); st.pop();
        result.push_back(curr->val);
        curr = curr->right;
    }
    return result;
}
```

**Use**: BST gives sorted order, expression trees

---

## Preorder (Root → Left → Right)

```cpp
// Iterative
vector<int> preorder(TreeNode* root) {
    if (!root) return {};
    vector<int> result;
    stack<TreeNode*> st;
    st.push(root);
    
    while (!st.empty()) {
        TreeNode* node = st.top(); st.pop();
        result.push_back(node->val);
        if (node->right) st.push(node->right);
        if (node->left) st.push(node->left);
    }
    return result;
}
```

**Use**: Serialize tree, copy tree

---

## Postorder (Left → Right → Root)

```cpp
// Iterative (reverse of modified preorder)
vector<int> postorder(TreeNode* root) {
    if (!root) return {};
    vector<int> result;
    stack<TreeNode*> st;
    st.push(root);
    
    while (!st.empty()) {
        TreeNode* node = st.top(); st.pop();
        result.push_back(node->val);
        if (node->left) st.push(node->left);
        if (node->right) st.push(node->right);
    }
    reverse(result.begin(), result.end());
    return result;
}
```

**Use**: Delete tree, evaluate expression tree

---

## Level Order (BFS)

```cpp
vector<vector<int>> levelOrder(TreeNode* root) {
    if (!root) return {};
    vector<vector<int>> result;
    queue<TreeNode*> q;
    q.push(root);
    
    while (!q.empty()) {
        int size = q.size();
        vector<int> level;
        
        for (int i = 0; i < size; i++) {
            TreeNode* node = q.front(); q.pop();
            level.push_back(node->val);
            if (node->left) q.push(node->left);
            if (node->right) q.push(node->right);
        }
        result.push_back(level);
    }
    return result;
}
```

**Use**: Level-by-level processing, shortest path in tree

---

## Example: Maximum Depth of Binary Tree

**LeetCode 104** | O(n) time

```cpp
int maxDepth(TreeNode* root) {
    if (!root) return 0;
    return 1 + max(maxDepth(root->left), maxDepth(root->right));
}
```

---

## Example: Same Tree

**LeetCode 100** | O(n) time

```cpp
bool isSameTree(TreeNode* p, TreeNode* q) {
    if (!p && !q) return true;
    if (!p || !q) return false;
    return p->val == q->val 
        && isSameTree(p->left, q->left) 
        && isSameTree(p->right, q->right);
}
```

---

## Practice Problems

| Problem | Difficulty | Key Insight |
|---------|------------|-------------|
| LC 94 - Inorder Traversal | Easy | BST → sorted |
| LC 102 - Level Order Traversal | Medium | BFS with size tracking |
| LC 104 - Maximum Depth | Easy | Simple recursion |
| LC 226 - Invert Binary Tree | Easy | Swap children recursively |
| LC 297 - Serialize and Deserialize | Hard | Preorder + queue |
