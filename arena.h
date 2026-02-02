#ifndef _ARENA_H_
#define _ARENA_H_

#include <stddef.h>
#include <assert.h>

#ifndef ARENA_MAX_CAP
#  define ARENA_MAX_CAP 4*1024*1024
#endif

typedef size_t arena_mark_t;
typedef struct
{
    size_t size;
    size_t max_capacity;
    char   memory[ARENA_MAX_CAP];
}
arena_t;

typedef struct
{
    size_t max_capacity;
}
heap_arena_opts_t;

#define DEFAULT_ARENA (arena_t) {.max_capacity = ARENA_MAX_CAP }

#define DEFAULT_HEAP_ARENA_OPTS \
    .max_capacity = ARENA_MAX_CAP

#define heap_arena(...) _heap_arena(&(heap_arena_opts_t){DEFAULT_HEAP_ARENA_OPTS, __VA_ARGS__})

arena_t *_heap_arena(const heap_arena_opts_t *const opts);
void free_heap_arena(arena_t *const arena);
arena_mark_t arena_snapshot(arena_t *const arena);
arena_t *arena_heap_clone(const arena_t *const arena, const size_t extra_cap);
void arena_reset(arena_t *const arena);
void arena_rewind(arena_t *const arena, const arena_mark_t mark);
void *arena_alloc(arena_t *const arena, const size_t size);
void *arena_realloc(arena_t *const arena, void *const old_ptr, const size_t old_size, const size_t new_size);

#endif /* _ARENA_H_ */

#if defined(ARENA_IMPLEMENTATION)

#include <stdlib.h>

arena_t g_default_arena = DEFAULT_ARENA;

arena_t *_heap_arena(const heap_arena_opts_t *const opts)
{
    arena_t *arena = malloc(sizeof(arena_t) + opts->max_capacity);
    if (!arena) return NULL;
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

    arena_t *new_arena = heap_arena(.max_capacity = arena->max_capacity + extra_cap);
    if (!new_arena) return NULL;

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
    assert(arena->size + size <= ARENA_MAX_CAP
            && "Arena exceeded maximum capacity");

    void *alloc = &arena->memory[arena->size];
    arena->size += size;

    return alloc;
}

void *arena_realloc(arena_t *const arena, void *const old_ptr, const size_t old_size, const size_t new_size)
{
    assert(arena);
    assert(old_ptr);

    if (new_size <= old_size) return old_ptr;

    char *new_ptr = arena_alloc(arena, new_size);
    for (size_t i = 0; i < old_size; ++i)
    {
        new_ptr[i] = ((char*)old_ptr)[i];
    }

    return new_ptr;
}

#endif /* defined(ARENA_IMPLEMENTATION) */
