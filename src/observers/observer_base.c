#include <stdlib.h>

#include "observer_base.h"
#include "observer_internal.h"


void observer_base_init(
    observer_base_t *this,
    observer_subclass_t class,
    observer_update_fn_t update_fn,
    observer_delete_fn_t delete_fn,
    observer_cb_t cb,
    void *ctxt,
    engine_reg_t reg
)
{
    this->class = class;

    this->update_fn = update_fn;
    this->delete_fn = delete_fn;

    this->cb = cb;
    this->ctxt = ctxt;

    this->reg = reg;

    this->first_time = 1;
}

void observer_base_delete( observer_base_t *this )
{
    if( this->delete_fn ) this->delete_fn( this );
    free( this );
}

void observer_base_update( observer_base_t *this, int value )
{
    this->value = value;

    this->update_fn( this, this->first_time );
    if( this->first_time ) this->first_time = 0;
}

engine_reg_t observer_base_get_reg( observer_base_t *this )
{
    return this->reg;
}

int observer_base_get_value( observer_base_t *this )
{
    return this->value;
}
