/*
** Copypaste entire "deap.h" and "arena.h" contents here instead of including:
** ...
*/
#include "deap.h"
#include "arena.h"

#ifndef _DEAP4_INT_H_
#define _DEAP4_INT_H_

DEAP_DECLARE(4, int)

#endif/*_DEAP4_INT_H_*/

#if defined(DEAP4_INT_IMPLEMENTATION)

int compare_ints(const int a, const int b)
{
    return b - a;
}

DEAP_IMPLEMENT(4, int, compare_ints)

#endif/* defined(DEAP4_INT_IMPLEMENTATION) */
