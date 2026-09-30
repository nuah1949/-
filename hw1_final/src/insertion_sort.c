#include "sort.h"

void insertionSort(Record *a, int n, SortStats *stats) {
    for (int i = 1; i < n; ++i) {
        Record key = a[i];
        stats->moves++;
        int j = i - 1;
        while (j >= 0) {
            stats->comparisons++;
            if (a[j].key <= key.key) break;
            move_record(&a[j + 1], &a[j], stats);
            --j;
        }
        move_record(&a[j + 1], &key, stats);
    }
}
