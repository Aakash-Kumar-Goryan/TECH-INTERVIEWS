# LRU and LFU Cache

> **Trigger**: Design cache with eviction, O(1) get/put, frequency-based eviction

---

## LRU Cache

**LeetCode 146** | O(1) get and put

**Data Structures**:

- HashMap: key → Node pointer
- Doubly Linked List: ordered by recency (MRU at head, LRU at tail)

```cpp
class LRUCache {
    struct Node {
        int key, value;
        Node* prev;
        Node* next;
        Node(int k, int v) : key(k), value(v), prev(nullptr), next(nullptr) {}
    };
    
    int capacity;
    unordered_map<int, Node*> cache;
    Node* head;  // Dummy head (MRU side)
    Node* tail;  // Dummy tail (LRU side)
    
    void addToHead(Node* node) {
        node->next = head->next;
        node->prev = head;
        head->next->prev = node;
        head->next = node;
    }
    
    void removeNode(Node* node) {
        node->prev->next = node->next;
        node->next->prev = node->prev;
    }
    
    void moveToHead(Node* node) {
        removeNode(node);
        addToHead(node);
    }
    
    Node* removeTail() {
        Node* lru = tail->prev;
        removeNode(lru);
        return lru;
    }
    
public:
    LRUCache(int capacity) : capacity(capacity) {
        head = new Node(0, 0);
        tail = new Node(0, 0);
        head->next = tail;
        tail->prev = head;
    }
    
    int get(int key) {
        if (!cache.count(key)) return -1;
        
        Node* node = cache[key];
        moveToHead(node);  // Mark as recently used
        return node->value;
    }
    
    void put(int key, int value) {
        if (cache.count(key)) {
            Node* node = cache[key];
            node->value = value;
            moveToHead(node);
        } else {
            Node* node = new Node(key, value);
            cache[key] = node;
            addToHead(node);
            
            if (cache.size() > capacity) {
                Node* lru = removeTail();
                cache.erase(lru->key);
                delete lru;
            }
        }
    }
};
```

---

## LFU Cache

**LeetCode 460** | O(1) get and put

**Data Structures**:

- `keyToNode`: HashMap for O(1) access
- `freqToList`: Frequency → DLL of nodes with that frequency
- `minFreq`: Track minimum frequency for eviction

```cpp
class LFUCache {
    struct Node {
        int key, value, freq;
        Node* prev;
        Node* next;
        Node(int k, int v) : key(k), value(v), freq(1), prev(nullptr), next(nullptr) {}
    };
    
    struct DList {
        Node* head;
        Node* tail;
        int size;
        
        DList() : size(0) {
            head = new Node(0, 0);
            tail = new Node(0, 0);
            head->next = tail;
            tail->prev = head;
        }
        
        void addToHead(Node* node) {
            node->next = head->next;
            node->prev = head;
            head->next->prev = node;
            head->next = node;
            size++;
        }
        
        void remove(Node* node) {
            node->prev->next = node->next;
            node->next->prev = node->prev;
            size--;
        }
        
        Node* removeTail() {
            if (size == 0) return nullptr;
            Node* node = tail->prev;
            remove(node);
            return node;
        }
    };
    
    int capacity, minFreq;
    unordered_map<int, Node*> keyToNode;
    unordered_map<int, DList*> freqToList;
    
    void updateFreq(Node* node) {
        int oldFreq = node->freq;
        freqToList[oldFreq]->remove(node);
        
        if (freqToList[oldFreq]->size == 0 && oldFreq == minFreq)
            minFreq++;
        
        node->freq++;
        if (!freqToList.count(node->freq))
            freqToList[node->freq] = new DList();
        freqToList[node->freq]->addToHead(node);
    }
    
public:
    LFUCache(int capacity) : capacity(capacity), minFreq(0) {}
    
    int get(int key) {
        if (!keyToNode.count(key)) return -1;
        
        Node* node = keyToNode[key];
        updateFreq(node);
        return node->value;
    }
    
    void put(int key, int value) {
        if (capacity == 0) return;
        
        if (keyToNode.count(key)) {
            Node* node = keyToNode[key];
            node->value = value;
            updateFreq(node);
        } else {
            if (keyToNode.size() >= capacity) {
                Node* lfu = freqToList[minFreq]->removeTail();
                keyToNode.erase(lfu->key);
                delete lfu;
            }
            
            Node* node = new Node(key, value);
            keyToNode[key] = node;
            minFreq = 1;
            if (!freqToList.count(1))
                freqToList[1] = new DList();
            freqToList[1]->addToHead(node);
        }
    }
};
```

---

## Key Differences

| Feature | LRU | LFU |
|---------|-----|-----|
| Eviction | Least Recently Used | Least Frequently Used |
| Structure | 1 DLL | Multiple DLLs (per freq) |
| Complexity | Simpler | More complex |
| Use Case | Temporal locality | Frequency matters |

---

## Practice Problems

| Problem | Difficulty | Key Insight |
|---------|------------|-------------|
| LC 146 - LRU Cache | Medium | HashMap + DLL |
| LC 460 - LFU Cache | Hard | HashMap + freq-to-DLL map |
| LC 432 - All O(1) Data Structure | Hard | Similar freq tracking |
| LC 355 - Design Twitter | Medium | HashMap + heap/sorting |
