#include <stdint.h>
#include <stddef.h>
#include "align.h"
#include "alloc.h"
#include "config_alloc.h"

static char heap[HEAP_SIZE] __attribute__((aligned(ALIGNMENT)));

static struct {
        void *beg;
        void *cursor;
        void *virt_end;
        size_t virt_tot_size;
        void *true_end;
} arena = {0};

void arena_init(size_t size)
{
        arena.beg           = heap;
        arena.cursor        = heap;
        arena.true_end      = (void *)&heap[HEAP_SIZE - 1U];
        arena.virt_end      = (void *)&heap[size - 1U];
        arena.virt_tot_size = size;
}

status_t arena_create(const size_t size)
{
        status_t status = aFAILED;

        if (0U == size || HEAP_SIZE < size) {
                status = aFAILED;
        } else {
                status = aSUCCESS;
                arena_init(size);
        }

        return status;
}

void *arena_alloc(const size_t size)
{
        void *cursor = arena.cursor;
        void *ncursor = (void *) ((char *)cursor + size - 1U);

        if (0U == size || (uintptr_t)ncursor > (uintptr_t)arena.virt_end) {
                cursor = NULL;
        } else {
                arena.cursor = align_up((void *) ((uintptr_t)ncursor + 1U));
        }

        return cursor;
}

void arena_reset(void)
{
        arena.cursor = arena.beg;
}
