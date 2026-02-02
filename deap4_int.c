#define ARENA_IMPLEMENTATION
#include "deap.h"

int compare_ints(const int *const a, const int *const b)
{
    return *b - *a;
}
DEAP_IMPLEMENT(4, int, compare_ints)
