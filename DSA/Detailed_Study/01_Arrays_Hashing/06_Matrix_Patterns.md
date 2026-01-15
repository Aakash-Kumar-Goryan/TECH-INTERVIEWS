# Matrix Patterns

> **Trigger**: 2D grid traversal, rotation, spiral order, search in matrix

---

## Spiral Order

**LeetCode 54** | O(m×n) time

```cpp
vector<int> spiralOrder(vector<vector<int>>& matrix) {
    vector<int> result;
    if (matrix.empty()) return result;
    
    int top = 0, bottom = matrix.size() - 1;
    int left = 0, right = matrix[0].size() - 1;
    
    while (top <= bottom && left <= right) {
        // Right
        for (int j = left; j <= right; j++)
            result.push_back(matrix[top][j]);
        top++;
        
        // Down
        for (int i = top; i <= bottom; i++)
            result.push_back(matrix[i][right]);
        right--;
        
        // Left
        if (top <= bottom) {
            for (int j = right; j >= left; j--)
                result.push_back(matrix[bottom][j]);
            bottom--;
        }
        
        // Up
        if (left <= right) {
            for (int i = bottom; i >= top; i--)
                result.push_back(matrix[i][left]);
            left++;
        }
    }
    return result;
}
```

---

## Rotate Matrix 90° Clockwise

**LeetCode 48** | O(n²) time, O(1) space

```cpp
void rotate(vector<vector<int>>& matrix) {
    int n = matrix.size();
    
    // Transpose
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            swap(matrix[i][j], matrix[j][i]);
        }
    }
    
    // Reverse each row
    for (int i = 0; i < n; i++) {
        reverse(matrix[i].begin(), matrix[i].end());
    }
}
```

**Rotations:**

- 90° clockwise: Transpose → Reverse rows
- 90° counter-clockwise: Reverse rows → Transpose
- 180°: Reverse rows → Reverse columns

---

## Set Matrix Zeroes

**LeetCode 73** | O(mn) time, O(1) space

```cpp
void setZeroes(vector<vector<int>>& matrix) {
    int m = matrix.size(), n = matrix[0].size();
    bool firstRowZero = false, firstColZero = false;
    
    // Check first row and column
    for (int j = 0; j < n; j++)
        if (matrix[0][j] == 0) firstRowZero = true;
    for (int i = 0; i < m; i++)
        if (matrix[i][0] == 0) firstColZero = true;
    
    // Mark zeros in first row/column
    for (int i = 1; i < m; i++) {
        for (int j = 1; j < n; j++) {
            if (matrix[i][j] == 0) {
                matrix[i][0] = 0;
                matrix[0][j] = 0;
            }
        }
    }
    
    // Set zeros based on marks
    for (int i = 1; i < m; i++) {
        for (int j = 1; j < n; j++) {
            if (matrix[i][0] == 0 || matrix[0][j] == 0)
                matrix[i][j] = 0;
        }
    }
    
    // Handle first row and column
    if (firstRowZero)
        for (int j = 0; j < n; j++) matrix[0][j] = 0;
    if (firstColZero)
        for (int i = 0; i < m; i++) matrix[i][0] = 0;
}
```

---

## Search in Sorted Matrix

**LeetCode 240** | O(m + n) time

```cpp
bool searchMatrix(vector<vector<int>>& matrix, int target) {
    int m = matrix.size(), n = matrix[0].size();
    int row = 0, col = n - 1;
    
    while (row < m && col >= 0) {
        if (matrix[row][col] == target) return true;
        else if (matrix[row][col] > target) col--;
        else row++;
    }
    return false;
}
```

---

## Practice Problems

| Problem | Difficulty | Key Insight |
|---------|------------|-------------|
| LC 54 - Spiral Matrix | Medium | Four pointers |
| LC 48 - Rotate Image | Medium | Transpose + reverse |
| LC 73 - Set Matrix Zeroes | Medium | Use first row/col as markers |
| LC 240 - Search 2D Matrix II | Medium | Start top-right |
| LC 74 - Search 2D Matrix | Medium | Binary search (treat as 1D) |
