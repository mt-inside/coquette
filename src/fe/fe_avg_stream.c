#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>

#include "common.h"
#include "com/com.h"
#include "commands.h"
#include "observers/observer_stats.h"
#include "stream_frame.h"


static void stats_print_cb( observer_base_t *obs, void *ctxt )
{
    int min, max, mean, stdev;

    (void)ctxt;

    observer_stats_get_stats( obs, &min, &max, &mean, &stdev );

    printf( "current: %d\tmin: %d\tmean: %d\tmax: %d\tstdev: %d\n",
            observer_base_get_value( obs ),
            min,
            mean,
            max,
            stdev
          );
}

static void init_streaming( void )
{
    const unsigned stream_count = 5;
    stream_t **streams = malloc( sizeof(stream_t *) * stream_count );
    unsigned i;


    for( i = 0; i < stream_count; ++i )
    {
        streams[i] = malloc( sizeof(stream_t) );
        streams[i]->observers = malloc( sizeof(observer_base_t *) );
        streams[i]->observers_len = 1;
    }


    streams[0]->reg = reg_engine_COOLANT_TEMP;
    streams[0]->observers[0] = (observer_base_t *)observer_stats_new( &stats_print_cb, NULL, 0 );

    streams[1]->reg = reg_TACHO;
    streams[1]->observers[0] = (observer_base_t *)observer_stats_new( &stats_print_cb, NULL, 0 );

    streams[2]->reg = reg_ROAD_SPEED;
    streams[2]->observers[0] = (observer_base_t *)observer_stats_new( &stats_print_cb, NULL, 0 );

    streams[3]->reg = reg_BATT_VOLT;
    streams[3]->observers[0] = (observer_base_t *)observer_stats_new( &stats_print_cb, NULL, 0 );

    streams[4]->reg = reg_TPS;
    streams[4]->observers[0] = (observer_base_t *)observer_stats_new( &stats_print_cb, NULL, 0 );


    /* spawns streaming thread and returns */
    stream_registers_start(
        streams,
        stream_count
    );
}

static void quit( void )
{
    LOG( "ending" );

    stream_registers_end( );

    com_finalise( );
}

int main( int argc, char **argv )
{
    assert( argc == 2 );

    assert( !com_init( argv[1] ) );

    handshake( ecu_ENGINE );

    LOG( "handshake done" );

    init_streaming();

    /* block this thread */
    getchar();

    quit();

    return 0;
}
