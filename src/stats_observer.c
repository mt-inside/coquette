#include <stdlib.h>
#include <assert.h>

#include "stats_observer.h"
#include "observer_internal.h"
#include "observer.h"


struct _stats_observer_t
{
    observer_t base;
    int min;
    int max;
    int mean;
};

static void stats_observer_update( observer_t *this, int value );

stats_observer_t *stats_observer_new( )
{
    stats_observer_t *obs = calloc( sizeof( stats_observer_t ), 1 );

    observer_init( (observer_t *)obs, observer_subclass_STATS );
    obs->base.update_fn = &stats_observer_update;

    return obs;
}

static void stats_observer_update( observer_t *this, int value )
{
    stats_observer_t *obs = (stats_observer_t *)this;

    assert( obs->base.class = observer_subclass_STATS );

    this->value = value;
    if( value < obs->min ) obs->min = value;
    if( value < obs->max ) obs->max = value;
    obs->mean = value; /* FIXME */
}

int stats_observer_get_min( stats_observer_t *obs )
{
    assert( obs->base.class = observer_subclass_STATS );

    return obs->min;
}
int stats_observer_get_max( stats_observer_t *obs )
{
    assert( obs->base.class = observer_subclass_STATS );

    return obs->max;
}
int stats_observer_get_mean( stats_observer_t *obs )
{
    assert( obs->base.class = observer_subclass_STATS );

    return obs->mean;
}
