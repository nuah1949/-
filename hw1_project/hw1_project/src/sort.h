#ifndef SORT_H
#define SORT_H

#include <stdint.h>

typedef struct {
    int key;
    int id;
} Record;

typedef struct {
    long long comparisons;
    long long moves;
    int maxDepth;
} SortStats;

typedef void (*SortFunction)(Record *, int, SortStats *);

static inline int compare_key(const Record *a, const Record *b, SortStats *s) {
    s->comparisons++;
    return (a->key > b->key) - (a->key < b->key);
}

static inline void move_record(Record *dst, const Record *src, SortStats *s) {
    *dst = *src;
    s->moves++;
}

static inline void swap_record(Record *a, Record *b, SortStats *s) {
    if (a == b) return;
    Record t = *a;
    s->moves++;
    *a = *b;
    s->moves++;
    *b = t;
    s->moves++;
}

/* Lecture-style minstd generator used for reproducible random pivots. */
void setSeed(int64_t seed);
int64_t nextRandom(void);

void insertionSort(Record *a, int n, SortStats *stats);
void quickSortRandom(Record *a, int lo, int hi, SortStats *stats);
void quickSortFirst(Record *a, int lo, int hi, SortStats *stats);
void quickSortMiddle(Record *a, int lo, int hi, SortStats *stats);
void quickSortMedian3(Record *a, int lo, int hi, SortStats *stats);
void heapSort(Record *a, int n, SortStats *stats);

#endif
