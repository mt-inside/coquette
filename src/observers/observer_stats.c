#include "common.h"

#include <stdlib.h>
#include <assert.h>
#include <limits.h>
#include <math.h>

#include "observer_base.h"
#include "observer_internal.h"
#include "observer_stats.h"


struct _observer_stats_t
{
    observer_base_t base;
    unsigned period;
    int *values;
    unsigned values_size;
    unsigned values_count;
};

static void observer_stats_update( observer_base_t *this, int first_time );
static void observer_stats_delete( observer_base_t *this );

observer_stats_t *observer_stats_new( engine_reg_t reg,
                                      observer_cb_t cb, void *ctxt,
                                      unsigned period )
{
    observer_stats_t *this = calloc( sizeof( observer_stats_t ), 1 );

    observer_base_init( (observer_base_t *)this, observer_subclass_STATS, &observer_stats_update, &observer_stats_delete, cb, ctxt, reg );

    this->period = period;

    return this;
}

static void observer_stats_update( observer_base_t *obs, int first_time )
{
    const unsigned values_resize_quantum = 64;
    observer_stats_t *this = (observer_stats_t *)obs;

    NOT_USED(first_time);

    assert( obs->class == observer_subclass_STATS );

    if( this->values_count == this->values_size )
    {
        this->values_size += values_resize_quantum;
        this->values = realloc( this->values, this->values_size * sizeof( int ) );
    }
    this->values[ this->values_count++ ] = obs->value;

    /* TODO: For now, assume we always have new stats available */
    obs->cb( obs, obs->ctxt );
}

static void observer_stats_delete( observer_base_t *obs )
{
    observer_stats_t *this = (observer_stats_t *)obs;
    assert( obs->class == observer_subclass_STATS );

    free( this->values );
}

/* TODO: move to an on-line algorithm as it'll be faster, use less space and be
 * easier to spot when the stats have changed. */
static void get_stats_for_range( int *start, unsigned count,
                                 int *min_out, int *max_out,
                                 int *mean_out, int *stdev_out )
{
    unsigned i;
    int min = INT_MAX, max = INT_MIN, sum = 0, sum_sqr = 0;
    float mean, variance;

    for( i = 0; i < count; ++i )
    {
        if( start[i] < min ) min = start[i];
        if( start[i] > max ) max = start[i];
        sum += start[i];
        sum_sqr += start[i] * start[i];
    }
    mean = (float)sum / count;
    variance = ((float)sum_sqr - (float)sum * mean) / count;

    *min_out = min; *max_out = max;
    *mean_out = (int)round( mean ); *stdev_out = (int)round( sqrt( variance ) );
}

void observer_stats_get_stats( observer_base_t *obs,
                               int *min, int *max, int *mean, int *stdev )
{
    observer_stats_t *this = (observer_stats_t *)obs;
    unsigned period;

    assert( obs->class == observer_subclass_STATS );

    period = this->period;
    if( period == 0 ) period = this->values_count;
    if( period > this->values_count ) period = this->values_count;

    get_stats_for_range( this->values + (this->values_count - period),
                         period,
                         min, max, mean, stdev );

    /* TODO: calc higher-order moments */
}
