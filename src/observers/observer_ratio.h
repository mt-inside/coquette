#ifndef _INCLUDED_OBSERVER_RATIO_H
#define _INCLUDED_OBSERVER_RATIO_H

#include "observer.h"


typedef struct _observer_ratio_t observer_ratio_t;

extern observer_ratio_t *observer_ratio_new(
    observer_cb_t, void *ctxt
);

/* Gets the raw value as a ratio of the max (with min normalised to 0).
 * Range is self-calibrating based on the range of observed values */
extern void observer_ratio_get_ratio( observer_t *obs, float *ratio );

#endif /* defined _INCLUDED_OBSERVER_RATIO_H */
