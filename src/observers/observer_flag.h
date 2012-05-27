#ifndef _INCLUDED_OBSERVER_FLAG_H
#define _INCLUDED_OBSERVER_FLAG_H

#include "observer_base.h"


typedef struct _observer_flag_t observer_flag_t;

extern observer_flag_t *observer_flag_new(
    engine_flag_t flag,
    observer_cb_t, void *ctxt
);

extern void observer_flag_get_flag(
    observer_base_t *obs,
    int *value
);

#endif /* defined _INCLUDED_OBSERVER_FLAG_H */
