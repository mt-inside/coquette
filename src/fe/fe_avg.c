#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>

#include "common.h"
#include "com/com.h"
#include "commands.h"
#include "observer.h"
#include "stats_observer.h"
#include "watcher.h"


static int s_ending = 0;


static void sig_int_handler( int signum )
{
    assert( signum == SIGINT );

    LOG( "caught SIGINT; cleaning up" );

    s_ending = 1;
}

static void stats_print_cb( observer_t *obs, void *ctxt )
{
    printf( "min: %d\tcurrent: %d\tmax: %d\n",
            stats_observer_get_min( obs ),
            observer_get_current( obs ),
            stats_observer_get_max( obs )
          );
}

static void init_watch( void )
{
    const unsigned watch_count = 1;
    watch_t **watches = malloc( sizeof(watch_t *) * watch_count );
    unsigned i;


    for( i = 0, i < watch_count; ++i )
    {
        watches[ i ] = malloc( sizeof(watch_t) );
    }

    watches[0]->reg = engine_reg_COOLANT_TEMP;
    watched[0]->obs = stats_observer_new( );


    watch(
        watches,
        sizeof(watches) / sizeof(watches[0]),
        &print_cb,
        NULL
    );
}

int main( int argc, char **argv )
{
    assert( argc == 2 );

    //signal( SIGINT, &sig_int_handler );

    com_init( argv[1] );
    handshake( ecu_ENGINE );
    LOG( "handshake done" );

    init_watch();

    LOG( "ending" );
    com_finalise( );

    return 0;
}
