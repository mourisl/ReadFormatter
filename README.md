The class for extracting actual sequence from read, barcode, UMI. The format specification follows the one of [Chromap](https://github.com/haowenz/chromap).

### Introduction
This is a class to that read in the description of where the true sequence is in read1 (r1), read2 (r2), barcode (bc), and UMI (um). For r1 and r2, the sequence is supposed to be in the sequence. bc and um's sequence is allowed to be in the fastq/fasta file header's comment field.

The format for specifying the read structure is a comma-separated list, where each element is like category:start:end:strand, where category is either r1, r2, bc, and um, start and end is 0-based inclusive range. The coordinate can be negative, meaning the distance to the end of the sequence. For example, -1 is the last position. Strand is "+" or "-" symbol and can be omitted. So "r1:0:3" means the first 4bp of read 1 file's sequence is the actual sequence for read 1.

To specify sequence in the header comment field, the second value should use keyword "hd" stands for header. The next component is the field in the comment (0-based, does not count read id), or a string representing the prefix of a string in the comment. Then it follows the "start:end:strand" convention to specify how to extract the information from the comment field. For example, the header looks like "@r1 UB:Z:NNNN CB:Z:ACGT", then "bc:hd:1:5:-1" or "bc:hd:CB:5:-1" will extract the barcode "ACGT".

### Example

g++ example.cpp

Read fastq file for r1, r2, bc, um is
```
@r  
ATCGATCGATCGATCG
+
ABCDEFGHIJKLMOPQ
```

./a.out r1:0:3,r2:4:-1,bc:0:3,bc:12:15:-,um:11:-2

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
GATC
-
LMOP
```

./a.out r1:0:3,r2:4:-1
