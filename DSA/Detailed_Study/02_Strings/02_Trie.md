# Trie (Prefix Tree)

> **Trigger**: Prefix search, autocomplete, word dictionary, XOR problems

---

## Trie Implementation

```cpp
class TrieNode {
public:
    TrieNode* children[26] = {nullptr};
    bool isEnd = false;
};

class Trie {
    TrieNode* root;
    
public:
    Trie() { root = new TrieNode(); }
    
    void insert(string word) {
        TrieNode* node = root;
        for (char c : word) {
            int idx = c - 'a';
            if (!node->children[idx]) {
                node->children[idx] = new TrieNode();
            }
            node = node->children[idx];
        }
        node->isEnd = true;
    }
    
    bool search(string word) {
        TrieNode* node = searchPrefix(word);
        return node && node->isEnd;
    }
    
    bool startsWith(string prefix) {
        return searchPrefix(prefix) != nullptr;
    }
    
private:
    TrieNode* searchPrefix(string& s) {
        TrieNode* node = root;
        for (char c : s) {
            int idx = c - 'a';
            if (!node->children[idx]) return nullptr;
            node = node->children[idx];
        }
        return node;
    }
};
```

---

## Word Search II (Trie + Backtracking)

**LeetCode 212** | O(m × n × 4^L) time

```cpp
class Solution {
    struct TrieNode {
        TrieNode* children[26] = {nullptr};
        string word = "";
    };
    
    TrieNode* buildTrie(vector<string>& words) {
        TrieNode* root = new TrieNode();
        for (string& word : words) {
            TrieNode* node = root;
            for (char c : word) {
                int i = c - 'a';
                if (!node->children[i]) 
                    node->children[i] = new TrieNode();
                node = node->children[i];
            }
            node->word = word;
        }
        return root;
    }
    
    vector<string> result;
    
    void dfs(vector<vector<char>>& board, int i, int j, TrieNode* node) {
        if (i < 0 || i >= board.size() || j < 0 || j >= board[0].size()) return;
        
        char c = board[i][j];
        if (c == '#' || !node->children[c - 'a']) return;
        
        node = node->children[c - 'a'];
        if (!node->word.empty()) {
            result.push_back(node->word);
            node->word = ""; // avoid duplicates
        }
        
        board[i][j] = '#';
        dfs(board, i+1, j, node);
        dfs(board, i-1, j, node);
        dfs(board, i, j+1, node);
        dfs(board, i, j-1, node);
        board[i][j] = c;
    }
    
public:
    vector<string> findWords(vector<vector<char>>& board, vector<string>& words) {
        TrieNode* root = buildTrie(words);
        
        for (int i = 0; i < board.size(); i++) {
            for (int j = 0; j < board[0].size(); j++) {
                dfs(board, i, j, root);
            }
        }
        return result;
    }
};
```

---

## Design Add and Search Words

**LeetCode 211** | Trie with wildcard

```cpp
class WordDictionary {
    struct TrieNode {
        TrieNode* children[26] = {nullptr};
        bool isEnd = false;
    };
    
    TrieNode* root;
    
    bool searchHelper(string& word, int idx, TrieNode* node) {
        if (!node) return false;
        if (idx == word.size()) return node->isEnd;
        
        char c = word[idx];
        if (c == '.') {
            for (int i = 0; i < 26; i++) {
                if (searchHelper(word, idx + 1, node->children[i]))
                    return true;
            }
            return false;
        }
        return searchHelper(word, idx + 1, node->children[c - 'a']);
    }
    
public:
    WordDictionary() { root = new TrieNode(); }
    
    void addWord(string word) {
        TrieNode* node = root;
        for (char c : word) {
            int i = c - 'a';
            if (!node->children[i]) 
                node->children[i] = new TrieNode();
            node = node->children[i];
        }
        node->isEnd = true;
    }
    
    bool search(string word) {
        return searchHelper(word, 0, root);
    }
};
```

---

## Practice Problems

| Problem | Difficulty | Key Insight |
|---------|------------|-------------|
| LC 208 - Implement Trie | Medium | Basic implementation |
| LC 211 - Add and Search Words | Medium | DFS for wildcard |
| LC 212 - Word Search II | Hard | Trie + backtracking |
| LC 642 - Autocomplete System | Hard | Trie + priority queue |
| LC 421 - Max XOR of Two Numbers | Medium | Bit trie |
