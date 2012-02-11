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
    stats_observer_t *this = calloc( sizeof( stats_observer_t ), 1 );

    observer_init( (observer_t *)this, observer_subclass_STATS, &stats_observer_update, cb, ctxt );

    return this;
}

static void stats_observer_update( observer_t *obs )
{
    const unsigned values_resize_quantum = 64;
    stats_observer_t *this = (stats_observer_t *)obs;

    assert( obs->class == observer_subclass_STATS );

    if( this->values_count == this->values_size )
    {
        this->values_size += values_resize_quantum;
        this->values = realloc( this->values, this->values_size * sizeof( int ) );
    }
    this->values[ this->values_count++ ] = obs->value;

    /* For now, assume we always have new stats available */
    obs->cb( obs, obs->ctxt );
}

static void get_stats_for_range( int *start, unsigned count,
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

void stats_observer_get_stats( observer_t *obs,
                               unsigned period,
                               int *min, int *max, int *mean, int *stdev )
{
    stats_observer_t *this = (stats_observer_t *)obs;

    assert( obs->class == observer_subclass_STATS );

    if( period == 0 ) period = this->values_count;
    if( period > this->values_count ) period = this->values_count;

    get_stats_for_range( this->values + (this->values_count - period),
                         period,
                         min, max, mean, stdev );

    /* TODO: calc higher-order moments */
}
