# Bit Manipulation

> **Trigger**: Toggle/check bits, XOR properties, power of 2, subset enumeration

---

## Essential Operations

```cpp
// Check if bit at position i is set
bool isBitSet(int n, int i) { return (n >> i) & 1; }

// Set bit at position i
int setBit(int n, int i) { return n | (1 << i); }

// Clear bit at position i
int clearBit(int n, int i) { return n & ~(1 << i); }

// Toggle bit at position i
int toggleBit(int n, int i) { return n ^ (1 << i); }

// Count set bits (Brian Kernighan)
int countBits(int n) {
    int count = 0;
    while (n) {
        n &= (n - 1);  // Clear lowest set bit
        count++;
    }
    return count;
}

// Check if power of 2
bool isPowerOf2(int n) { return n > 0 && (n & (n - 1)) == 0; }

// Get lowest set bit
int lowestSetBit(int n) { return n & (-n); }

// Clear lowest set bit
int clearLowestSetBit(int n) { return n & (n - 1); }
```

---

## XOR Properties

```
a ^ a = 0          (same numbers cancel)
a ^ 0 = a          (identity)
a ^ b ^ a = b      (associative/commutative)
a ^ b ^ b = a      (associative/commutative)
```

---

## Single Number (Find Unique)

**LeetCode 136** | O(n) time, O(1) space

```cpp
int singleNumber(vector<int>& nums) {
    int result = 0;
    for (int num : nums) {
        result ^= num;
    }
    return result;
}
```

---

## Single Number III (Two Unique Numbers)

**LeetCode 260** | O(n) time, O(1) space

```cpp
vector<int> singleNumber(vector<int>& nums) {
    long xorAll = 0;
    for (int num : nums) xorAll ^= num;
    
    // Find rightmost set bit (differs between the two numbers)
    long rightmostBit = xorAll & (-xorAll);
    
    int a = 0, b = 0;
    for (int num : nums) {
        if (num & rightmostBit) a ^= num;
        else b ^= num;
    }
    return {a, b};
}
```

---

## Counting Bits (0 to n)

**LeetCode 338** | O(n) time

```cpp
vector<int> countBits(int n) {
    vector<int> dp(n + 1, 0);
    for (int i = 1; i <= n; i++) {
        dp[i] = dp[i >> 1] + (i & 1);
    }
    return dp;
}
```

---

## Subsets Using Bitmask

**LeetCode 78** | O(n × 2ⁿ) time

```cpp
vector<vector<int>> subsets(vector<int>& nums) {
    int n = nums.size();
    vector<vector<int>> result;
    
    for (int mask = 0; mask < (1 << n); mask++) {
        vector<int> subset;
        for (int i = 0; i < n; i++) {
            if (mask & (1 << i)) {
                subset.push_back(nums[i]);
            }
        }
        result.push_back(subset);
    }
    return result;
}
```

---

## Reverse Bits

**LeetCode 190** | O(1) time

```cpp
uint32_t reverseBits(uint32_t n) {
    uint32_t result = 0;
    for (int i = 0; i < 32; i++) {
        result = (result << 1) | (n & 1);
        n >>= 1;
    }
    return result;
}
```

---

## Common Tricks Summary

| Task | Expression |
|------|------------|
| Check if even | `(n & 1) == 0` |
| Multiply by 2^k | `n << k` |
| Divide by 2^k | `n >> k` |
| Check power of 2 | `n & (n-1) == 0` |
| Get/set/clear/toggle bit i | See above |
| Swap without temp | `a ^= b; b ^= a; a ^= b;` |

---

## Practice Problems

| Problem | Difficulty | Key Insight |
|---------|------------|-------------|
| LC 136 - Single Number | Easy | XOR all |
| LC 260 - Single Number III | Medium | Split by rightmost bit |
| LC 338 - Counting Bits | Easy | dp[i] = dp[i>>1] + (i&1) |
| LC 191 - Number of 1 Bits | Easy | n & (n-1) |
| LC 78 - Subsets | Medium | Bitmask enumeration |
