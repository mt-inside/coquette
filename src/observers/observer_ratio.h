#ifndef _INCLUDED_OBSERVER_RATIO_H
#define _INCLUDED_OBSERVER_RATIO_H

#include "observer_base.h"


typedef struct _observer_ratio_t observer_ratio_t;

extern observer_ratio_t *observer_ratio_new(
    engine_reg_t reg,
    observer_cb_t, void *ctxt,
    int normal_min, int normal_max
);

/* Gets the raw value as a ratio of the max (with min normalised to 0).
 * Range is self-calibrating based on the range of observed values */
extern void observer_ratio_get_ratio( observer_base_t *obs, float *ratio );

#endif /* defined _INCLUDED_OBSERVER_RATIO_H */
