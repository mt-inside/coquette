/* (Numerically) differentiating observer.
 * While the integral observer can gaily use every sample to be super-accurate,
 * it would be silly for this one to do so. Because of the quantised nature of
 * the values you'd either get lots of 0s with the odd high rate-of-change for
 * slow-changing values or a rate-of-change that fluctuated so rapidly so as to
 * not be useful.
 * Either way, for display you'd end up averaging both of them out, so you may
 * as well sample less often
 * Note: You may want to graph a rate-of-change (e.g. acceleration), in which
 * case frequent updates are desired. Just change this to accept ms periods - if
 * the period is shorter that the sample period then you'll simply get called
 * for every datum
 */
#ifndef _INCLUDED_OBSERVER_DERIVATIVE_H
#define _INCLUDED_OBSERVER_DERIVATIVE_H

#include "observer_base.h"


typedef struct _observer_derivative_t observer_derivative_t;

extern observer_derivative_t *observer_derivative_new(
    engine_reg_t reg,
    observer_cb_t, void *ctxt,
    unsigned period /* seconds */
);
extern void observer_derivative_get_derivative( observer_base_t *obs, float *derivative );

#endif /* defined _INCLUDED_OBSERVER_DERIVATIVE_H */
