#ifndef _INCLUDED_OBSERVER_STATS_H
#define _INCLUDED_OBSERVER_STATS_H

#include "observer_base.h"


typedef struct _observer_stats_t observer_stats_t;

extern observer_stats_t *observer_stats_new( observer_cb_t cb, void *ctxt );

/* 0 period means forever */
extern void observer_stats_get_stats( observer_base_t *obs,
                                      unsigned period,
                                      int *min, int *max, int *mean, int *stdev );

#endif /* defined _INCLUDED_OBSERVER_STATS_H */
