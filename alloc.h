#ifndef ALLOC_H
#define ALLOC_H

#include <stdint.h>

typedef enum {
        STATUS_ENTRY,
        STATUS_INVALID,
        STATUS_OK,
} status_t;

status_t init_arena_alloc(const uint32_t size_requested);
void *arena_alloc(const uint32_t size_required);
void arena_free_all(void *p);

#endif
