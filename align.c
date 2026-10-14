#include <stdint.h>
#include <stddef.h>
#include "align.h"
#include "config_alloc.h"

extern inline void *align_down(void *p);
extern inline void *align_up(void *p);

inline void *align_down(void *p)
{
        return (void *) (((uintptr_t)p >> ALIGN_LOG2) << ALIGN_LOG2);
}

inline void *align_up(void *p)
{
        void *aligned_up = NULL;
        uintptr_t aligned_down = (uintptr_t)align_down(p);

        if (aligned_down == (uintptr_t)p) {
                aligned_up = (void *)aligned_down;
        } else {
                aligned_up = (void *)(aligned_down + (1 << ALIGN_LOG2));
        }

        return aligned_up;
}
