#ifndef CONFIG_ARENA_ALLOC_H
#define CONFIG_ARENA_ALLOC_H

#include <stddef.h>

#define HEAP_SIZE_LOG2 10
#define HEAP_SIZE  (1 << HEAP_SIZE_LOG2)
#define ALIGN_LOG2 4U
#define ALIGNMENT (1 << ALIGN_LOG2)

#endif

