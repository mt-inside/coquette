#include <stdlib.h>
#include <assert.h>
#include <math.h>

#include "observer_ratio.h"
#include "observer_internal.h"
#include "observer.h"
#include "utils.h"


struct _observer_ratio_t
{
    observer_t base;
    int value_old;
    int first_time;
    int min;
    int max;
};

static void observer_ratio_update( observer_t *this );

observer_ratio_t *observer_ratio_new(
    observer_cb_t cb, void *ctxt
)
{
    observer_ratio_t *this = calloc( sizeof( observer_ratio_t ), 1 );

    observer_init( (observer_t *)this, observer_subclass_RATIO, &observer_ratio_update, cb, ctxt );

    this->first_time = 1;

    return this;
}

static void observer_ratio_update( observer_t *obs )
{
    observer_ratio_t *this = (observer_ratio_t *)obs;

    assert( obs->class == observer_subclass_RATIO );

    if( this->first_time )
    {
        this->min = this->max = this->value_old = obs->value;
        this->first_time = 0;
    }

    if( obs->value != this->value_old )
    {
        if     ( obs->value < this->min ) this->min = obs->value;
        else if( obs->value > this->max ) this->max = obs->value;

        obs->cb( obs, obs->ctxt );
    }

    this->value_old = obs->value;
}

void observer_ratio_get_ratio( observer_t *obs, float *ratio )
{
    observer_ratio_t *this = (observer_ratio_t *)obs;

    assert( obs->class == observer_subclass_RATIO );

    *ratio = (float)(obs->value - this->min) / (this->max - this->min);
}
