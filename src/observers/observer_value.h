#ifndef _INCLUDED_OBSERVER_VALUE_H
#define _INCLUDED_OBSERVER_VALUE_H

#include "observer.h"


typedef struct _observer_value_t observer_value_t;

extern observer_value_t *observer_value_new(
    observer_cb_t, void *ctxt
);

extern void observer_value_get_value( observer_t *obs, int *value );

#endif /* defined _INCLUDED_OBSERVER_VALUE_H */
