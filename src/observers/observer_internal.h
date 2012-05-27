/* Header file to include if you want to subclass observer_base_t */

#ifndef _INCLUDED_OBSERVER_INTERNAL_H
#define _INCLUDED_OBSERVER_INTERNAL_H

#include "observer_base.h"
#include "registers.h"


typedef enum
{
    observer_subclass_FLAG,
    observer_subclass_DERIVATIVE,
    observer_subclass_INTEGRAL,
    observer_subclass_RATIO,
    observer_subclass_SHIFT,
    observer_subclass_STATS,
    observer_subclass_VALUE,
    observer_subclass_ZEROSIXTY
} observer_subclass_t;

/* "Virtual" function types. These are supplied to the base class by subclasses
 * and are called by the corresponding methods on the base class, like a shite
 * virtual dispatch */
typedef void (*observer_update_fn_t)( struct _observer_base_t *obs, int first_time );
typedef void (*observer_delete_fn_t)( struct _observer_base_t *obs );


struct _observer_base_t
{
    observer_subclass_t class;
    observer_update_fn_t update_fn;
    observer_delete_fn_t delete_fn;
    observer_cb_t cb;
    void *ctxt;
    engine_reg_t reg;
    int value;
    int first_time;
};


extern void observer_base_init(
    observer_base_t *obs,
    observer_subclass_t class,
    observer_update_fn_t update_fn,
    observer_delete_fn_t delete_fn,
    observer_cb_t cb,
    void *ctxt,
    engine_reg_t reg
);

#endif /* defined _INCLUDED_OBSERVER_INTERNAL_H */
