#include "common.h"

#include <stdio.h>
#include <unistd.h>
#include <assert.h>

#include "com.h"
#include "commands.h"
#include "consult_constants.h"


typedef enum
{
    state_RUNNING,
    state_STOPPING,
    state_FINALISED
} state_t;

static FILE *s_file;
static state_t s_state;


int com_init( char *path )
{
    s_state = state_RUNNING;
    return( !( s_file = fopen( path, "r" ) ) );
}

int com_finalise( void )
{
    s_state = state_FINALISED;
    return fclose( s_file );
}


int com_send_byte( uint8_t byte )
{
    if( byte == cmd_STOP )
    {
        assert( s_state == state_RUNNING );
        s_state = state_STOPPING;
    }

    printf( "COM_REPLAY > 0x%02x\n", byte );

    return 0;
}

int com_send_bytes( uint8_t *bytes, unsigned count )
{
    unsigned i;

    for( i = 0; i < count; ++i )
    {
        com_send_byte( bytes[i] );
    }

    return 0;
}


int com_read_byte( uint8_t *byte )
{
    unsigned x;

    usleep(100 * 1000);

    switch( s_state )
    {
        case state_STOPPING:
            *byte = c_end_of_response;
            break;
        case state_RUNNING:
            fscanf( s_file, "%x ", &x );
            *byte = (uint8_t)x;
            break;
        default:
            assert( 0 );
    }

    printf( "COM_REPLAY < 0x%02x\n", *byte );

    return 0;
}

int com_read_bytes( uint8_t *bytes, unsigned count )
{
    unsigned i;

    for( i = 0; i < count; ++i )
    {
        com_read_byte( &(bytes[i]) );
    }

    return 0;
}
