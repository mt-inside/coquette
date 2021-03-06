#ifndef _INCLUDED_OBSERVER_ZEROSIXTY_H
#define _INCLUDED_OBSERVER_ZEROSIXTY_H

#include "observer_base.h"


typedef struct _observer_zerosixty_t observer_zerosixty_t;

extern observer_zerosixty_t *observer_zerosixty_new(
    engine_reg_t reg,
    observer_cb_t, void *ctxt,
    unsigned target
);

/* gets the last 0-60 time in seconds */
extern void observer_zerosixty_get_zerosixty( observer_base_t *obs, float *zerosixty );

#endif /* defined _INCLUDED_OBSERVER_ZEROSIXTY_H */
