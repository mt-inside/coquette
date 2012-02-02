#ifndef _INCLUDED_WATCHER_H
#define _INCLUDED_WATCHER_H

#include "commands.h"
#include "observers/observer.h"


typedef struct
{
    engine_reg_t reg;
    observer_t *obs;
} watch_t;

typedef void (*watcher_cb_t)( observer_t *obs, void *ctxt );
extern int watch( watch_t **watches, unsigned no_watches, watcher_cb_t cb, void *ctxt );

#endif /* defined _INCLUDED_WATCHER_H */
