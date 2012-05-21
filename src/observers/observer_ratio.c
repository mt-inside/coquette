#include <stdlib.h>
#include <assert.h>
#include <math.h>

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
};

static void observer_ratio_update( observer_base_t *this, int first_time );

observer_ratio_t *observer_ratio_new(
    engine_reg_t reg,
    observer_cb_t cb, void *ctxt
)
{
    observer_ratio_t *this = calloc( sizeof( observer_ratio_t ), 1 );

    observer_base_init( (observer_base_t *)this, observer_subclass_RATIO, &observer_ratio_update, NULL, cb, ctxt, reg );

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
        this->min = this->max = this->value_old = obs->value;
    }

    /* Ratio makes no sense if there is only one data point (it's undefined
     * because max==min, so don't raise this first time */
    if( obs->value != this->value_old )
    {
        if     ( obs->value < this->min ) this->min = obs->value;
        else if( obs->value > this->max ) this->max = obs->value;

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
