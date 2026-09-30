#include "sort.h"

static int partitionRandom(Record *a, int lo, int hi, SortStats *s) {
    int p = lo + (int)(nextRandom() % (hi - lo + 1));
    swap_record(&a[lo], &a[p], s);
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
    int p = partitionRandom(a, lo, hi, s);
    qs(a, lo, p - 1, s, depth + 1);
    qs(a, p + 1, hi, s, depth + 1);
}

void quickSortRandom(Record *a, int lo, int hi, SortStats *stats) {
    if (lo < hi) qs(a, lo, hi, stats, 1);
}
