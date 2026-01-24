# A tiny Java “survival kit” (memorize only this)

You only need ~10 things for Stripe-style problems.

## Boilerplate

```java
import java.util.*;
import java.io.*;

public class Solution {
    public static void main(String[] args) {
        Scanner in = new Scanner(System.in);
        // ... code ...
    }
}
```

## Input

```java
while (in.hasNextLine()) {
    String line = in.nextLine();
}
```

## Output

```java
System.out.println(x);
```

## Conversions

```java
int val = Integer.parseInt(s);
String str = String.valueOf(x);
long y = Long.parseLong(s);
double d = Double.parseDouble(s);

try {
    int x = Integer.parseInt(s);
} catch (NumberFormatException e) {
    // handle invalid number
}
```

## Lists

```java
List<Integer> list = new ArrayList<>();
list.add(x);
list.get(i);
list.size();
```

## Maps

```java
Map<String, Integer> map = new HashMap<>();
map.put(k, v);
map.get(k);
map.getOrDefault(k, 0);
map.containsKey(k);
```

## Loops

```java
for (int i = 0; i < n; i++) { }
for (var e : list) { }
```

## Strings

```java
s.length();
s.charAt(i);
s.substring(a, b);
s.split(" ");
s.equals("val"); // Use .equals() for Strings. Always.
```

## Arrays

```java
int[] arr = new int[n];
```

## Sorting

```java
Collections.sort(list);
list.sort((a, b) -> a - b);
```

That’s it. No more.
