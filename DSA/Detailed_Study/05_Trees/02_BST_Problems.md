# BST Problems

> **Trigger**: Sorted property, search/insert O(log n), inorder = sorted, validate range

---

## Key Properties

- Left subtree values < node value
- Right subtree values > node value  
- Inorder traversal gives sorted sequence
- Search, insert, delete: O(h) → O(log n) balanced, O(n) worst

---

## Search in BST

```cpp
TreeNode* searchBST(TreeNode* root, int val) {
    if (!root || root->val == val) return root;
    return val < root->val 
        ? searchBST(root->left, val) 
        : searchBST(root->right, val);
}
```

---

## Insert into BST

```cpp
TreeNode* insertIntoBST(TreeNode* root, int val) {
    if (!root) return new TreeNode(val);
    
    if (val < root->val) root->left = insertIntoBST(root->left, val);
    else root->right = insertIntoBST(root->right, val);
    
    return root;
}
```

---

## Delete from BST

```cpp
TreeNode* deleteNode(TreeNode* root, int key) {
    if (!root) return nullptr;
    
    if (key < root->val) {
        root->left = deleteNode(root->left, key);
    } else if (key > root->val) {
        root->right = deleteNode(root->right, key);
    } else {
        // Node to delete found
        if (!root->left) return root->right;
        if (!root->right) return root->left;
        
        // Two children: replace with inorder successor
        TreeNode* minNode = root->right;
        while (minNode->left) minNode = minNode->left;
        root->val = minNode->val;
        root->right = deleteNode(root->right, minNode->val);
    }
    return root;
}
```

---

## Validate BST

**LeetCode 98** | O(n) time

```cpp
bool isValidBST(TreeNode* root) {
    return validate(root, LONG_MIN, LONG_MAX);
}

bool validate(TreeNode* node, long minVal, long maxVal) {
    if (!node) return true;
    if (node->val <= minVal || node->val >= maxVal) return false;
    
    return validate(node->left, minVal, node->val) 
        && validate(node->right, node->val, maxVal);
}
```

---

## Kth Smallest Element in BST

**LeetCode 230** | O(h + k) time

```cpp
int kthSmallest(TreeNode* root, int k) {
    stack<TreeNode*> st;
    TreeNode* curr = root;
    
    while (curr || !st.empty()) {
        while (curr) {
            st.push(curr);
            curr = curr->left;
        }
        curr = st.top(); st.pop();
        k--;
        if (k == 0) return curr->val;
        curr = curr->right;
    }
    return -1;
}
```

---

## Lowest Common Ancestor in BST

**LeetCode 235** | O(h) time

```cpp
TreeNode* lowestCommonAncestor(TreeNode* root, TreeNode* p, TreeNode* q) {
    while (root) {
        if (p->val < root->val && q->val < root->val)
            root = root->left;
        else if (p->val > root->val && q->val > root->val)
            root = root->right;
        else
            return root; // split point = LCA
    }
    return nullptr;
}
```

---

## Practice Problems

| Problem | Difficulty | Key Insight |
|---------|------------|-------------|
| LC 700 - Search in BST | Easy | Divide by comparison |
| LC 98 - Validate BST | Medium | Pass valid range down |
| LC 230 - Kth Smallest | Medium | Inorder + counter |
| LC 235 - LCA of BST | Medium | Use BST property |
| LC 108 - Sorted Array to BST | Easy | Mid = root, recurse |
