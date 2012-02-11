#include <stdlib.h>
#include <assert.h>
#include <limits.h>
#include <math.h>

#include "stats_observer.h"
#include "observer_internal.h"
#include "observer.h"


struct _stats_observer_t
{
    observer_t base;
    int *values;
    unsigned values_size;
    unsigned values_count;
};

static void stats_observer_update( observer_t *this );

stats_observer_t *stats_observer_new( observer_cb_t cb, void *ctxt )
{
    stats_observer_t *obs = calloc( sizeof( stats_observer_t ), 1 );

    observer_init( (observer_t *)obs, observer_subclass_STATS, &stats_observer_update, cb, ctxt );

    return obs;
}

static void stats_observer_update( observer_t *this )
{
    const unsigned values_resize_quantum = 64;
    stats_observer_t *obs = (stats_observer_t *)this;

    assert( obs->base.class == observer_subclass_STATS );

    if( obs->values_count == obs->values_size )
    {
        obs->values_size += values_resize_quantum;
        obs->values = realloc( obs->values, obs->values_size * sizeof( int ) );
    }
    obs->values[ obs->values_count++ ] = this->value;

    /* For now, assume we always have new stats available */
    this->cb( this, this->ctxt );
}

void get_stats_for_range( int *start, unsigned count,
                          int *min_out, int *max_out,
                          int *mean_out, int *stdev_out )
{
    unsigned i;
    int min = INT_MAX, max = INT_MIN, sum = 0, sum_sqr = 0, mean, variance;

    for( i = 0; i < count; ++i )
    {
        if( start[i] < min ) min = start[i];
        if( start[i] > max ) max = start[i];
        sum += start[i];
        sum_sqr += start[i] * start[i];
    }
    mean = sum / count;
    variance = (sum_sqr - sum * mean) / count;

    *min_out = min; *max_out = max;
    *mean_out = mean; *stdev_out = (int)round( sqrt( variance ) );
}

void stats_observer_get_stats( observer_t *this,
                               unsigned period,
                               int *min, int *max, int *mean, int *stdev )
{
    stats_observer_t *obs;

    assert( this->class == observer_subclass_STATS );
    obs = (stats_observer_t *)this;

    if( period == 0 ) period = obs->values_count;
    if( period > obs->values_count ) period = obs->values_count;

    get_stats_for_range( obs->values + (obs->values_count - period),
                         period,
                         min, max, mean, stdev );

    /* TODO: calc higher-order moments */
}
