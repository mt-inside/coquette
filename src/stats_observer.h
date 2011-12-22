#ifndef _INCLUDED_STATS_OBSERVER_H
#define _INCLUDED_STATS_OBSERVER_H

#include "observer.h"


typedef struct _stats_observer_t stats_observer_t;

extern stats_observer_t *stats_observer_new( );

/* 0 period means forever */
extern void stats_observer_get_stats( observer_t *this,
                                      unsigned period,
                                      int *min, int *max, int *mean, int *stdev );

#endif /* defined _INCLUDED_STATS_OBSERVER_H */
