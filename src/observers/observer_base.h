/* Public API of the observer base type */

#ifndef _INCLUDED_OBSERVER_BASE_H
#define _INCLUDED_OBSERVER_BASE_H

#include "registers.h"


typedef struct _observer_base_t observer_base_t;

/* Type of the function that the observer calls when it has new data.
 * Aka the "Up" call. Provided to the observer_*_new() functions.
 * This is just a Unit atm; the notified party has to call back into the
 * observer to get the data */
typedef void (*observer_cb_t)( observer_base_t *obs, void *ctxt );

/* observer_*_new is defined on the sub-classes */
extern void observer_base_delete( observer_base_t *obs ); /* virtual */
extern void observer_base_update( observer_base_t *obs, int value ); /* virtual */
extern engine_reg_t observer_base_get_reg( observer_base_t *obs );
extern int observer_base_get_value( observer_base_t *obs ); /* all observers support getting their current value */

#endif /* defined _INCLUDED_OBSERVER_BASE_H */
