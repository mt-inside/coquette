#include <stdlib.h>
#include <assert.h>
#include <math.h>

#include "observer_base.h"
#include "observer_internal.h"
#include "observer_shift.h"
#include "utils.h"


struct _observer_shift_t
{
    observer_base_t base;
    unsigned warn_start;
    unsigned warn_end;
    unsigned warn_level;
    unsigned warn_level_old;
};

static void observer_shift_update( observer_base_t *this );

observer_shift_t *observer_shift_new(
    observer_cb_t cb, void *ctxt,
    unsigned warn_start, unsigned warn_end
)
{
    observer_shift_t *this = calloc( sizeof( observer_shift_t ), 1 );

    observer_base_init( (observer_base_t *)this, observer_subclass_SHIFT, &observer_shift_update, cb, ctxt );

    this->warn_start = warn_start;
    this->warn_end   = warn_end;

    this->warn_level = 0;
    this->warn_level_old = 0;

    return this;
}

static void observer_shift_update( observer_base_t *obs )
{
    observer_shift_t *this = (observer_shift_t *)obs;
    int w;

    assert( obs->class == observer_subclass_SHIFT );

    /* mmm, branch free... */
    w = (int)ceil((double)((signed)obs->value - (signed)this->warn_start) / (((signed)this->warn_end - (signed)this->warn_start) / 3));
    w = MAX( w, 0 );
    w = MIN( w, 4 );

    this->warn_level = w;

    if( this->warn_level != this->warn_level_old )
    {
        obs->cb( obs, obs->ctxt );
    }

    this->warn_level_old = this->warn_level;
}

void observer_shift_get_shift( observer_base_t *obs, unsigned *warn_level )
{
    observer_shift_t *this = (observer_shift_t *)obs;

    assert( obs->class == observer_subclass_SHIFT );

    *warn_level = this->warn_level;
}
