#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>

#include "common.h"
#include "com/com.h"
#include "commands.h"
#include "observers/stats_observer.h"
#include "watcher.h"


static void stats_print_cb( observer_t *obs, void *ctxt )
{
    int min, max, mean, stdev;

    (void)ctxt;

    stats_observer_get_stats( obs, 0, &min, &max, &mean, &stdev );

    printf( "current: %d\tmin: %d\tmean: %d\tmax: %d\tstdev: %d\n",
            observer_get_value( obs ),
            min,
            mean,
            max,
            stdev
          );
}

static void init_watch( void )
{
    const unsigned watch_count = 5;
    watch_t **watches = malloc( sizeof(watch_t *) * watch_count );
    unsigned i;


    for( i = 0; i < watch_count; ++i )
    {
        watches[ i ] = malloc( sizeof(watch_t) );
    }

    watches[0]->reg = reg_engine_COOLANT_TEMP;
    watches[0]->obs = (observer_t *)stats_observer_new( );

    watches[1]->reg = reg_TACHO;
    watches[1]->obs = (observer_t *)stats_observer_new( );

    watches[2]->reg = reg_ROAD_SPEED;
    watches[2]->obs = (observer_t *)stats_observer_new( );

    watches[3]->reg = reg_BATT_VOLT;
    watches[3]->obs = (observer_t *)stats_observer_new( );

    watches[4]->reg = reg_TPS;
    watches[4]->obs = (observer_t *)stats_observer_new( );


    watch(
        watches,
        watch_count,
        &stats_print_cb,
        NULL
    );
}

int main( int argc, char **argv )
{
    assert( argc == 2 );

    com_init( argv[1] );
    handshake( ecu_ENGINE );
    LOG( "handshake done" );

    init_watch();

    LOG( "ending" );
    com_finalise( );

    return 0;
}
