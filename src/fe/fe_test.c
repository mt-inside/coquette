#include <stdio.h>
#include <assert.h>

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

    /* read_register( reg_TACHO, &reg ); */
    read_register( reg_engine_COOLANT_TEMP, &reg );

    printf( "%d $unit\n", reg );

    return 0;
}

static int read_mem( void )
{
    uint8_t data;

    peek_memory( 0x1234, &data );

    printf( "byte at 0x1234: 0x%02x\n", data );

    return 0;
}

static int read_bit( void )
{
    int bit;

    read_flag( bit_FUEL_PUMP_RELAY, &bit );

    printf( "flag: %s\n", bit ? "on" : "off" );

    return 0;
}

int main( int argc, char **argv )
{
    assert( argc == 2 );

    com_init( argv[1] );
    handshake( ecu_ENGINE );

    read_bit( );

    com_finalise( );

    return 0;
}
