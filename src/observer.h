#ifndef _INCLUDED_OBSERVER_H
#define _INCLUDED_OBSERVER_H

typedef struct _observer_t observer_t;

typedef enum
{
    observer_subclass_STATS
} observer_subclass_t;

extern void observer_init( observer_t *this, observer_subclass_t type );
extern void observer_update( observer_t *this, int value );
extern int observer_get_value( observer_t *this );

#endif /* defined _INCLUDED_OBSERVER_H */
