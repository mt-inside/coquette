#include <stdlib.h>

#include "observer_internal.h"


void observer_init( observer_t *this, observer_subclass_t class )
{
    this->class = class;
}

void observer_update( observer_t *this, int value )
{
    this->update_fn( this, value );
}

int observer_get_value( observer_t *this )
{
    return this->value;
}
