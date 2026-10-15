#ifndef ALLOC_H
#define ALLOC_H

#include <stdint.h>
#include <stddef.h>

typedef enum {
        aFAILED,
        aSUCCESS,
} status_t;

status_t arena_create(const size_t);
void *arena_alloc(const size_t);
void arena_reset(void);
void *arena_get_cursor(void);
size_t arena_used(void);
size_t arena_available(void);
void *arena_alloc_zero(size_t);
status_t arena_rollback(size_t);

#endif
