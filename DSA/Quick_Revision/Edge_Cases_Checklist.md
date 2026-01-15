# Edge Cases Checklist

> **Use Before Coding**: Mentally run through these before writing code.  
> **Use After Coding**: Verify your solution handles these.

---

## Universal Edge Cases

```
□ Empty input ([], "", null)
□ Single element ([1], "a")
□ Two elements (minimum for comparison)
□ All same values ([5,5,5,5])
□ Already sorted / reverse sorted
□ Negative numbers (if applicable)
□ Zero (division, multiplication, index)
□ Integer overflow (INT_MAX, INT_MIN)
□ Very large n (10^5, 10^6)
```

---

## Arrays

```
□ Empty array
□ Single element
□ All duplicates
□ No duplicates
□ Target at boundaries (first/last)
□ Target not in array
□ Negative numbers
□ Sorted ascending / descending
□ Rotated array (pivot at start/end)
```

---

## Strings

```
□ Empty string ""
□ Single character "a"
□ All same characters "aaaa"
□ Palindrome vs non-palindrome
□ Case sensitivity (A vs a)
□ Special characters / spaces
□ Unicode / non-ASCII
```

---

## Linked Lists

```
□ Empty list (null head)
□ Single node
□ Two nodes
□ Cycle at head / middle / tail
□ No cycle
□ Odd vs even length
```

---

## Trees

```
□ Empty tree (null root)
□ Single node (root only)
□ Left-skewed (like linked list)
□ Right-skewed (like linked list)
□ Perfect binary tree
□ BST: all left or all right
□ Nodes with same values
□ Negative node values
```

---

## Graphs

```
□ Empty graph (no nodes)
□ Single node
□ Disconnected components
□ Fully connected (complete graph)
□ Self-loops
□ Multiple edges between same nodes
□ Directed vs undirected
□ Negative edge weights (for Dijkstra: invalid!)
□ Cycle vs DAG
```

---

## Dynamic Programming

```
□ n = 0 (empty)
□ n = 1 (base case)
□ Target = 0 (often valid answer)
□ Target > sum of all elements
□ All negative values
□ Exact boundary conditions
□ Overflow in sum/product
```

---

## Binary Search

```
□ Empty array
□ Single element (equal/not equal to target)
□ Target smaller than all elements
□ Target larger than all elements
□ Target at first/last position
□ Duplicate targets (first/last occurrence)
□ Even vs odd length array
```

---

## Intervals

```
□ Empty list
□ Single interval
□ No overlaps
□ All overlapping (same interval)
□ Touching intervals [1,2], [2,3]
□ Nested intervals [1,10], [2,3]
□ Negative bounds
```

---

## Quick Mental Checklist (FENS)

| Letter | Check |
|--------|-------|
| **F** | First element edge case? |
| **E** | Empty input handling? |
| **N** | Negative / null values? |
| **S** | Single element / size limits? |

---

## Interview Hack

When given a problem, **immediately ask**:

1. "Can the input be empty?"
2. "Can there be duplicates?"
3. "Can values be negative?"
4. "What's the max size of input?"

This shows thoroughness and buys thinking time.
