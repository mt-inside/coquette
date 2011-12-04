#include <stdio.h>
#include <assert.h>
#include <unistd.h>

#include "common.h"
#include "com/com.h"
#include "commands.h"


static int read_faults( void )
{
    fault_report_t *faults, *fault;

    read_dtc( &faults );
    fault = faults;
    while( fault )
    {
        printf( "Fault code %#x, last seen %d engine starts ago\n",
                fault->code, fault->last_seen );

        fault = fault->next;
    }

    faults_free( faults );

    return 0;
}

static int read_regs( void )
{
    int reg;

    read_register( reg_TACHO, &reg );
    printf( "engine %d rpm\n", reg );

    read_register( reg_engine_COOLANT_TEMP, &reg );
    printf( "coolant %d degC\n", reg );

    return 0;
}

static int read_bit( void )
{
    int bit;

    read_flag( bit_AC_ON_SWITCH, &bit );

    printf( "AC switch: %s\n", bit ? "on" : "off" );

    return 0;
}

int main( int argc, char **argv )
{
    assert( argc == 2 );

    com_init( argv[1] );
    handshake( ecu_ENGINE );
    LOG( "handshake done" );

    read_faults( );

    while( 1 )
    {
        read_regs( );
        read_bit( );

        sleep( 1 );
    }

    LOG( "ending" );
    com_finalise( );

    return 0;
}
