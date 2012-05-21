#include "common.h"

#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <sys/time.h>

#include "observer_base.h"
#include "observer_internal.h"
#include "observer_integral.h"
#include "utils.h"


struct _observer_integral_t
{
    observer_base_t base;
    int value_old;
    struct timeval time_old;
    /* For an original unit of $unit/$time, these values are in:
     * $unit * 10^6 * $time in seconds
     * e.g. for kmh, the integral is in km * 3.6 * 10^9 */
    int64_t integral;
    int64_t integral_old;
};

static void observer_integral_update( observer_base_t *this, int first_time );

observer_integral_t *observer_integral_new(
    engine_reg_t reg,
    observer_cb_t cb, void *ctxt
)
{
    observer_integral_t *this = calloc( sizeof( observer_integral_t ), 1 );

    observer_base_init( (observer_base_t *)this, observer_subclass_INTEGRAL, &observer_integral_update, cb, ctxt, reg );

    this->integral = 0;

    return this;
}

static void observer_integral_update( observer_base_t *obs, int first_time )
{
    observer_integral_t *this = (observer_integral_t *)obs;

    assert( obs->class == observer_subclass_INTEGRAL );

    struct timeval tv_now, tv_diff;
    gettimeofday( &tv_now, NULL );

    if( !first_time )
    {
        timersub( &tv_now, &this->time_old, &tv_diff );
        this->integral += (int64_t)((this->value_old + obs->value) / 2) * (int64_t)timeval_to_usec( &tv_diff );
    }

    if( this->integral != this->integral_old || first_time )
    {
        obs->cb( obs, obs->ctxt );
    }

    this->integral_old = this->integral;
    this->time_old = tv_now;
    this->value_old = obs->value;
}

void observer_integral_get_integral( observer_base_t *obs, int64_t *integral )
{
    observer_integral_t *this = (observer_integral_t *)obs;

    assert( obs->class == observer_subclass_INTEGRAL );

    *integral = this->integral / 1000000;
}
