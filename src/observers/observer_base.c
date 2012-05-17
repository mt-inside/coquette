#include <stdlib.h>

#include "observer_base.h"
#include "observer_internal.h"


void observer_base_init(
    observer_base_t *this,
    observer_subclass_t class,
    observer_update_fn_t update_fn,
    observer_cb_t cb,
    void *ctxt
)
{
    this->class = class;

    this->update_fn = update_fn;

    this->cb = cb;
    this->ctxt = ctxt;
}

void observer_base_update( observer_base_t *this, int value )
{
    this->value = value;

    this->update_fn( this );
}

int observer_base_get_value( observer_base_t *this )
{
    return this->value;
}
