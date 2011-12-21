#ifndef _INCLUDED_OBSERVER_INTERNAL_H
#define _INCLUDED_OBSERVER_INTERNAL_H

#include "observer.h"


struct _observer_t
{
    observer_subclass_t class;
    void (*update_fn)( struct _observer_t *this, int value ); /* move me off this struct to a normal function that takes a observer_t */
    int value;
};

#endif /* defined _INCLUDED_OBSERVER_INTERNAL_H */
