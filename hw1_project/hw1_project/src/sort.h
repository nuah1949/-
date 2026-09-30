#ifndef SORT_H
#define SORT_H

typedef struct {
    int key;
    int id;
} Record;

typedef struct {
    long long comparisons;
    long long moves;
    int max_depth;
} SortStats;

typedef void (*SortFunction)(Record *a, int n, SortStats *stats);

static inline void reset_stats(SortStats *stats)
{
    stats->comparisons = 0;
    stats->moves = 0;
    stats->max_depth = 0;
}

static inline int compare_record(const Record *a, const Record *b,
                                 SortStats *stats)
{
    stats->comparisons++;
    if (a->key < b->key) return -1;
    if (a->key > b->key) return 1;
    return 0;
}

static inline void move_record(Record *dst, const Record *src,
                               SortStats *stats)
{
    *dst = *src;
    stats->moves++;
}

static inline void swap_record(Record *a, Record *b, SortStats *stats)
{
    if (a == b) return;
    Record temp;
    move_record(&temp, a, stats);
    move_record(a, b, stats);
    move_record(b, &temp, stats);
}

void insertion_sort(Record *a, int n, SortStats *stats);
void quick_sort(Record *a, int n, SortStats *stats);
void heap_sort(Record *a, int n, SortStats *stats);

#endif
