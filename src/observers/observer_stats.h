/* This class could take the period as an argument to get_stats() but that
 * implies the implementation. It's also neater to have one observer per time
 * window one is interested in. Not least because the subscriber will only get
 * called when the values have changed, and this may be different for different
 * window lengths.
 */
#ifndef _INCLUDED_OBSERVER_STATS_H
#define _INCLUDED_OBSERVER_STATS_H

#include "observer_base.h"


typedef struct _observer_stats_t observer_stats_t;

/* 0 period means forever */
extern observer_stats_t *observer_stats_new( observer_cb_t cb, void *ctxt,
                                             unsigned period );

extern void observer_stats_get_stats( observer_base_t *obs,
                                      int *min, int *max, int *mean, int *stdev );

#endif /* defined _INCLUDED_OBSERVER_STATS_H */
