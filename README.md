## ArenaAllocator
### This a very minimal arena allocator written in C.
## Installation
### To use it put all the files in your folder and just include alloc.h and statically compile with alloc.c align.c
### Currently the header only provides the following functions

```c
    arena_create(const size_t);
    arena_alloc(const size_t);
    arena_reset(void);
    arena_get_cursor(void);
    arena_used(void);
    arena_available(void);
    arena_alloc_zero(size_t);
    arena_rollback(size_t);
```

## Contributions
### contributions are open....!
