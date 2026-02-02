#define ARENA_IMPLEMENTATION
#include "deap.h"

int compare_ints(const int a, const int b)
{
    return b - a;
}
DEAP_IMPLEMENT(4, int, compare_ints)
