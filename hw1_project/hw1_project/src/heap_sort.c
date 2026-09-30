#include "sort.h"

static void sift_down(Record *a, int n, int root, SortStats *stats)
{
    int current = root;

    while (1) {
        int largest = current;
        int left = 2 * current + 1;
        int right = left + 1;

        if (left < n &&
            compare_record(&a[left], &a[largest], stats) > 0) {
            largest = left;
        }

        if (right < n &&
            compare_record(&a[right], &a[largest], stats) > 0) {
            largest = right;
        }

        if (largest == current) break;

        swap_record(&a[current], &a[largest], stats);
        current = largest;
    }
}

void heap_sort(Record *a, int n, SortStats *stats)
{
    if (n <= 1) return;

    for (int i = n / 2 - 1; i >= 0; --i) {
        sift_down(a, n, i, stats);
    }

    for (int end = n - 1; end > 0; --end) {
        swap_record(&a[0], &a[end], stats);
        sift_down(a, end, 0, stats);
    }
}
