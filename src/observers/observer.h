#ifndef _INCLUDED_OBSERVER_H
#define _INCLUDED_OBSERVER_H

typedef struct _observer_t observer_t;

/* needed by derived classes' new()s */
typedef void (*observer_cb_t)( observer_t *obs, void *ctxt );

extern void observer_update( observer_t *this, int value );
extern int observer_get_value( observer_t *this ); /* all observers support getting their current value */

#endif /* defined _INCLUDED_OBSERVER_H */
