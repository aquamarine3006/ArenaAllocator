#include <stdint.h>
#include <stddef.h>
#include "align.h"
#include "alloc.h"
#include "config_alloc.h"

static char heap[HEAP_SIZE] __attribute__((aligned(ALIGNMENT)));

static struct {
        void   *beg;
        void   *cursor;
        void   *virt_end;
        size_t virt_tot_size;
        void   *true_end;
} arena = { 
        .beg      = (void *)heap,
        .cursor   = (void *)heap,
        .true_end = (void *)&heap[HEAP_SIZE - 1U],
};

static enum {
        DESTROYED_STATE,
        CREATED_STATE,
} arena_status = DESTROYED_STATE;

static void arena_init(size_t size)
{
        void *arena_end     = (void *)&heap[size - 1U];

        arena.cursor        = arena.beg;
        arena.virt_end      = arena_end;
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
                arena_status = CREATED_STATE;
        }

        return status;
}

void *arena_alloc(const size_t size)
{
        void *cur = arena.cursor;

        if (arena_status == CREATED_STATE) {
                void *mem_end     = arena.virt_end;
                void *aligned_end = align_down((void *)((uintptr_t)mem_end + 1U));

                size_t available  = (uintptr_t)aligned_end - (uintptr_t)cur ;

                if (0U == size || available < size) {
                        cur = NULL;
                } else {
                        void *unaligned_next = (void *) &((char *)cur)[size - 1U];
                        void *next = align_up((void *) ((uintptr_t)unaligned_next + 1U));

                        arena.cursor = next;
                }
        } else {
                cur = NULL;
        }

        return cur;
}

void arena_reset(void)
{
        arena.cursor = arena.beg;
}

void *arena_get_cursor(void)
{
        void *ret = NULL;
        if (arena_status == CREATED_STATE) {
                ret = arena.cursor;
        }

        return ret;
}

size_t arena_used(void)
{
        size_t used = (size_t)((uintptr_t)arena.cursor - (uintptr_t)arena.beg);

        return used;
}

size_t arena_available(void)
{
        size_t avail = (size_t)((uintptr_t)arena.virt_end - (uintptr_t)arena.cursor + 1U);

        return avail;
}

void *arena_alloc_zero(size_t size)
{
        void *ret     = NULL;
        void *old_cur = arena.cursor;

        void *ptr     = arena_alloc(size);

        if (ptr != NULL) {
                ret = ptr;
                for(size_t i = 0; i < size; ++i) {
                        //zero-out (char *)old_cur[0] to (char *)old_cur[size - 1]
                        ((char *)old_cur)[i] = (char)0; 
                }
        }

        return ret;
}

status_t arena_rollback(size_t offset)
{
        status_t status = aFAILED;
        void *new_cur   = align_up((void *)((char *)arena.cursor - offset));

        _Bool outof_bound = (uintptr_t)arena.beg > (uintptr_t)new_cur;
        if (!(outof_bound || offset == 0)) {
                status       = aSUCCESS;
                arena.cursor = new_cur;
        }

        return status;
}
