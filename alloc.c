#include <stdint.h>
#include <stddef.h>
#include "align.h"
#include "alloc.h"
#include "config_arena_alloc.h"

static char heap_array[HEAP_MAX_SIZE];

static struct {
        void *heap_array_beginning;
        void *heap_cursor;
        void *heap_array_virtual_end;
        uint32_t heap_virtual_total_size;
        void *heap_array_true_end;
} heap_info = {0};

status_t init_arena_alloc(const uint32_t size_requested)
{
        status_t status = STATUS_ENTRY;

        if (HEAP_MAX_SIZE < size_requested || 
            0 == size_requested) {
                status = STATUS_INVALID;
        } else {
                status = STATUS_OK;
                heap_info.heap_array_beginning = 
                        align_towards_higher_address((void *)heap_array),
                heap_info.heap_cursor          = 
                        align_towards_higher_address((void *)heap_array),
                heap_info.heap_array_true_end  = 
                        align_towards_lower_address((void *)(char *)heap_array + 
                                                    (HEAP_MAX_SIZE - 1)),
                heap_info.heap_array_virtual_end = 
                        (void *)((char *)heap_array + (size_requested - 1));
                heap_info.heap_virtual_total_size = size_requested;
        }

        return status;
}

void *arena_alloc(const uint32_t size_required)
{
        status_t status = STATUS_ENTRY;

        void *cursor = heap_info.heap_cursor;
        void *new_cursor = (void *)((char *)cursor + size_required - 1);

        if (0 == size_required ||
            (uintptr_t)(new_cursor) > (uintptr_t)heap_info.heap_array_virtual_end) {
                status = STATUS_INVALID;
                cursor = NULL;
        } else {
                status = STATUS_OK;
                heap_info.heap_cursor = align_towards_higher_address( 
                                        (void *)((uintptr_t)new_cursor + 1) );
        }

        return cursor;
}

void arena_free_all(void *p)
{
        heap_info.heap_cursor = heap_info.heap_array_beginning;
}
