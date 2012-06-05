#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <limits.h>

#include "observer_base.h"
#include "observer_internal.h"
#include "observer_ratio.h"
#include "utils.h"


struct _observer_ratio_t
{
    observer_base_t base;
    int value_old;
    int min;
    int max;
    int calibrate_min;
    int calibrate_max;
};

static void observer_ratio_update( observer_base_t *this, int first_time );

observer_ratio_t *observer_ratio_new(
    engine_reg_t reg,
    observer_cb_t cb, void *ctxt,
    int normal_min, int normal_max
)
{
    observer_ratio_t *this = calloc( sizeof( observer_ratio_t ), 1 );

    observer_base_init( (observer_base_t *)this, observer_subclass_RATIO, &observer_ratio_update, NULL, cb, ctxt, reg );

    if( normal_min == INT_MAX )
        this->calibrate_min = 1;
    else
        this->min = normal_min;

    if( normal_max == INT_MIN )
        this->calibrate_max = 1;
    else
        this->max = normal_max;

    return this;
}

static void observer_ratio_update( observer_base_t *obs, int first_time )
{
    observer_ratio_t *this = (observer_ratio_t *)obs;

    assert( obs->class == observer_subclass_RATIO );

    if( first_time )
    {
        /* it is a subtlety that value_old is assigned to here.
         * Morally it shouldn't be. Thus we'd enter the next block and the
         * event would be raised. However on callback there would be a division
         * by 0. This way, the event won't be raised until max != min. */
        this->value_old = obs->value;
        if( this->calibrate_min ) this->min = obs->value;
        if( this->calibrate_max ) this->max = obs->value;

        /* If at least one limit is manually set, and the other doesn't equal
         * it, then the ratio is defined; raise event. We won't double-raise
         * because min==max. */
        if( !this->calibrate_min || !this->calibrate_max && this->min != this->max )
        {
            obs->cb( obs, obs->ctxt );
        }
    }

    /* Ratio makes no sense if there's only been one data value (it's undefined
     * because max==min, so don't raise event. */
    if( obs->value != this->value_old )
    {
        if     ( this->calibrate_min && obs->value < this->min ) this->min = obs->value;
        else if( this->calibrate_max && obs->value > this->max ) this->max = obs->value;

        obs->cb( obs, obs->ctxt );
    }

    this->value_old = obs->value;
}

void observer_ratio_get_ratio( observer_base_t *obs, float *ratio )
{
    observer_ratio_t *this = (observer_ratio_t *)obs;

    assert( obs->class == observer_subclass_RATIO );

    *ratio = (float)(obs->value - this->min) / (this->max - this->min);
}
