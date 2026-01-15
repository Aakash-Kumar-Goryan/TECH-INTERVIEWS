# Serialize and Deserialize Trees

> **Trigger**: Save/load tree state, network transmission, string representation

---

## Binary Tree (Preorder + Null Markers)

**LeetCode 297** | O(n) time

```cpp
class Codec {
public:
    // Serialize: preorder with "null" markers
    string serialize(TreeNode* root) {
        if (!root) return "null";
        return to_string(root->val) + "," + 
               serialize(root->left) + "," + 
               serialize(root->right);
    }
    
    // Deserialize: rebuild from preorder
    TreeNode* deserialize(string data) {
        queue<string> nodes;
        stringstream ss(data);
        string token;
        while (getline(ss, token, ',')) {
            nodes.push(token);
        }
        return buildTree(nodes);
    }
    
private:
    TreeNode* buildTree(queue<string>& nodes) {
        string val = nodes.front(); nodes.pop();
        if (val == "null") return nullptr;
        
        TreeNode* node = new TreeNode(stoi(val));
        node->left = buildTree(nodes);
        node->right = buildTree(nodes);
        return node;
    }
};
```

---

## BST (More Efficient - No Null Markers)

**LeetCode 449** | O(n) time

```cpp
class Codec {
public:
    string serialize(TreeNode* root) {
        string result;
        serializeHelper(root, result);
        return result;
    }
    
    TreeNode* deserialize(string data) {
        queue<int> nodes;
        stringstream ss(data);
        string token;
        while (getline(ss, token, ',')) {
            if (!token.empty()) nodes.push(stoi(token));
        }
        return buildBST(nodes, INT_MIN, INT_MAX);
    }
    
private:
    void serializeHelper(TreeNode* node, string& result) {
        if (!node) return;
        result += to_string(node->val) + ",";
        serializeHelper(node->left, result);
        serializeHelper(node->right, result);
    }
    
    TreeNode* buildBST(queue<int>& nodes, int minVal, int maxVal) {
        if (nodes.empty()) return nullptr;
        
        int val = nodes.front();
        if (val < minVal || val > maxVal) return nullptr;
        
        nodes.pop();
        TreeNode* node = new TreeNode(val);
        node->left = buildBST(nodes, minVal, val);
        node->right = buildBST(nodes, val, maxVal);
        return node;
    }
};
```

---

## Level Order (BFS) Serialization

```cpp
class Codec {
public:
    string serialize(TreeNode* root) {
        if (!root) return "";
        
        string result;
        queue<TreeNode*> q;
        q.push(root);
        
        while (!q.empty()) {
            TreeNode* node = q.front(); q.pop();
            if (node) {
                result += to_string(node->val) + ",";
                q.push(node->left);
                q.push(node->right);
            } else {
                result += "null,";
            }
        }
        return result;
    }
    
    TreeNode* deserialize(string data) {
        if (data.empty()) return nullptr;
        
        vector<string> nodes;
        stringstream ss(data);
        string token;
        while (getline(ss, token, ',')) {
            if (!token.empty()) nodes.push_back(token);
        }
        
        TreeNode* root = new TreeNode(stoi(nodes[0]));
        queue<TreeNode*> q;
        q.push(root);
        int i = 1;
        
        while (!q.empty() && i < nodes.size()) {
            TreeNode* node = q.front(); q.pop();
            
            if (nodes[i] != "null") {
                node->left = new TreeNode(stoi(nodes[i]));
                q.push(node->left);
            }
            i++;
            
            if (i < nodes.size() && nodes[i] != "null") {
                node->right = new TreeNode(stoi(nodes[i]));
                q.push(node->right);
            }
            i++;
        }
        return root;
    }
};
```

---

## N-ary Tree Serialization

**LeetCode 428**

```cpp
class Codec {
public:
    string serialize(Node* root) {
        if (!root) return "";
        string result = to_string(root->val) + " " + 
                        to_string(root->children.size()) + " ";
        for (Node* child : root->children) {
            result += serialize(child);
        }
        return result;
    }
    
    Node* deserialize(string data) {
        if (data.empty()) return nullptr;
        istringstream iss(data);
        return buildTree(iss);
    }
    
private:
    Node* buildTree(istringstream& iss) {
        int val, size;
        iss >> val >> size;
        Node* node = new Node(val);
        for (int i = 0; i < size; i++) {
            node->children.push_back(buildTree(iss));
        }
        return node;
    }
};
```

---

## Practice Problems

| Problem | Difficulty | Key Insight |
|---------|------------|-------------|
| LC 297 - Serialize Binary Tree | Hard | Preorder + null markers |
| LC 449 - Serialize BST | Medium | Use BST bounds, no nulls |
| LC 428 - Serialize N-ary Tree | Hard | Store child count |
| LC 652 - Find Duplicate Subtrees | Medium | Serialize subtrees, use map |
