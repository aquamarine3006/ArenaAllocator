#ifndef ALLOC_H
#define ALLOC_H

#include <stdint.h>
#include <stddef.h>

typedef enum {
        aFAILED,
        aSUCCESS,
} status_t;

void arena_init(size_t);
status_t arena_create(const size_t);
void *arena_alloc(const size_t);
void arena_reset(void);

#endif
