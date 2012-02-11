#include <unistd.h>

#include "watcher.h"
#include "commands.h"

#if 0
This is a utility to pivot between top-down call-driven approach and bottom-up data-streaming approach.
This is bad and wrong - we should be using stream_frame all the way through

int watch(
    watch_t **watches,
    unsigned no_watches,
    watcher_cb_t cb,
    void *ctxt
)
{
    unsigned i;
    int value;
    watch_t *watch;


    while( 1 )
    {
        for( i = 0; i < no_watches; ++i )
        {
            watch = watches[i];
            read_register( watch->reg, &value ); /* TODO: error */
            observer_update( watch->obs, value );
            cb( watch->obs, ctxt );
        }

        sleep( 1 );
    }


    return 0;
}
#endif
