#ifndef _INCLUDED_STATS_OBSERVER_H
#define _INCLUDED_STATS_OBSERVER_H

typedef struct _stats_observer_t stats_observer_t;

extern stats_observer_t *stats_observer_new( );
extern int stats_observer_get_min( stats_observer_t *obs );
extern int stats_observer_get_max( stats_observer_t *obs );
extern int stats_observer_get_mean( stats_observer_t *obs );

#endif /* defined _INCLUDED_STATS_OBSERVER_H */
