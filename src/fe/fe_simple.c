#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>

#include "common.h"
#include "com/com.h"
#include "commands.h"


static int s_ending = 0;


static void sig_int_handler( int signum )
{
    assert( signum == SIGINT );

    LOG( "caught SIGINT; cleaning up" );

    s_ending = 1;
}


static int read_serial( void )
{
    ecu_part_no_t *part_no;

    read_ecu_part_no( &part_no );

    printf( "%hx %x-%s\n", part_no->part1, part_no->part2, part_no->sw_ver );

    free( part_no );

    return 0;
}

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

    read_register( reg_ENGINE_SPEED, &reg );
    printf( "engine %d rpm\n", reg );

    read_register( reg_engine_COOLANT_TEMP, &reg );
    printf( "coolant %d degC\n", reg );

    read_register( reg_BATT_VOLT, &reg );
    printf( "battery %d mV\n", reg );

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

    signal( SIGINT, &sig_int_handler );

    com_init( argv[1] );
    handshake( ecu_ENGINE );
    LOG( "handshake done" );

    read_serial( );
    read_faults( );

    while( !s_ending )
    {
        read_regs( );
        read_bit( );

        sleep( 1 );
    }

    LOG( "ending" );
    com_finalise( );

    return 0;
}
