#include "sort.h"

void insertion_sort(Record *a, int n, SortStats *stats)
{
    if (n <= 1) return;

    for (int i = 1; i < n; ++i) {
        if (compare_record(&a[i - 1], &a[i], stats) <= 0) {
            continue;
        }

        Record temp;
        move_record(&temp, &a[i], stats);

        int j = i;
        while (j > 0 && compare_record(&a[j - 1], &temp, stats) > 0) {
            move_record(&a[j], &a[j - 1], stats);
            --j;
        }

        move_record(&a[j], &temp, stats);
    }
}
