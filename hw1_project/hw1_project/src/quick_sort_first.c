#include "sort.h"

static int partitionFirst(Record *a, int lo, int hi, SortStats *s) {
    Record pivot = a[lo];
    s->moves++;
    int i = lo;
    for (int j = lo + 1; j <= hi; ++j) {
        s->comparisons++;
        if (a[j].key < pivot.key) {
            ++i;
            swap_record(&a[i], &a[j], s);
        }
    }
    swap_record(&a[lo], &a[i], s);
    return i;
}

static void qs(Record *a, int lo, int hi, SortStats *s, int depth) {
    if (lo >= hi) return;
    if (depth > s->maxDepth) s->maxDepth = depth;
    int p = partitionFirst(a, lo, hi, s);
    qs(a, lo, p - 1, s, depth + 1);
    qs(a, p + 1, hi, s, depth + 1);
}

void quickSortFirst(Record *a, int lo, int hi, SortStats *stats) {
    if (lo < hi) qs(a, lo, hi, stats, 1);
}
