#include <stdio.h>
#include <inttypes.h>
#include <assert.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>

#include "common.h"
#include "com/com.h"
#include "commands.h"
#include "observers/observer_derivative.h"
#include "observers/observer_integral.h"
#include "observers/observer_shift.h"
#include "observers/observer_stats.h"
#include "stream_frame.h"


static void derivative_print_cb( observer_base_t *obs, void *ctxt )
{
    float derivative;

    (void)ctxt;

    observer_derivative_get_derivative( obs, &derivative );

    printf( "derivative: %f\n",
            derivative
          );
}

static void integral_print_cb( observer_base_t *obs, void *ctxt )
{
    int64_t integral;

    (void)ctxt;

    observer_integral_get_integral( obs, &integral );

    printf( "distance travelled: %" PRId64 "\n",
            integral
          );
}

static void shift_print_cb( observer_base_t *obs, void *ctxt )
{
    unsigned level;

    (void)ctxt;

    observer_shift_get_shift( obs, &level );

    printf( "shift level: %d\n",
            level
          );
}

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

static stream_t *get_stream( void )
{
    return stream_new( 8,
        (observer_base_t *)observer_stats_new     ( reg_engine_COOLANT_TEMP, &stats_print_cb,      NULL, 0              ),
        (observer_base_t *)observer_derivative_new( reg_engine_COOLANT_TEMP, &derivative_print_cb, NULL, 1              ),
        (observer_base_t *)observer_stats_new     ( reg_ENGINE_SPEED,        &stats_print_cb,      NULL, 0              ),
        (observer_base_t *)observer_shift_new     ( reg_ENGINE_SPEED,        &shift_print_cb,      NULL, 2000, 3000, 10 ),
        (observer_base_t *)observer_stats_new     ( reg_ROAD_SPEED,          &stats_print_cb,      NULL, 0              ),
        (observer_base_t *)observer_integral_new  ( reg_ROAD_SPEED,          &integral_print_cb,   NULL                 ),
        (observer_base_t *)observer_derivative_new( reg_ROAD_SPEED,          &derivative_print_cb, NULL, 1              ),
        (observer_base_t *)observer_stats_new     ( reg_TPS,                 &stats_print_cb,      NULL, 0              )
    );
}

int main( int argc, char **argv )
{
    stream_t *stream;

    assert( argc == 2 );

    assert( !com_init( argv[1] ) );

    handshake( ecu_ENGINE );

    LOG( "handshake done" );

    stream = get_stream();
    /* spawns streaming thread and returns */
    stream_registers_start( stream );

    /* block this thread */
    getchar();

    LOG( "ending" );

    stream_registers_end( );
    stream_delete( stream );

    com_finalise( );

    return 0;
}
