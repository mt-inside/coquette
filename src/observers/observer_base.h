#ifndef _INCLUDED_OBSERVER_BASE_H
#define _INCLUDED_OBSERVER_BASE_H

#include "registers.h"


typedef struct _observer_base_t observer_base_t;

/* needed by derived classes' new()s */
typedef void (*observer_cb_t)( observer_base_t *obs, void *ctxt );

extern void observer_base_delete( observer_base_t *obs );
extern void observer_base_update( observer_base_t *obs, int value );
extern engine_reg_t observer_base_get_reg( observer_base_t *obs );
extern int observer_base_get_value( observer_base_t *obs ); /* all observers support getting their current value */

#endif /* defined _INCLUDED_OBSERVER_BASE_H */
