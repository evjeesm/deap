#include <stddef.h>
#include <stdlib.h>
#include <assert.h>
#include "arena.h"

#define PARENT(D, i)   (((i) - 1) / D)
#define FIRST_CHILD(D, i) (D*(i) + 1)

/*
** Templates:
**   D - heap degree (num of children)
**   T - stored data type
*/

#define DEAP_TEMPLATE(name,D,T) deap ## D ## _ ## T ## _ ## name
#define DEAP(D,T) DEAP_TEMPLATE(t,D,T)
#define DEAP_FN(name,D,T) DEAP_TEMPLATE(name,D,T)
#define DEAP_FN_PTR(name,D,T) (*DEAP_FN(name,D,T))

/*
** Forward-declare templated data structure for D and T, don't implement yet:
*/
#define DEAP_DECLARE(D,T)                                                       \
  typedef struct DEAP(D,T) DEAP(D,T);                                           \
  /* Compare returns greater than zero when 'a' comes before 'b' */             \
  /* typedef int DEAP_FN_PTR(is_prior_t,D,T) (const T *const a,                 \
       const T *const b); */                                                    \
  DEAP(D,T) * DEAP_FN(create,D,T) (const size_t capacity,                       \
                                   arena_t *const arena                         \
                                   /*,DEAP_FN(is_prior_t,D,T)*/ );              \
                                                                                \
  /*void        DEAP_FN(destroy,D,T) (DEAP(D,T) *const deap); */                \
  void    DEAP_FN(push   ,D,T) (DEAP(D,T) *const deap, T *const element);       \
  T *     DEAP_FN(peek   ,D,T) (const DEAP(D,T) *const deap);                   \
  T       DEAP_FN(pop    ,D,T) (DEAP(D,T) *const deap);                         \
  void    DEAP_FN(clear  ,D,T) (DEAP(D,T) *const deap);                         \
  void    DEAP_FN(reserve,D,T) (DEAP(D,T) **const deap, const size_t new_cap);  \

/*
** Implement template respecting compile-time parameters D, T
** and 'is_prior' callback:
*/
#define DEAP_IMPLEMENT(D,T,is_prior)                                            \
  DEAP_DECLARE(D,T)                                                             \
  struct DEAP(D,T) {                                                            \
      size_t capacity;                                                          \
      size_t size;                                                              \
      /* DEAP_FN(is_prior_t,D,T) is_prior;*/                                    \
      arena_t *arena;                                                           \
      T data[];                                                                 \
  };                                                                            \
                                                                                \
  DEAP(D,T) * DEAP_FN(create,D,T) (const size_t capacity,                       \
                                   arena_t *const arena                         \
                                  /*,DEAP_FN(is_prior_t,D,T) is_prior*/ ) {     \
      DEAP(D,T) *deap = arena_alloc(arena, sizeof(*deap) + sizeof(T)*capacity); \
      deap->capacity = capacity;                                                \
      deap->size = 0;                                                           \
      deap->arena = arena;                                                      \
      /* deap->is_prior = is_prior; */                                          \
      return deap;                                                              \
  }                                                                             \
                                                                                \
  /*void DEAP_FN(destroy,D,T) (DEAP(D,T) *const deap) {                         \
      assert(deap);                                                             \
      free(deap);                                                               \
  }*/                                                                           \
                                                                                \
  static void DEAP_FN(sift_up,D,T) (DEAP(D,T) *const deap, size_t i);           \
  static void DEAP_FN(sift_down,D,T) (DEAP(D,T) *const deap, size_t i);         \
                                                                                \
  void DEAP_FN(push,D,T) (DEAP(D,T) *const deap, T *const element) {            \
      assert(deap);                                                             \
                                                                                \
      assert(deap->size < deap->capacity);  /* TODO:  */                        \
                                                                                \
      size_t i = deap->size++;                                                  \
      deap->data[i] = *element;                                                 \
      DEAP_FN(sift_up,D,T)(deap, i);                                            \
  }                                                                             \
                                                                                \
  T * DEAP_FN(peek,D,T) (const DEAP(D,T) *const deap) {                         \
      assert(deap);                                                             \
      if (0 == deap->size) return NULL;                                         \
      return (T*)&deap->data[0];                                                \
  }                                                                             \
                                                                                \
  T DEAP_FN(pop,D,T) (DEAP(D,T) *const deap) {                                  \
      assert(deap);                                                             \
                                                                                \
      /* caller must ensure deap->size > 0 */                                   \
      T min = deap->data[0];                                                    \
                                                                                \
      deap->size--;                                                             \
      if (deap->size > 0) {                                                     \
          deap->data[0] = deap->data[deap->size];                               \
          DEAP_FN(sift_down,D,T)(deap, 0);                                      \
      }                                                                         \
                                                                                \
      return min;                                                               \
  }                                                                             \
                                                                                \
  void DEAP_FN(clear,D,T) (DEAP(D,T) *const deap) {                             \
      assert(deap);                                                             \
      deap->size = 0;                                                           \
  }                                                                             \
                                                                                \
  static void DEAP_FN(sift_up,D,T) (DEAP(D,T) *const deap, size_t i) {          \
      T element = deap->data[i];                                                \
      while (i > 0) {                                                           \
          size_t p = PARENT(D, i);                                              \
          T *parent = &deap->data[p];                                           \
          if (is_prior(parent, &element) >= 0) {                                \
              break;                                                            \
          }                                                                     \
          deap->data[i] = deap->data[p];                                        \
          i = p;                                                                \
      }                                                                         \
      deap->data[i] = element;                                                  \
  }                                                                             \
                                                                                \
  static void DEAP_FN(sift_down,D,T) (DEAP(D,T) *const deap, size_t i) {        \
    T element = deap->data[i];                                                  \
                                                                                \
    for (;;) {                                                                  \
        size_t first = FIRST_CHILD(D,i);                                        \
        if (first >= deap->size) {                                              \
            break;                                                              \
        }                                                                       \
                                                                                \
        size_t best = first;                                                    \
        size_t last = first + D;                                                \
        if (last > deap->size) {                                                \
            last = deap->size;                                                  \
        }                                                                       \
                                                                                \
        /* find min child */                                                    \
        for (size_t c = first + 1; c < last; ++c) {                             \
            if (is_prior(&deap->data[c], &deap->data[best]) > 0) {              \
                best = c;                                                       \
            }                                                                   \
        }                                                                       \
                                                                                \
        if (is_prior(&element, &deap->data[best]) >= 0) {                       \
            break;                                                              \
        }                                                                       \
                                                                                \
        deap->data[i] = deap->data[best];                                       \
        i = best;                                                               \
    }                                                                           \
                                                                                \
    deap->data[i] = element;                                                    \
  }                                                                             \
                                                                                \
  void DEAP_FN(reserve,D,T) (DEAP(D,T) **const deap, const size_t new_cap) {    \
      assert(deap && *deap);                                                    \
      if (new_cap <= (*deap)->capacity) return;                                  \
      const size_t old_size = sizeof(**deap)+sizeof(T)*((*deap)->capacity);     \
      const size_t new_size = sizeof(**deap)+sizeof(T)*new_cap;                 \
      *deap = arena_realloc((*deap)->arena, *deap, new_size, old_size);         \
      (*deap)->capacity = new_cap;                                              \
  }
