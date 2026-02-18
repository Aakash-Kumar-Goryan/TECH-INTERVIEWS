# Atlassian Code Design & DSA Interview Strategy

This document outlines a strategic approach to the Atlassian coding rounds based on recent interview experiences.

## 🎯 High-Level Overview

* **Format**: 2 Rounds (Code Design & Data Structures).
* **Duration**: Typically 45-60 minutes each.
* **Break**: ~30 minutes between rounds.
* **Platform**: Your own IDE (VS Code, IntelliJ, etc) + Screen Share. **AI tools must be disabled.**
* **Mindset**: Treat it as "Pair Programming". Communicate constantly.

---

## 🕒 The 30-Minute Gap

Use this time wisely to reset your brain.

1. **Step Away**: Physically leave your desk for 5-10 mins.
2. **Hydrate & Snack**: Brain needs glucose.
3. **Review Cheat Sheets**: Quickly glance at syntax for `Deque`, `PriorityQueue`, `HashMap`, or common Tree traversal patterns if needed.
4. **Do NOT Panic**: Use the last 5 mins to set up your IDE for the next round (clean file, main method ready).

---

## 🛠️ Round 1: Code Design (Low Level Design)

This round tests your ability to write clean, maintainable, and extensible code. It is **NOT** just about getting the output.

### Common Topics

* **Games**: Snake, Tic-Tac-Toe, Connect 4.
* **Rate Limiter**: Fixed Window, Token Bucket.
* **File System**: In-memory file structure.
* **Router**: Key-value store with wildcards.

### Time Management & Strategy

#### 1. The Discussion Phase (0:00 - 0:10)

* **Clarify Requirements**: Don't assume. Ask about grid size, boundaries, collision rules, etc.
* **Agree on Approach**: Sketch a high-level class diagram verbally.
  * *Example (Snake)*: "I'm thinking of a `Snake` class to hold the body (Deque), a `Board` class for state, and a `Game` controller."
* **Get Buy-in**: Wait for the interviewer to say "That sounds good."

#### 2. The Core Implementation (0:10 - 0:30)

* **Start Simple**: Implement the MVP.
  * *Snake*: Just a snake moving on an empty grid. No food, no walls yet.
* **Data Structures**:
  * Use `Deque` / `LinkedList` for Snake body (O(1) add head/remove tail).
  * Use `Set` for quick lookup of body positions (Collision detection).
* **Clean Code**:
  * Use meaningful variable names (`snakeHead`, `nextPosition`).
  * Keep methods short.

#### 3. Evolution & Edge Cases (0:30 - 0:45)

* **Add Complexity**: The interviewer will add features (Food, Walls, Teleporting).
* **Refactor**: Show you can adapt your code without rewriting everything.
* **Don't Over-engineer**: Don't implement a complex Design Pattern unless it solves the immediate problem.

### 💡 Focus On

* **Readability**: Can the interviewer read your code like a story?
* **Separation of Concerns**: Logic shouldn't be mixed with UI/Printing.
* **Extensibility**: How easy is it to add a new rule?

---

## 🧩 Round 2: Data Structures & Algorithms (DSA)

This round is traditional algorithmic problem solving.

### Common Topics

* **Trees & Graphs**: **Lowest Common Ancestor (LCA)**, BFS, DFS, Valid BST.
* **HashMaps**: Voting systems, Frequency counters.
* **Arrays/Strings**: Rank teams, finding patterns.

### Time Management & Strategy

#### 1. Understanding & Approach (0:00 - 0:10)

* **Visualise**: Draw (or type) an example case.
* **Propose Solution**: "I can use DFS to traverse..."
* **Complexity**: Mention Time/Space complexity *before* coding.

#### 2. The Implementation (0:10 - 0:35)

* **Stick to the Plan**: Don't change approach midway unless necessary.
* **Traps**:
  * *Graph/Tree*: Be comfortable with Node classes (`val`, `left`, `right`, `children`).
  * *Recursion*: Watch out for stack overflow or base cases.

#### 3. Debugging & Optimization (0:35 - 0:45)

* **Dry Run**: Walk through your code with a sample input line-by-line.
* **Stuck? Brute Force!**: If the optimal solution (e.g., O(N)) is tricky, ask: "I can get a working solution in O(N^2) quickly, should I start with that?" **A working brute force is better than a broken optimal solution.**
* **Communication**: "I'm stuck on how to handle this edge case. I'm thinking X or Y."

### 💡 Focus On

* **Corner Cases**: Null roots, single node trees, disconnected graphs.
* **Standard Library**: Know your language's `Map`, `Set`, `Queue`, `Stack` API by heart.
* **Correctness**: A solution that handles all edge cases is often preferred over a complex/buggy optimal one.

---

## ⚠️ "Must Knows" / Quick Checklist

### For Code Design (Snake/Game)

* **Grid**: 2D Array vs Map of coordinates (Sparse matrix).
* **Snake Body**: `Deque` (Double ended queue) is essential.
* **Game Loop**: `while(!gameOver) { update(); render(); }`.
* **Enums**: `Direction.UP`, `Direction.DOWN` etc.

### For DSA

* **LCA (Lowest Common Ancestor)**:
  * *With Parent Pointers*: Intersection of two linked lists logic.
  * *Without Parent Pointers (BST)*: Navigate based on values.
  * *Without Parent Pointers (Binary Tree)*: Recursive DFS returning user match.
* **BFS vs DFS**:
  * BFS for Shortest Path.
  * DFS for Exhaustive Search / Path finding.

## 🧠 Mental Game

* **Interviewer as Peer**: They are your colleague. Collaborate.
* **Silence is Bad**: Keep talking. "Now I'm implementing the move function..."
* **Feedback**: Take hints! If they say "Are you sure about that loop?", stop and re-check.
