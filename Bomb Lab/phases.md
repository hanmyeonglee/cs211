# Phase solutions

## Phase 1

### Input Solution

> `I am for medical liability at the federal level.`

### Pseudocode

```py
string = input()
if string != "I am for medical liability at the federal level.": explode_bomb()
```

## Phase 2

### Input Solution

> `1 2 4 8 16 32`

### Pseudocode

```py
nums = list(map(int, input().split()))
if len(nums) < 6: explode_bomb()

for i in range(len(nums) - 1):
    n1, n2 = nums[i], nums[i+1]
    if 2 * n1 != n2: explode_bomb()
```

## Phase 3

### Input Solution

> `0 247`  
`1 880`  
`2 336`  
`3 555`  
`4 417`  
`5 853`  
`6 91`  
`7 601`

### Pseudocode

```py
a, b = map(int, input().split())
match a:
    case 0: target = 247
    case 1: target = 880
    case 2: target = 336
    case 3: target = 555
    case 4: target = 417
    case 5: target = 853
    case 6: target = 91
    case 7: target = 601
    case _: explode_bomb()

if b != target: explode_bomb() 
```

## Phase 4

### Input Solution

> `7 7`

> `7 7 DrJisunPark` (if you want `secret_phase`)

### Pseudocode

```py
a, b = map(int, input().split())
if a > 14: explode_bomb()

def recfunc(n, low, high):
    mid = (low + high) // 2
    
    if n < mid:
        return mid + recfunc(n, low, high - 1)
    elif n > mid:
        return mid + recfunc(n, low + 1, high)

    return mid

result = recfunc(a, 0, 14)
if result != 7 or b != 7:
    explode_bomb()
```

## Phase 5

### Input Solution

> `5 115`

### Pseudocode

```py
L = 10, 2, 14, 7, 8, 12, 15, 11, 0, 4, 1, 13, 3, 9, 6, 5

a, b = map(int, input().split())
b &= 0xF

if b == 0xF: explode_bomb()

x = y = 0
while b != 15:
    y += 1
    b = L[b]
    x += b

if y != 15 or a != x: explode_bomb()
```

## Phase 6

### Input Solution

> 6 3 1 4 2 5

### Pseudocode

```py
nums = list(map(int, input().split()))
assert len(nums) == 6

if (
    any(num > 6 for num in nums) or
    len(set(nums)) < 6
):
    explode_bomb()

class Node:
    def __init__(self, id, val):
        self.id = id
        self.val = val
        self.next = None

node1 = Node(1, 0x24c)
node2 = Node(2, 0x354)
node3 = Node(3, 0x16d)
node4 = Node(4, 0x32e)
node5 = Node(5, 0x379)
node6 = Node(6, 0x11e)

node1.next = node2
node2.next = node3
node3.next = node4
node4.next = node5
node5.next = node6

nodes = []
for i in range(0, 24, 4):
    n = nums[i // 4]
    node = node1
    if n > 1:
        j = 1
        while j < n:
            node = node.next
            j += 1
    
    nodes.append(node)

for n1, n2 in zip(nodes[:-1], nodes[1:]):
    if n1.val > n2.val: explode_bomb()
```

## Secret Phase

### Input Solution

> `35`

### Pseudocode

```py
def recfunc(tree, i, number):
    if i >= len(tree): return -1

    N = tree[i]
    if N > number:
        return 2 * recfunc(tree, 2 * i + 1, number)
    elif N < number:
        return 2 * recfunc(tree, 2 * i + 2, number) + 1
    else:
        return 0

TREE = [0x24, 0x8, 0x32, 0x6, 0x16, 0x2d, 0x6b, 0x1, 0x7, 0x14, 0x23, 0x28, 0x2f, 0x63, 0x3e9]
n = int(input())
if n > 1001: explode_bomb()
if recfunc(TREE, 0, n) != 6: explode_bomb()
```