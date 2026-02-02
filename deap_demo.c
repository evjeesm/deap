#include "deap.h"

#include <stdio.h>

// int compare_ints(const int *const first, const int *const second);
int compare_ints(const int *const a, const int *const b)
{
    return *b - *a;
}

#define ARENA_IMPLEMENTATION
#include "arena.h"

DEAP_IMPLEMENT(4,int,compare_ints)

typedef DEAP(4,int) d4i_t;
#define d4i_create  DEAP_FN(create,4,int)
#define d4i_push    DEAP_FN(push,4,int)
#define d4i_pop     DEAP_FN(pop,4,int)

typedef struct A
{
    int a;
    char b;
}
A;

// DEAP_DECLARE(8,A)
// DEAP_IMPLEMENT(8,A)

#define TMP(T, x) (T[1]){(x)}

// int compare_ints(const int *const first, const int *const second)
// {
//     return *second - *first;
// }

int main(void)
{
    arena_t *arena = heap_arena();
    d4i_t *deap = d4i_create(256, arena);

    d4i_push(deap, TMP(int, 9));
    d4i_push(deap, TMP(int, 1));
    d4i_push(deap, TMP(int, 3));
    d4i_push(deap, TMP(int, 7));
    d4i_push(deap, TMP(int, 2));
    d4i_push(deap, TMP(int, 8));
    d4i_push(deap, TMP(int, 3));

    int val;
    val = d4i_pop(deap); printf("%d\n", val);
    val = d4i_pop(deap); printf("%d\n", val);
    val = d4i_pop(deap); printf("%d\n", val);
    val = d4i_pop(deap); printf("%d\n", val);
    val = d4i_pop(deap); printf("%d\n", val);
    val = d4i_pop(deap); printf("%d\n", val);
    val = d4i_pop(deap); printf("%d\n", val);

    free_heap_arena(arena);
    return 0;
}
