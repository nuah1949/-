# HW1 - Sorting Algorithm Comparison

## Algorithms

- Insertion Sort
- Quick Sort
- Heap Sort

## Build

```bash
gcc -std=c17 -Wall -Wextra -O2 src/main.c src/insertion_sort.c src/quick_sort.c src/heap_sort.c -o hw1
```

## Run

```bash
./hw1
```

The program checks sorting correctness and empirical stability, then measures CPU time, comparisons, moves, and quicksort recursion depth on random, sorted, reverse-sorted, and duplicate-heavy inputs.

## Folder structure

```text
hw1/
├── src/
│   ├── sort.h
│   ├── insertion_sort.c
│   ├── quick_sort.c
│   ├── heap_sort.c
│   └── main.c
├── report/
│   └── REPORT.md
├── images/
└── README.md
```
