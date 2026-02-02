# DEAP - a Heap with compile time D

D-ary heap as scaffolded template in pure C
\- a Zero-overhead abstraction goodness for your next C project.  

Made for people that want to program in C, but scared of the absence of Templates<>.  

Depends on [arena.h]('www.github.com/evjeesm/arena') which is included in the repo for your convenience.

The lib focuses on simplicity in a way that, 'T' which represents stored data type in  
the heap is being handled mostly by value, except 'peek' method.  
If the end user ~wishes to shoot his foot~ decides to work on references,  
is allowed to pass a pointer of the desired data type along with a matching compare callback accordingly.  

```c
typedef int* int_ptr;
DEAP_DECLARE(4, int_ptr)

int int_ptr_cmp(int_ptr a, int_ptr b) {...}
DEAP_IMPLEMENT(4, int_ptr, int_ptr_cmp)
```

'typedef' is required by design for parameter 'T' when working with pointers or complex types,  
in order to facilitate name mangling. (name T should be concatenateable)  


see ['deap_demo.c'](./deap_demo.c) for examples

Compile and run demo:
```c
gcc deap_demo.c -o deap_demo && ./deap_demo
```

---

## Usage:

Forward-declare api and typedefs by invoking 'DEAP_DECLARE' macro:  
```c
DEAP_DECLARE(4, int)
```

Specialize your template anywhere you want by invoking 'DEAP_IMPLEMENT' macro:  

```c
DEAP_IMPLEMENT(4,my_type_t, my_type_compare_func)
```

This allows you to make a classical source/header file pair,  
wrapping your specification in a separate translation unit, see [deap4_int.h](./deap4_int.h), [deap4_int.c](./deap4_int.c)  

```sh
gcc -I./demo4_int.h ./demo4_int.c -c
```

... or even convert it into stb-style library along with your best template specs, see [stb_deap4_int.h](./stb_deap4_int.h)  
Just prepend entire contents of "deap.h" and "arena.h" to that file.  

Than you can compile it into an object like that (gcc):  

```sh
gcc -xc -c -DDEAP4_INT_IMPLEMENTATION stb_deap4_int.h -o stb_deap4_int.o
```

---

P.S.: If want to say something like, "but author, your lib just shoot my foot already by reducing debugability to effective zero!",  
You are god damn right!  

Just kidding, here is a [script](./template_unwrap.sh) that builds pretty formatted version of the templated source code,  
so you can link against it and debug your project, like a nobel:

```sh

./template_unwrap.sh && gcc -g stb_deap4_int.c -c
gcc -g my_main.c stb_deap4_int.o -o debuggable_build
gdb ./debuggable_build

```

P.S.:  
  P.S.: 
    the sad part if that source fixes should be transplanted manually to the templated code.

