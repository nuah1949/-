#include "sort.h"

static int medianIndex(Record *a, int lo, int mid, int hi, SortStats *s) {
    int ab = compare_key(&a[lo], &a[mid], s);
    int ac = compare_key(&a[lo], &a[hi], s);
    int bc = compare_key(&a[mid], &a[hi], s);
    if ((ab <= 0 && bc <= 0) || (ab >= 0 && bc >= 0)) return mid;
    if ((ab <= 0 && ac >= 0) || (ab >= 0 && ac <= 0)) return lo;
    return hi;
}

static int partitionMedian3(Record *a, int lo, int hi, SortStats *s) {
    int mid = lo + (hi - lo) / 2;
    int p = medianIndex(a, lo, mid, hi, s);
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
    int p = partitionMedian3(a, lo, hi, s);
    qs(a, lo, p - 1, s, depth + 1);
    qs(a, p + 1, hi, s, depth + 1);
}

void quickSortMedian3(Record *a, int lo, int hi, SortStats *stats) {
    if (lo < hi) qs(a, lo, hi, stats, 1);
}
