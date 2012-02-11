#ifndef _INCLUDED_OBSERVER_INTERNAL_H
#define _INCLUDED_OBSERVER_INTERNAL_H

#include "observer.h"


typedef enum
{
    observer_subclass_STATS
} observer_subclass_t;


typedef void (*observer_update_fn_t)( struct _observer_t *this );


struct _observer_t
{
    observer_subclass_t class;
    observer_update_fn_t update_fn;
    observer_cb_t cb;
    void *ctxt;
    int value;
};


extern void observer_init(
    observer_t *this,
    observer_subclass_t class,
    observer_update_fn_t update_fn,
    observer_cb_t cb,
    void *ctxt
);

#endif /* defined _INCLUDED_OBSERVER_INTERNAL_H */
