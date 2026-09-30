#include "sort.h"

static void siftDown(Record *a, int n, int root, SortStats *s) {
    while (1) {
        int child = root * 2 + 1;
        if (child >= n) return;
        if (child + 1 < n && compare_key(&a[child], &a[child + 1], s) < 0)
            ++child;
        if (compare_key(&a[root], &a[child], s) >= 0) return;
        swap_record(&a[root], &a[child], s);
        root = child;
    }
}

void heapSort(Record *a, int n, SortStats *stats) {
    for (int i = n / 2 - 1; i >= 0; --i) siftDown(a, n, i, stats);
    for (int end = n - 1; end > 0; --end) {
        swap_record(&a[0], &a[end], stats);
        siftDown(a, end, 0, stats);
    }
}
