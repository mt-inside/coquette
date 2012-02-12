#include <stdlib.h>
#include <assert.h>
#include <sys/time.h>

#include "observer_zerosixty.h"
#include "observer_internal.h"
#include "observer.h"
#include "utils.h"


typedef enum
{
    zerosixty_state_WAITING_ZERO,
    zerosixty_state_WAITING_NONZERO,
    zerosixty_state_TIMING
} zerosixty_state_t;

struct _observer_zerosixty_t
{
    observer_t base;
    zerosixty_state_t state;
    struct timeval *start_time;
    unsigned target;
    unsigned zerosixty;     /* microseconds */
    unsigned zerosixty_old; /* microseconds */
};

static void observer_zerosixty_update( observer_t *this );

observer_zerosixty_t *observer_zerosixty_new( observer_cb_t cb, void *ctxt, unsigned target )
{
    observer_zerosixty_t *this = calloc( sizeof( observer_zerosixty_t ), 1 );

    observer_init( (observer_t *)this, observer_subclass_ZEROSIXTY, &observer_zerosixty_update, cb, ctxt );

    this->state = zerosixty_state_WAITING_ZERO;
    this->start_time = malloc( sizeof(struct timeval) );
    this->target = target;
    this->zerosixty = 0;
    this->zerosixty_old = 0;

    return this;
}

static void observer_zerosixty_update( observer_t *obs )
{
    observer_zerosixty_t *this = (observer_zerosixty_t *)obs;

    assert( obs->class == observer_subclass_ZEROSIXTY );


    switch( this->state )
    {
        case zerosixty_state_WAITING_ZERO:
            if( obs->value == 0 ) this->state = zerosixty_state_WAITING_NONZERO;
            break;

        case zerosixty_state_WAITING_NONZERO:
            if( obs->value != 0 )
            {
                gettimeofday( this->start_time, NULL );
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
                timersub( &tv_now, this->start_time, &tv_diff );
                this->zerosixty = (tv_diff.tv_sec * (10 << 6)) + tv_diff.tv_usec;

                this->state = zerosixty_state_WAITING_ZERO;
            }
            break;

        default:
            assert( 0 );
    }


    if( this->zerosixty != this->zerosixty_old )
    {
        obs->cb( obs, obs->ctxt );
    }

    this->zerosixty_old = this->zerosixty;
}

void obsserver_zerosixty_get_zerosixty( observer_t *obs, unsigned *zerosixty )
{
    observer_zerosixty_t *this = (observer_zerosixty_t *)obs;

    assert( obs->class == observer_subclass_ZEROSIXTY );

    *zerosixty = this->zerosixty;
}
