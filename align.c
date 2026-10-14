#include <stdint.h>
#include <stddef.h>
#include "align.h"
#include "config_arena_alloc.h"

extern inline void *align_towards_lower_address(void *p);
extern inline void *align_towards_higher_address(void *p);

inline void *align_towards_lower_address(void *p)
{
        return (void *)( ((uintptr_t)p >> ALIGNMENT_IN_EXPONENTOF_2) << ALIGNMENT_IN_EXPONENTOF_2 );
}

inline void *align_towards_higher_address(void *p)
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
