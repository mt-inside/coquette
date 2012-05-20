/* This is a super-accurate integration class.
 * It takes every sample, as fast as they come, and sums them with micro-second
 * accuracy. This results in a lot of 64bit multiplication and division, very
 * often. If this proves too slow, consider building one that samples every
 * second or so */

#ifndef _INCLUDED_OBSERVER_INTEGRAL_H
#define _INCLUDED_OBSERVER_INTEGRAL_H

#include "observer_base.h"


typedef struct _observer_integral_t observer_integral_t;

extern observer_integral_t *observer_integral_new(
    engine_reg_t reg,
    observer_cb_t, void *ctxt
);

/* The value has been integrated wrt seconds. Thus if the original unit was per
 * hour, you need to adjust this value by / 3600 */
void observer_integral_get_integral( observer_base_t *obs, int64_t *integral );

#endif /* defined _INCLUDED_OBSERVER_INTEGRAL_H */
