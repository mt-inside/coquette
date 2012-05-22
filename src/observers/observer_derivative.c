#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <sys/time.h>

#include "observer_base.h"
#include "observer_internal.h"
#include "observer_derivative.h"
#include "utils.h"


struct _observer_derivative_t
{
    observer_base_t base;
    int value_old;
    struct timeval start_time;
    float derivative;
    float derivative_old;
    unsigned period;
};

static void observer_derivative_update( observer_base_t *this, int first_time );

observer_derivative_t *observer_derivative_new(
    engine_reg_t reg,
    observer_cb_t cb, void *ctxt,
    unsigned period
)
{
    observer_derivative_t *this = calloc( sizeof( observer_derivative_t ), 1 );

    observer_base_init( (observer_base_t *)this, observer_subclass_DERIVATIVE, &observer_derivative_update, NULL, cb, ctxt, reg );

    this->period = period;

    return this;
}

static void observer_derivative_update( observer_base_t *obs, int first_time )
{
    observer_derivative_t *this = (observer_derivative_t *)obs;

    assert( obs->class == observer_subclass_DERIVATIVE );

    if( first_time )
    {
        this->value_old = obs->value;
        gettimeofday( &this->start_time, NULL );
    }
    else
    {
        struct timeval tv_now, tv_diff;
        gettimeofday( &tv_now, NULL );
        timersub( &tv_now, &this->start_time, &tv_diff );

        if( tv_diff.tv_sec >= this->period )
        {
            this->derivative = (float)(obs->value - this->value_old) / (float)this->period;

            this->value_old = obs->value;
            gettimeofday( &this->start_time, NULL );

            if( this->derivative != this->derivative_old )
            {
                obs->cb( obs, obs->ctxt );
                this->derivative_old = this->derivative;
            }
        }
    }
}

void observer_derivative_get_derivative( observer_base_t *obs, float *derivative )
{
    observer_derivative_t *this = (observer_derivative_t *)obs;

    assert( obs->class == observer_subclass_DERIVATIVE );

    *derivative = this->derivative;
}
