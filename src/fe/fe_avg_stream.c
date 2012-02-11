#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>

#include "common.h"
#include "com/com.h"
#include "commands.h"
#include "observers/stats_observer.h"
#include "stream_frame.h"


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
    const unsigned stream_count = 5;
    stream_t **streams = malloc( sizeof(stream_t *) * stream_count );
    unsigned i;


    for( i = 0; i < stream_count; ++i )
    {
        streams[i] = malloc( sizeof(stream_t) );
        streams[i]->observers = malloc( sizeof(observer_t *) );
        streams[i]->observers_len = 1;
    }


    streams[0]->reg = reg_engine_COOLANT_TEMP;
    streams[0]->observers[0] = (observer_t *)stats_observer_new( &stats_print_cb, NULL );

    streams[1]->reg = reg_TACHO;
    streams[1]->observers[0] = (observer_t *)stats_observer_new( &stats_print_cb, NULL );

    streams[2]->reg = reg_ROAD_SPEED;
    streams[2]->observers[0] = (observer_t *)stats_observer_new( &stats_print_cb, NULL );

    streams[3]->reg = reg_BATT_VOLT;
    streams[3]->observers[0] = (observer_t *)stats_observer_new( &stats_print_cb, NULL );

    streams[4]->reg = reg_TPS;
    streams[4]->observers[0] = (observer_t *)stats_observer_new( &stats_print_cb, NULL );


    stream_registers(
        streams,
        stream_count
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
