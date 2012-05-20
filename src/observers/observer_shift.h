#ifndef _INCLUDED_OBSERVER_SHIFT_H
#define _INCLUDED_OBSERVER_SHIFT_H

#include "observer_base.h"


typedef struct _observer_shift_t observer_shift_t;

extern observer_shift_t *observer_shift_new(
    engine_reg_t reg,
    observer_cb_t, void *ctxt,
    unsigned warn_start, unsigned warn_end,
    /* number of steps "in the middle". E.g. for 3 intervals there are 5 states:
     * under, interval[1,2,3], over */
    unsigned intervals
);

/* gets the shift warning level, [0 - intervals+1]
 *              x < warn_start  => 0           - no warning
 * warn_start < x < warn_end    => 1-intervals - warning
 * warn_end   < x               => intervals+1 - shift right now
 */
extern void observer_shift_get_shift( observer_base_t *obs, unsigned *warn_level );

#endif /* defined _INCLUDED_OBSERVER_SHIFT_H */
