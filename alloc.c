#include <stdint.h>
#include <stddef.h>

#define HEAP_MAX_SIZE 1024
#define ALIGNMENT_IN_EXPONENTOF_2 3

static char heap_array[HEAP_MAX_SIZE];

typedef enum {
        STATUS_ENTRY,
        STATUS_INVALID,
        STATUS_OK,
} status_t;

static inline void *align_towards_lower_address(void *p)
{
        return (void *)( ((uintptr_t)p >> ALIGNMENT_IN_EXPONENTOF_2) << ALIGNMENT_IN_EXPONENTOF_2 );
}

static inline void *align_towards_higher_address(void *p)
{
        void *aligned_towards_higher_addr = NULL;
        uintptr_t aligned_towards_lower_addr = (uintptr_t)align_towards_lower_address(p);

        if (aligned_towards_lower_addr == (uintptr_t)p) {
                aligned_towards_higher_addr = (void *)aligned_towards_lower_addr;
        } else {
                aligned_towards_higher_addr = (void *)( aligned_towards_lower_addr + 
                                                (1 << ALIGNMENT_IN_EXPONENTOF_2) );
        }

        return aligned_towards_higher_addr;
}

struct {
        void *heap_array_beginning;
        void *heap_cursor;
        void *heap_array_virtual_end;
        uint32_t heap_virtual_total_size;
        void *heap_array_true_end;
} heap_info = {0};

status_t init_arena_alloc(const uint32_t size_requested)
{
        status_t status = STATUS_ENTRY;
        /*enum { TRUE, FALSE} is_powerof_2 = FALSE;

        for(uint32_t i = 0; i < HEAP_MAX_SIZE_IN_EXPONENTOF_2; ++i) {
                if (size_requested == (uint32_t)(1 << i) ) {
                        is_powerof_2 = TRUE;
                }
        }*/

        if (HEAP_MAX_SIZE < size_requested || 
            0 == size_requested) {
                status = STATUS_INVALID;
        } else {
                status = STATUS_OK;
                heap_info.heap_array_beginning = align_towards_higher_address((void *)heap_array),
                heap_info.heap_cursor          = align_towards_higher_address((void *)heap_array),
                heap_info.heap_array_true_end  = align_towards_lower_address((void *)(char *)heap_array + (HEAP_MAX_SIZE - 1)),
                heap_info.heap_array_virtual_end = (void *)((char *)heap_array + (size_requested - 1));
                heap_info.heap_virtual_total_size = size_requested;
        }

        return status;
}

void *arena_alloc(const uint32_t size_required)
{
        status_t status = STATUS_ENTRY;

        void *cursor = heap_info.heap_cursor;
        void *new_cursor = (void *)((char *)cursor + size_required);
        if (0 == size_required ||
            (uintptr_t)new_cursor >= (uintptr_t)heap_info.heap_array_virtual_end) {
                status = STATUS_INVALID;
                cursor = NULL;
        } else {
                status = STATUS_OK;
                heap_info.heap_cursor = align_towards_higher_address( 
                                        (void *)new_cursor );
        }

        return cursor;
}

void arena_free_all(void *p)
{
        heap_info.heap_cursor = heap_info.heap_array_beginning;
}
