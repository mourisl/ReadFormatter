The class for extracting actual sequence from read, barcode, UMI. The format specification follows the one of [Chromap](https://github.com/haowenz/chromap).

### Example

g++ example.cpp

./a.out r1:0:3,r2:4:-1,bc:0:3,bc:12:15:-,um:11:15

Output
```
@r1
ATCG
-
ABCD
@r2
ATCGATCGATCG
-
EFGHIJKLMOPQ
@bc
CGATCGAT
-
QPOMDCBA
@um
GATCG
-
LMOPQ
```
