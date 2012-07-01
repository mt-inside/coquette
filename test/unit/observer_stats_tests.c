#include "common.h"
#include <assert.h>
#include <stdio.h>
#include "unit_driver.h"
#include "observer_stats.h"


static int expected_min,
           expected_max,
           expected_mean,
           expected_stdev;

static void test_cb( observer_base_t *obs, void *ctxt )
{
    int min, max, mean, stdev;

    NOT_USED( ctxt );

    observer_stats_get_stats( obs, &min, &max, &mean, &stdev );

    assert( min == expected_min );
    assert( max == expected_max );
    assert( mean == expected_mean );
    assert( stdev == expected_stdev );
}

static void test_basic( void )
{
    observer_stats_t *obs = observer_stats_new( 0, &test_cb, NULL, 0 );

    expected_min = 0;
    expected_max = 0;
    expected_mean = 0;
    expected_stdev = 0;
    observer_base_update( (observer_base_t *)obs, 0 );

    expected_min = 0;
    expected_max = 2;
    expected_mean = 1;
    expected_stdev = 1;
    observer_base_update( (observer_base_t *)obs, 2 );

    expected_min = 0;
    expected_max = 10;
    expected_mean = 4;
    expected_stdev = 4;
    observer_base_update( (observer_base_t *)obs, 10 );

    expected_min = 0;
    expected_max = 10;
    expected_mean = 6; // 5.5
    expected_stdev = 5; // 4.5
    observer_base_update( (observer_base_t *)obs, 10 );

    expected_min = 0;
    expected_max = 1000;
    expected_mean = 204; // 204.4
    expected_stdev = 398; // 397.82...
    observer_base_update( (observer_base_t *)obs, 1000 );

    expected_min = -12345;
    expected_max = 1000;
    expected_mean = -1887; // -1887.1666...
    expected_stdev = 4691; // 4690.963...
    observer_base_update( (observer_base_t *)obs, -12345 );

    observer_base_delete( (observer_base_t *)obs );
}

static void test_small_range( void )
{
    observer_stats_t *obs = observer_stats_new( 0, &test_cb, NULL, 0 );

    expected_min = 0;
    expected_max = 0;
    expected_mean = 0;
    expected_stdev = 0;
    observer_base_update( (observer_base_t *)obs, 0 );

    expected_min = 0;
    expected_max = 0;
    expected_mean = 0;
    expected_stdev = 0;
    observer_base_update( (observer_base_t *)obs, 0 );

    expected_min = 0;
    expected_max = 1;
    expected_mean = 0;
    expected_stdev = 0;
    observer_base_update( (observer_base_t *)obs, 1 );

    expected_min = 0;
    expected_max = 1;
    expected_mean = 1;
    expected_stdev = 1;
    observer_base_update( (observer_base_t *)obs, 1 );

    expected_min = 0;
    expected_max = 1;
    expected_mean = 1;
    expected_stdev = 0;
    observer_base_update( (observer_base_t *)obs, 1 );

    expected_min = 0;
    expected_max = 1;
    expected_mean = 1;
    expected_stdev = 1;
    observer_base_update( (observer_base_t *)obs, 0 );

    observer_base_delete( (observer_base_t *)obs );
}

void observer_stats_tests( void )
{
    test_basic( );
    test_small_range( );

    printf( "observer_stats_tests PASSED\n" );
}
