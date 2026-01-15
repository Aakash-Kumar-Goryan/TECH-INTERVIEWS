# Fast & Slow Pointers (Floyd's)

> **Trigger**: Cycle detection, find middle, find cycle start, happy number

---

## Template: Detect Cycle

```cpp
bool hasCycle(ListNode* head) {
    ListNode* slow = head;
    ListNode* fast = head;
    
    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
        if (slow == fast) return true;
    }
    return false;
}
```

---

## Template: Find Middle

```cpp
ListNode* findMiddle(ListNode* head) {
    ListNode* slow = head;
    ListNode* fast = head;
    
    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
    }
    return slow; // middle (or second middle if even)
}
```

---

## Find Cycle Start

**LeetCode 142** | O(n) time, O(1) space

```cpp
ListNode* detectCycle(ListNode* head) {
    ListNode* slow = head;
    ListNode* fast = head;
    
    // Find meeting point
    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
        if (slow == fast) break;
    }
    
    if (!fast || !fast->next) return nullptr;
    
    // Reset slow to head, move both at same speed
    slow = head;
    while (slow != fast) {
        slow = slow->next;
        fast = fast->next;
    }
    return slow; // cycle start
}
```

---

## Reverse Linked List

**LeetCode 206** | O(n) time

```cpp
ListNode* reverseList(ListNode* head) {
    ListNode* prev = nullptr;
    ListNode* curr = head;
    
    while (curr) {
        ListNode* next = curr->next;
        curr->next = prev;
        prev = curr;
        curr = next;
    }
    return prev;
}
```

---

## Palindrome Linked List

**LeetCode 234** | O(n) time, O(1) space

```cpp
bool isPalindrome(ListNode* head) {
    // Find middle
    ListNode* slow = head;
    ListNode* fast = head;
    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
    }
    
    // Reverse second half
    ListNode* prev = nullptr;
    while (slow) {
        ListNode* next = slow->next;
        slow->next = prev;
        prev = slow;
        slow = next;
    }
    
    // Compare halves
    ListNode* left = head;
    ListNode* right = prev;
    while (right) {
        if (left->val != right->val) return false;
        left = left->next;
        right = right->next;
    }
    return true;
}
```

---

## Practice Problems

| Problem | Difficulty | Key Insight |
|---------|------------|-------------|
| LC 141 - Linked List Cycle | Easy | Floyd's algorithm |
| LC 142 - Linked List Cycle II | Medium | Reset + same speed |
| LC 876 - Middle of Linked List | Easy | Fast moves 2x |
| LC 234 - Palindrome Linked List | Easy | Middle + reverse + compare |
| LC 143 - Reorder List | Medium | Middle + reverse + merge |
