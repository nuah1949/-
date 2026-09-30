#include "sort.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef void (*BenchFn)(Record *, int, SortStats *);

typedef struct {
    const char *name;
    BenchFn fn;
    int isQuickRandom;
} Algorithm;

typedef enum { RANDOM_INPUT, SORTED_INPUT, REVERSE_INPUT, DUPLICATES_INPUT } InputType;

static int64_t dataRandom(int64_t *state) {
    *state = *state * 16807 % 2147483647;
    return *state;
}

static void makeInput(Record *a, int n, InputType type) {
    int64_t state = 1;
    for (int i = 0; i < n; ++i) {
        a[i].id = i;
        if (type == RANDOM_INPUT) a[i].key = (int)(dataRandom(&state) % n);
        else if (type == SORTED_INPUT) a[i].key = i;
        else if (type == REVERSE_INPUT) a[i].key = n - i;
        else a[i].key = (int)(dataRandom(&state) % 20);
    }
}

static int isSorted(const Record *a, int n) {
    for (int i = 1; i < n; ++i)
        if (a[i - 1].key > a[i].key) return 0;
    return 1;
}

static int isStable(const Record *a, int n) {
    for (int i = 1; i < n; ++i)
        if (a[i - 1].key == a[i].key && a[i - 1].id > a[i].id) return 0;
    return 1;
}

static void insertionBench(Record *a, int n, SortStats *s) { insertionSort(a, n, s); }
static void quickRandomBench(Record *a, int n, SortStats *s) {
    setSeed(12345);
    quickSortRandom(a, 0, n - 1, s);
}
static void quickFirstBench(Record *a, int n, SortStats *s) { quickSortFirst(a, 0, n - 1, s); }
static void quickMiddleBench(Record *a, int n, SortStats *s) { quickSortMiddle(a, 0, n - 1, s); }
static void quickMedian3Bench(Record *a, int n, SortStats *s) { quickSortMedian3(a, 0, n - 1, s); }
static void heapBench(Record *a, int n, SortStats *s) { heapSort(a, n, s); }

static double runOnce(const Record *input, Record *work, int n,
                      const Algorithm *alg, SortStats *out) {
    memcpy(work, input, (size_t)n * sizeof(*work));
    *out = (SortStats){0, 0, 0};
    clock_t start = clock();
    alg->fn(work, n, out);
    clock_t end = clock();
    return 1000.0 * (double)(end - start) / CLOCKS_PER_SEC;
}

static void printHeader(void) {
    printf("Algorithm,Input,n,Time_ms,Comparisons,Moves,Depth,Correct,Stable\n");
}

static const char *inputName(InputType t) {
    const char *names[] = {"random", "sorted", "reverse", "duplicates"};
    return names[t];
}

static void benchmark(FILE *csv, const Algorithm *algs, int algCount,
                      InputType type, int n, int repeats) {
    Record *input = malloc((size_t)n * sizeof(*input));
    Record *work = malloc((size_t)n * sizeof(*work));
    if (!input || !work) { fprintf(stderr, "memory allocation failed\n"); exit(1); }
    makeInput(input, n, type);

    for (int ai = 0; ai < algCount; ++ai) {
        double timeSum = 0;
        long long comp = 0, moves = 0;
        int depth = 0, correct = 1, stable = 1;
        for (int r = 0; r < repeats; ++r) {
            SortStats s;
            double ms = runOnce(input, work, n, &algs[ai], &s);
            timeSum += ms;
            comp += s.comparisons;
            moves += s.moves;
            if (s.maxDepth > depth) depth = s.maxDepth;
            correct &= isSorted(work, n);
            stable &= isStable(work, n);
        }
        double tm = timeSum / repeats;
        long long c = comp / repeats;
        long long m = moves / repeats;
        printf("%-16s %-10s n=%5d  time=%9.3f ms  cmp=%10lld  moves=%10lld  depth=%5d  %s  %s\n",
               algs[ai].name, inputName(type), n, tm, c, m, depth,
               correct ? "PASS" : "FAIL", stable ? "stable" : "unstable");
        if (csv) fprintf(csv, "%s,%s,%d,%.6f,%lld,%lld,%d,%s,%s\n",
                         algs[ai].name, inputName(type), n, tm, c, m, depth,
                         correct ? "PASS" : "FAIL", stable ? "PASS" : "FAIL");
    }
    free(input); free(work);
}

static void stabilityTest(const Algorithm *algs, int algCount) {
    const int n = 40, trials = 200;
    Record input[n], work[n];
    printf("\nStability test (%d trials):\n", trials);
    for (int ai = 0; ai < algCount; ++ai) {
        int pass = 1;
        for (int t = 0; t < trials; ++t) {
            for (int i = 0; i < n; ++i) {
                input[i].key = (int)((i * 17 + t * 7) % 5);
                input[i].id = i;
            }
            SortStats s;
            runOnce(input, work, n, &algs[ai], &s);
            if (!isStable(work, n)) { pass = 0; break; }
        }
        printf("  %-16s %s\n", algs[ai].name, pass ? "PASS" : "FAIL");
    }
}

int main(void) {
    const Algorithm algs[] = {
        {"Insertion", insertionBench, 0},
        {"Quick-Random", quickRandomBench, 1},
        {"Quick-First", quickFirstBench, 0},
        {"Quick-Middle", quickMiddleBench, 0},
        {"Quick-Median3", quickMedian3Bench, 0},
        {"Heap", heapBench, 0}
    };
    const int algCount = (int)(sizeof(algs) / sizeof(algs[0]));
    const int sizes[] = {1000, 5000, 10000};
    const int sizeCount = (int)(sizeof(sizes) / sizeof(sizes[0]));
    const int repeats = 3;

    printf("=== Pivot selection and input-size growth experiment ===\n");
    printf("repeats=%d, random-pivot seed=12345\n\n", repeats);

    FILE *csv = fopen("results_growth.csv", "w");
    if (!csv) { perror("results_growth.csv"); return 1; }
    printHeader();
    fprintf(csv, "Algorithm,Input,n,Time_ms,Comparisons,Moves,Depth,Correct,Stable\n");

    for (int si = 0; si < sizeCount; ++si)
        for (int type = RANDOM_INPUT; type <= DUPLICATES_INPUT; ++type)
            benchmark(csv, algs, algCount, (InputType)type, sizes[si], repeats);
    fclose(csv);

    stabilityTest(algs, algCount);
    printf("\nSaved: results_growth.csv\n");
    return 0;
}
