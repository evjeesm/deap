#include <stddef.h>
#include <stdlib.h>
#include <assert.h>
/*
** Copypaste entire "deap.h" and "arena.h" contents here instead of including:
** ...
*/
typedef size_t arena_mark_t;

typedef struct
{
    size_t size;
    size_t max_capacity;
    char memory[4 * 1024 * 1024];
} arena_t;

typedef struct
{
    size_t max_capacity;
} heap_arena_opts_t;

arena_t *_heap_arena(const heap_arena_opts_t *const opts);
void free_heap_arena(arena_t *const arena);
arena_mark_t arena_snapshot(arena_t *const arena);
arena_t *arena_heap_clone(const arena_t *const arena, const size_t extra_cap);
void arena_reset(arena_t *const arena);
void arena_rewind(arena_t *const arena, const arena_mark_t mark);
void *arena_alloc(arena_t *const arena, const size_t size);
void *arena_realloc(arena_t *const arena, void *const old_ptr, const size_t old_size, const size_t new_size);
/*
** Templates:
**   D - heap degree (num of children)
**   T - stored data type
*/
/*
** Forward-declare templated data structure for D and T, don't implement yet:
*/
/*
** Implement template respecting compile-time parameters D, T
** and 'is_prior' callback:
*/
typedef struct deap4_int_t deap4_int_t;
deap4_int_t *deap4_int_create(const size_t capacity, arena_t *const arena);
void deap4_int_push(deap4_int_t *const deap, const int element);
const int *deap4_int_peek(const deap4_int_t *const deap);
int deap4_int_pop(deap4_int_t *const deap);
void deap4_int_clear(deap4_int_t *const deap);
void deap4_int_reserve(deap4_int_t **const deap, const size_t new_cap);
size_t deap4_int_get_size(const deap4_int_t *const deap);
size_t deap4_int_get_capacity(const deap4_int_t *const deap);

int compare_ints(const int a, const int b)
{
    return b - a;
}
typedef struct deap4_int_t deap4_int_t;
deap4_int_t *deap4_int_create(const size_t capacity, arena_t *const arena);
void deap4_int_push(deap4_int_t *const deap, const int element);
const int *deap4_int_peek(const deap4_int_t *const deap);
int deap4_int_pop(deap4_int_t *const deap);
void deap4_int_clear(deap4_int_t *const deap);
void deap4_int_reserve(deap4_int_t **const deap, const size_t new_cap);
size_t deap4_int_get_size(const deap4_int_t *const deap);
size_t deap4_int_get_capacity(const deap4_int_t *const deap);

struct deap4_int_t
{
    size_t capacity;
    size_t size;
    arena_t *arena;
    int data[];
};

deap4_int_t *deap4_int_create(const size_t capacity, arena_t *const arena)
{
    deap4_int_t *deap = arena_alloc(arena, sizeof(*deap) + sizeof(int) * capacity);
    deap->capacity = capacity;
    deap->size = 0;
    deap->arena = arena;
    return deap;
}

void deap4_int_reserve(deap4_int_t **const deap, const size_t new_cap)
{
    assert(deap && *deap);
    if (new_cap <= (*deap)->capacity)
        return;
    const size_t old_size = sizeof(**deap) + sizeof(int) * ((*deap)->capacity);
    const size_t new_size = sizeof(**deap) + sizeof(int) * new_cap;
    *deap = arena_realloc((*deap)->arena, *deap, new_size, old_size);
    (*deap)->capacity = new_cap;
}

static void deap4_int_sift_up(deap4_int_t *const deap, size_t i);
static void deap4_int_sift_down(deap4_int_t *const deap, size_t i);

void deap4_int_push(deap4_int_t *const deap, const int element)
{
    assert(deap);
    assert(deap->size < deap->capacity);
    size_t i = deap->size++;
    deap->data[i] = element;
    deap4_int_sift_up(deap, i);
}

const int *deap4_int_peek(const deap4_int_t *const deap)
{
    assert(deap);
    if (0 == deap->size)
        return NULL;
    return (int *)&deap->data[0];
}

int deap4_int_pop(deap4_int_t *const deap)
{
    assert(deap);
    int min = deap->data[0];
    deap->size--;
    if (deap->size > 0)
    {
        deap->data[0] = deap->data[deap->size];
        deap4_int_sift_down(deap, 0);
    }
    return min;
}

void deap4_int_clear(deap4_int_t *const deap)
{
    assert(deap);
    deap->size = 0;
}

size_t deap4_int_get_size(const deap4_int_t *const deap)
{
    assert(deap);
    return deap->size;
}

size_t deap4_int_get_capacity(const deap4_int_t *const deap)
{
    assert(deap);
    return deap->capacity;
}

static void deap4_int_sift_up(deap4_int_t *const deap, size_t i)
{
    int element = deap->data[i];
    while (i > 0)
    {
        size_t p = (((i)-1) / 4);
        if (compare_ints(deap->data[p], element) >= 0)
        {
            break;
        }
        deap->data[i] = deap->data[p];
        i = p;
    }
    deap->data[i] = element;
}

static void deap4_int_sift_down(deap4_int_t *const deap, size_t i)
{
    int element = deap->data[i];
    for (;;)
    {
        size_t first = (4 * (i) + 1);
        if (first >= deap->size)
        {
            break;
        }
        size_t best = first;
        size_t last = first + 4;
        if (last > deap->size)
        {
            last = deap->size;
        }
        for (size_t c = first + 1; c < last; ++c)
        {
            if (compare_ints(deap->data[c], deap->data[best]) > 0)
            {
                best = c;
            }
        }
        if (compare_ints(element, deap->data[best]) >= 0)
        {
            break;
        }
        deap->data[i] = deap->data[best];
        i = best;
    }
    deap->data[i] = element;
}

arena_t g_default_arena = (arena_t){.max_capacity = 4 * 1024 * 1024};

arena_t *_heap_arena(const heap_arena_opts_t *const opts)
{
    arena_t *arena = malloc(sizeof(arena_t) + opts->max_capacity);
    if (!arena)
        return NULL;
    arena->max_capacity = opts->max_capacity;
    arena->size = 0;
    return arena;
}

void free_heap_arena(arena_t *const arena)
{
    assert(arena);
    free(arena);
}

arena_mark_t arena_snapshot(arena_t *const arena)
{
    assert(arena);
    return arena->size;
}

arena_t *arena_heap_clone(const arena_t *const arena, const size_t extra_cap)
{
    assert(arena);
    arena_t *new_arena = _heap_arena(
        &(heap_arena_opts_t){.max_capacity = 4 * 1024 * 1024, .max_capacity = arena->max_capacity + extra_cap});
    if (!new_arena)
        return NULL;
    new_arena->size = arena->size;
    for (size_t i = 0; i < arena->size; ++i)
    {
        new_arena->memory[i] = arena->memory[i];
    }
    return new_arena;
}

void arena_reset(arena_t *const arena)
{
    assert(arena);
    arena->size = 0;
}

void arena_rewind(arena_t *const arena, const arena_mark_t mark)
{
    assert(arena);
    arena->size = mark;
}

void *arena_alloc(arena_t *const arena, const size_t size)
{
    assert(arena);
    assert(arena->size + size <= 4 * 1024 * 1024 && "Arena exceeded maximum capacity");
    void *alloc = &arena->memory[arena->size];
    arena->size += size;
    return alloc;
}

void *arena_realloc(arena_t *const arena, void *const old_ptr, const size_t old_size, const size_t new_size)
{
    assert(arena);
    assert(old_ptr);
    if (new_size <= old_size)
        return old_ptr;
    char *new_ptr = arena_alloc(arena, new_size);
    for (size_t i = 0; i < old_size; ++i)
    {
        new_ptr[i] = ((char *)old_ptr)[i];
    }
    return new_ptr;
}
