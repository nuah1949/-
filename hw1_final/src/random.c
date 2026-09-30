#include "sort.h"

static int64_t state = 1;

void setSeed(int64_t seed) {
    state = (seed <= 0) ? 1 : seed;
}

int64_t nextRandom(void) {
    state = state * 16807 % 2147483647;
    return state;
}
