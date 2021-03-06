#include "common.h"

#include <stdlib.h>
#include <assert.h>
#include <sys/time.h>

#include "observer_base.h"
#include "observer_internal.h"
#include "observer_zerosixty.h"
#include "utils.h"


typedef enum
{
    zerosixty_state_WAITING_ZERO,
    zerosixty_state_WAITING_NONZERO,
    zerosixty_state_TIMING
} zerosixty_state_t;

struct _observer_zerosixty_t
{
    observer_base_t base;
    zerosixty_state_t state;
    struct timeval start_time;
    unsigned target;
    unsigned zerosixty;     /* microseconds */
    unsigned zerosixty_old; /* microseconds */
};

static void observer_zerosixty_update( observer_base_t *this, int first_time );

observer_zerosixty_t *observer_zerosixty_new(
    engine_reg_t reg,
    observer_cb_t cb, void *ctxt,
    unsigned target
)
{
    observer_zerosixty_t *this = calloc( sizeof( observer_zerosixty_t ), 1 );

    observer_base_init( (observer_base_t *)this, observer_subclass_ZEROSIXTY, &observer_zerosixty_update, NULL, cb, ctxt, reg );

    this->state = zerosixty_state_WAITING_ZERO;
    this->target = target;

    return this;
}

static void observer_zerosixty_update( observer_base_t *obs, int first_time )
{
    observer_zerosixty_t *this = (observer_zerosixty_t *)obs;

    assert( obs->class == observer_subclass_ZEROSIXTY );
    NOT_USED(first_time);


    switch( this->state )
    {
        case zerosixty_state_WAITING_ZERO:
            if( obs->value == 0 ) this->state = zerosixty_state_WAITING_NONZERO;
            break;

        case zerosixty_state_WAITING_NONZERO:
            if( obs->value != 0 )
            {
                gettimeofday( &this->start_time, NULL );
                this->state = zerosixty_state_TIMING;
            }
            break;

        case zerosixty_state_TIMING:
            if( obs->value == 0 )
            {
                this->state = zerosixty_state_WAITING_NONZERO;
            }
            else if( obs->value >= (int)this->target )
            {
                struct timeval tv_now, tv_diff;
                gettimeofday( &tv_now, NULL );
                timersub( &tv_now, &this->start_time, &tv_diff );
                this->zerosixty = timeval_to_usec( &tv_diff );

                this->state = zerosixty_state_WAITING_ZERO;
            }
            break;

        default:
            assert( 0 );
    }


    /* Zero-sixty makes no sense from just one datum as the value can't have
     * changed, so don't raise this event first time */
    if( this->zerosixty != this->zerosixty_old )
    {
        obs->cb( obs, obs->ctxt );
    }

    this->zerosixty_old = this->zerosixty;
}

void observer_zerosixty_get_zerosixty( observer_base_t *obs, float *zerosixty )
{
    observer_zerosixty_t *this = (observer_zerosixty_t *)obs;

    assert( obs->class == observer_subclass_ZEROSIXTY );

    *zerosixty = (float)this->zerosixty / 1000000.0f;
}
