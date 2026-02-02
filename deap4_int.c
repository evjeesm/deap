#include "deap.h"

int compare_ints(const int *const a, const int *const b)
{
    return *b - *a;
}
DEAP_DECLARE(4, int)
DEAP_IMPLEMENT(4, int, compare_ints)
