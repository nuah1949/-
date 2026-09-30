#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "sort.h"

#define BENCHMARK_N 5000
#define REPEAT 3
#define STABILITY_TRIALS 100
#define STABILITY_N 50

typedef enum {
    INPUT_RANDOM,
    INPUT_SORTED,
    INPUT_REVERSE,
    INPUT_DUPLICATES
} InputType;

typedef struct {
    const char *name;
    SortFunction sort;
} Algorithm;

static unsigned int next_random(unsigned int *state)
{
    *state = (*state * 1664525u) + 1013904223u;
    return *state;
}

static const char *input_name(InputType type)
{
    switch (type) {
        case INPUT_RANDOM: return "random";
        case INPUT_SORTED: return "sorted";
        case INPUT_REVERSE: return "reverse";
        case INPUT_DUPLICATES: return "duplicates";
    }
    return "unknown";
}

static void fill_input(Record *a, int n, InputType type)
{
    unsigned int state = 20260929u;

    for (int i = 0; i < n; ++i) {
        a[i].id = i;

        switch (type) {
            case INPUT_RANDOM:
                a[i].key = (int)(next_random(&state) % 100000u);
                break;
            case INPUT_SORTED:
                a[i].key = i;
                break;
            case INPUT_REVERSE:
                a[i].key = n - i;
                break;
            case INPUT_DUPLICATES:
                a[i].key = (int)(next_random(&state) % 20u);
                break;
        }
    }
}

static int is_sorted(const Record *a, int n)
{
    for (int i = 1; i < n; ++i) {
        if (a[i - 1].key > a[i].key) return 0;
    }
    return 1;
}

static int is_stable(const Record *a, int n)
{
    for (int i = 1; i < n; ++i) {
        if (a[i - 1].key == a[i].key && a[i - 1].id > a[i].id) {
            return 0;
        }
    }
    return 1;
}

static double elapsed_ms(clock_t start, clock_t end)
{
    return 1000.0 * (double)(end - start) / (double)CLOCKS_PER_SEC;
}

static int correctness_test(SortFunction sort)
{
    const int sizes[] = {0, 1, 2, 10, 100};

    for (size_t s = 0; s < sizeof(sizes) / sizeof(sizes[0]); ++s) {
        int n = sizes[s];
        Record a[100];

        for (int i = 0; i < n; ++i) {
            a[i].key = (i * 37 + 11) % 23;
            a[i].id = i;
        }

        SortStats stats;
        reset_stats(&stats);
        sort(a, n, &stats);

        if (!is_sorted(a, n)) return 0;
    }

    return 1;
}

static int stability_test(SortFunction sort)
{
    for (int trial = 0; trial < STABILITY_TRIALS; ++trial) {
        Record a[STABILITY_N];
        unsigned int state = 9000u + (unsigned int)trial;

        for (int i = 0; i < STABILITY_N; ++i) {
            a[i].key = (int)(next_random(&state) % 5u);
            a[i].id = i;
        }

        SortStats stats;
        reset_stats(&stats);
        sort(a, STABILITY_N, &stats);

        if (!is_stable(a, STABILITY_N)) return 0;
    }

    return 1;
}

static void benchmark_case(const Algorithm *algorithm,
                           const Record *input,
                           int n,
                           const char *case_name)
{
    Record *work = malloc((size_t)n * sizeof(*work));
    if (work == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }

    double total_ms = 0.0;
    SortStats last_stats;
    int correct = 1;

    for (int r = 0; r < REPEAT; ++r) {
        /* Input preparation is outside the timed region. */
        memcpy(work, input, (size_t)n * sizeof(*work));

        SortStats stats;
        reset_stats(&stats);

        clock_t start = clock();
        algorithm->sort(work, n, &stats);
        clock_t end = clock();

        total_ms += elapsed_ms(start, end);
        last_stats = stats;

        if (!is_sorted(work, n)) correct = 0;
    }

    printf("%-12s %-12s %10.3f %14lld %12lld %8s %8d\n",
           algorithm->name,
           case_name,
           total_ms / REPEAT,
           last_stats.comparisons,
           last_stats.moves,
           correct ? "PASS" : "FAIL",
           last_stats.max_depth);

    free(work);
}

int main(void)
{
    const Algorithm algorithms[] = {
        {"Insertion", insertion_sort},
        {"Quick", quick_sort},
        {"Heap", heap_sort}
    };

    const InputType input_types[] = {
        INPUT_RANDOM,
        INPUT_SORTED,
        INPUT_REVERSE,
        INPUT_DUPLICATES
    };

    const int algorithm_count =
        (int)(sizeof(algorithms) / sizeof(algorithms[0]));
    const int input_count =
        (int)(sizeof(input_types) / sizeof(input_types[0]));

    Record *input = malloc((size_t)BENCHMARK_N * sizeof(*input));
    if (input == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        return EXIT_FAILURE;
    }

    printf("=== Correctness / Stability ===\n");
    for (int i = 0; i < algorithm_count; ++i) {
        printf("%-12s correct=%s, empirical_stability=%s\n",
               algorithms[i].name,
               correctness_test(algorithms[i].sort) ? "PASS" : "FAIL",
               stability_test(algorithms[i].sort) ? "PASS" : "FAIL");
    }

    printf("\n=== Benchmark ===\n");
    printf("n=%d, repeats=%d, timing=CPU time, copy excluded\n\n",
           BENCHMARK_N, REPEAT);
    printf("%-12s %-12s %10s %14s %12s %8s %8s\n",
           "Algorithm", "Input", "Time(ms)", "Comparisons",
           "Moves", "Correct", "Depth");
    printf("--------------------------------------------------------------------------\n");

    for (int t = 0; t < input_count; ++t) {
        fill_input(input, BENCHMARK_N, input_types[t]);

        for (int i = 0; i < algorithm_count; ++i) {
            benchmark_case(&algorithms[i], input, BENCHMARK_N,
                           input_name(input_types[t]));
        }
    }

    free(input);
    return EXIT_SUCCESS;
}
