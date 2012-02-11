#ifndef _INCLUDED_OBSERVER_SHIFT_H
#define _INCLUDED_OBSERVER_SHIFT_H

#include "observer.h"


typedef struct _observer_shift_t observer_shift_t;

extern observer_shift_t *observer_shift_new(
    observer_cb_t, void *ctxt,
    unsigned warn_start, unsigned warn_end
);

/* gets the shift warning level, [0-4]
 *              x < warn_start  => 0   - no warning
 * warn_start < x < warn_end    => 1-3 - warning
 * warn_end   < x               => 4   - shift right now
 */
extern void observer_shift_get_shift( observer_t *this, unsigned *warn_level );

#endif /* defined _INCLUDED_OBSERVER_SHIFT_H */
