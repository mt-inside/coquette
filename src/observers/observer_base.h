#ifndef _INCLUDED_OBSERVER_BASE_H
#define _INCLUDED_OBSERVER_BASE_H

typedef struct _observer_base_t observer_base_t;

/* needed by derived classes' new()s */
typedef void (*observer_cb_t)( observer_base_t *obs, void *ctxt );

extern void observer_base_update( observer_base_t *obs, int value );
extern int observer_base_get_value( observer_base_t *obs ); /* all observers support getting their current value */

#endif /* defined _INCLUDED_OBSERVER_BASE_H */
