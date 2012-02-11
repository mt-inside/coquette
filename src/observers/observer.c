#include <stdlib.h>

#include "observer_internal.h"


void observer_init(
    observer_t *this,
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

void observer_update( observer_t *this, int value )
{
    this->value = value;

    this->update_fn( this );
}

int observer_get_value( observer_t *this )
{
    return this->value;
}
