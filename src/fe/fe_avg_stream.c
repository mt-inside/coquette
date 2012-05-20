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
    stream_t *stream = malloc( sizeof(stream_t) );

    stream->observers_len = 4;
    stream->observers = malloc( sizeof(observer_base_t *) * stream->observers_len );

    stream->observers[0] = (observer_base_t *)observer_stats_new( reg_engine_COOLANT_TEMP, &stats_print_cb, NULL, 0 );
    stream->observers[1] = (observer_base_t *)observer_stats_new( reg_TACHO,               &stats_print_cb, NULL, 0 );
    stream->observers[2] = (observer_base_t *)observer_stats_new( reg_ROAD_SPEED,          &stats_print_cb, NULL, 0 );
    stream->observers[3] = (observer_base_t *)observer_stats_new( reg_TPS,                 &stats_print_cb, NULL, 0 );


    /* spawns streaming thread and returns */
    stream_registers_start( stream );
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
