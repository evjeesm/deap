#include "deap.h"

#include <stdio.h>

// int compare_ints(const int *const first, const int *const second);
int compare_ints(const int a, const int b)
{
    return b - a;
}

#define ARENA_IMPLEMENTATION
#include "arena.h"

/*
** Primitive by value example
*/
DEAP_IMPLEMENT(4,int,compare_ints)

typedef DEAP(4,int) d4i_t;
#define d4i_create  DEAP_FN(create,4,int)
#define d4i_push    DEAP_FN(push,4,int)
#define d4i_pop     DEAP_FN(pop,4,int)
#define d4i_peek    DEAP_FN(peek,4,int)

/*
** Struct by reference example
*/
typedef struct
{
    int priority;
    char payload[];
} A;
typedef A* A_ptr;

int compare_A(const A *restrict const a, const A *restrict const b)
{
    return b->priority - a->priority;
}
DEAP_IMPLEMENT(8,A_ptr,compare_A)

typedef DEAP(8,A_ptr) d8Aptr_t;
#define d8Aptr_create  DEAP_FN(create,8, A_ptr)
#define d8Aptr_push    DEAP_FN(push,  8, A_ptr)
#define d8Aptr_pop     DEAP_FN(pop,   8, A_ptr)
#define d8Aptr_peek    DEAP_FN(peek,  8, A_ptr)

#define TMP(T, x) (T[1]){(x)}
#define TMP_STRUCT(T, ...) &(T){__VA_ARGS__}

int main(void)
{
    arena_t *arena = heap_arena();
    d4i_t *deap = d4i_create(256, arena);

    printf("By value example:\n");

    d4i_push(deap, 9);
    d4i_push(deap, 1);
    d4i_push(deap, 3);

    const int *i = d4i_peek(deap);
    printf("peek (%d)\n", *i);

    d4i_push(deap, 7);
    d4i_push(deap, 2);
    d4i_push(deap, 8);
    d4i_push(deap, 3);

    int val;
    val = d4i_pop(deap); printf("%d\n", val);
    val = d4i_pop(deap); printf("%d\n", val);
    val = d4i_pop(deap); printf("%d\n", val);
    val = d4i_pop(deap); printf("%d\n", val);
    val = d4i_pop(deap); printf("%d\n", val);
    val = d4i_pop(deap); printf("%d\n", val);
    val = d4i_pop(deap); printf("%d\n", val);

    d8Aptr_t *deap_a = d8Aptr_create(10, arena);

    printf("By ref example:\n");

    d8Aptr_push(deap_a, TMP_STRUCT(A, .priority=9));
    d8Aptr_push(deap_a, TMP_STRUCT(A, .priority=1));
    d8Aptr_push(deap_a, TMP_STRUCT(A, .priority=3));

    const A_ptr *v = d8Aptr_peek(deap_a);
    printf("peek ({%d})\n", (*v)->priority);

    A_ptr a_val;
    a_val = d8Aptr_pop(deap_a); printf("%d\n", a_val->priority);
    a_val = d8Aptr_pop(deap_a); printf("%d\n", a_val->priority);
    a_val = d8Aptr_pop(deap_a); printf("%d\n", a_val->priority);

    free_heap_arena(arena);
    return 0;
}
