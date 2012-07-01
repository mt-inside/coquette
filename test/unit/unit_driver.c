#include "common.h"
#include <stdio.h>
#include "unit_driver.h"

int main( int argc, char **argv )
{
    NOT_USED( argc );
    NOT_USED( argv );

    observer_stats_tests( );

    printf( "ALL TESTS PASSED\n" );

    return 0;
}
