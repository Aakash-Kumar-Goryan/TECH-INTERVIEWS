# Path Sum & Tree DP

> **Trigger**: Sum along paths, max path through node, tree optimization

---

## Path Sum (Root to Leaf)

**LeetCode 112** | O(n) time

```cpp
bool hasPathSum(TreeNode* root, int targetSum) {
    if (!root) return false;
    
    if (!root->left && !root->right)
        return root->val == targetSum;
    
    return hasPathSum(root->left, targetSum - root->val) ||
           hasPathSum(root->right, targetSum - root->val);
}
```

---

## Path Sum II (Return All Paths)

**LeetCode 113** | O(n) time

```cpp
vector<vector<int>> pathSum(TreeNode* root, int targetSum) {
    vector<vector<int>> result;
    vector<int> path;
    dfs(root, targetSum, path, result);
    return result;
}

void dfs(TreeNode* node, int sum, vector<int>& path, 
         vector<vector<int>>& result) {
    if (!node) return;
    
    path.push_back(node->val);
    
    if (!node->left && !node->right && sum == node->val) {
        result.push_back(path);
    }
    
    dfs(node->left, sum - node->val, path, result);
    dfs(node->right, sum - node->val, path, result);
    
    path.pop_back();
}
```

---

## Binary Tree Maximum Path Sum

**LeetCode 124** | O(n) time

```cpp
class Solution {
    int maxSum = INT_MIN;
    
    int maxGain(TreeNode* node) {
        if (!node) return 0;
        
        int leftGain = max(maxGain(node->left), 0);
        int rightGain = max(maxGain(node->right), 0);
        
        // Path through this node
        int pathSum = node->val + leftGain + rightGain;
        maxSum = max(maxSum, pathSum);
        
        // Return max single path (can only go one direction)
        return node->val + max(leftGain, rightGain);
    }
    
public:
    int maxPathSum(TreeNode* root) {
        maxGain(root);
        return maxSum;
    }
};
```

---

## Diameter of Binary Tree

**LeetCode 543** | O(n) time

```cpp
class Solution {
    int diameter = 0;
    
    int depth(TreeNode* node) {
        if (!node) return 0;
        
        int left = depth(node->left);
        int right = depth(node->right);
        
        diameter = max(diameter, left + right);
        return 1 + max(left, right);
    }
    
public:
    int diameterOfBinaryTree(TreeNode* root) {
        depth(root);
        return diameter;
    }
};
```

---

## House Robber III (Tree DP)

**LeetCode 337** | O(n) time

```cpp
int rob(TreeNode* root) {
    auto [robbed, notRobbed] = dfs(root);
    return max(robbed, notRobbed);
}

pair<int, int> dfs(TreeNode* node) {
    if (!node) return {0, 0};
    
    auto [leftRob, leftNotRob] = dfs(node->left);
    auto [rightRob, rightNotRob] = dfs(node->right);
    
    // If we rob this node, can't rob children
    int robThis = node->val + leftNotRob + rightNotRob;
    
    // If we don't rob this node, take max of each child
    int notRobThis = max(leftRob, leftNotRob) + max(rightRob, rightNotRob);
    
    return {robThis, notRobThis};
}
```

---

## Construct Binary Tree from Preorder and Inorder

**LeetCode 105** | O(n) time

```cpp
TreeNode* buildTree(vector<int>& preorder, vector<int>& inorder) {
    unordered_map<int, int> inorderIdx;
    for (int i = 0; i < inorder.size(); i++)
        inorderIdx[inorder[i]] = i;
    
    int preIdx = 0;
    return build(preorder, 0, inorder.size() - 1, preIdx, inorderIdx);
}

TreeNode* build(vector<int>& preorder, int inStart, int inEnd, 
                int& preIdx, unordered_map<int, int>& inorderIdx) {
    if (inStart > inEnd) return nullptr;
    
    int rootVal = preorder[preIdx++];
    TreeNode* root = new TreeNode(rootVal);
    
    int inMid = inorderIdx[rootVal];
    root->left = build(preorder, inStart, inMid - 1, preIdx, inorderIdx);
    root->right = build(preorder, inMid + 1, inEnd, preIdx, inorderIdx);
    
    return root;
}
```

---

## Practice Problems

| Problem | Difficulty | Key Insight |
|---------|------------|-------------|
| LC 112 - Path Sum | Easy | Subtract going down |
| LC 124 - Max Path Sum | Hard | Global max, return single path |
| LC 543 - Diameter | Easy | Left depth + right depth |
| LC 337 - House Robber III | Medium | Tree DP: rob/not rob state |
| LC 105 - Construct from Traversals | Medium | HashMap + recursion |
