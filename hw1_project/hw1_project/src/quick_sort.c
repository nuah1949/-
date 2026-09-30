#include "sort.h"

static int partition(Record *a, int lo, int hi, SortStats *stats)
{
    Record pivot;
    move_record(&pivot, &a[hi], stats);

    int i = lo;
    for (int j = lo; j < hi; ++j) {
        /* <= is intentional for this in-place, non-stable implementation. */
        if (compare_record(&a[j], &pivot, stats) <= 0) {
            swap_record(&a[i], &a[j], stats);
            ++i;
        }
    }

    swap_record(&a[i], &a[hi], stats);
    return i;
}

static void quick_sort_range(Record *a, int lo, int hi,
                             SortStats *stats, int depth)
{
    if (lo >= hi) return;

    if (depth > stats->max_depth) {
        stats->max_depth = depth;
    }

    int p = partition(a, lo, hi, stats);
    quick_sort_range(a, lo, p - 1, stats, depth + 1);
    quick_sort_range(a, p + 1, hi, stats, depth + 1);
}

void quick_sort(Record *a, int n, SortStats *stats)
{
    if (n <= 1) return;
    quick_sort_range(a, 0, n - 1, stats, 1);
}
