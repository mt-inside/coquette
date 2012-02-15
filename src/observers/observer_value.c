#include <stdlib.h>
#include <assert.h>
#include <math.h>

#include "observer_value.h"
#include "observer_internal.h"
#include "observer.h"
#include "utils.h"


struct _observer_value_t
{
    observer_t base;
    int value_old;
};

static void observer_value_update( observer_t *this );

observer_value_t *observer_value_new(
    observer_cb_t cb, void *ctxt
)
{
    observer_value_t *this = calloc( sizeof( observer_value_t ), 1 );

    observer_init( (observer_t *)this, observer_subclass_VALUE, &observer_value_update, cb, ctxt );

    this->value_old = 0;

    return this;
}

static void observer_value_update( observer_t *obs )
{
    observer_value_t *this = (observer_value_t *)obs;

    assert( obs->class == observer_subclass_VALUE );

    if( obs->value != this->value_old )
    {
        obs->cb( obs, obs->ctxt );
    }

    this->value_old = obs->value;
}

void observer_value_get_value( observer_t *obs, int *value )
{
    assert( obs->class == observer_subclass_VALUE );

    *value = obs->value;
}
