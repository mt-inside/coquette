#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <stdarg.h>

#include "observer_base.h"
#include "observer_internal.h"
#include "observer_flag.h"
#include "utils.h"


struct _observer_flag_t
{
    observer_base_t base;
    engine_flag_t flag;
    /* base->value holds the whole flags register; the individual flag is
     * extracted and stored here */
    int flag_value;
    int flag_value_old;
};

static void observer_flag_update( observer_base_t *this, int first_time );

observer_flag_t *observer_flag_new(
    engine_flag_t flag,
    observer_cb_t cb, void *ctxt
)
{
    engine_reg_t reg;
    unsigned offset;


    engine_flag_to_ordinates( flag, &reg, &offset );

    observer_flag_t *this = calloc( sizeof( observer_flag_t ), 1 );

    observer_base_init( (observer_base_t *)this, observer_subclass_FLAG, &observer_flag_update, NULL, cb, ctxt, reg );

    this->flag = flag;

    return this;
}

static void observer_flag_update( observer_base_t *obs, int first_time )
{
    engine_reg_t reg; unsigned offset;
    observer_flag_t *this = (observer_flag_t *)obs;

    assert( obs->class == observer_subclass_FLAG );

    engine_flag_to_ordinates( this->flag, &reg, &offset );

    this->flag_value = ( obs->value & ( 1 << offset ) ) >> offset;

    if( this->flag_value != this->flag_value_old || first_time )
    {
        obs->cb( obs, obs->ctxt );
    }

    this->flag_value_old = this->flag_value;
}

void observer_flag_get_flag(
    observer_base_t *obs,
    int *flag_value
)
{
    observer_flag_t *this = (observer_flag_t *)obs;

    assert( obs->class == observer_subclass_FLAG );

    *flag_value = this->flag_value;
}
