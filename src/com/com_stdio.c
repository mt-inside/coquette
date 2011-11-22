#include <stdio.h>

#include "com.h"


int com_init( char *tty_dev_name )
{
    (void)tty_dev_name;

    return 0;
}

int com_finalise( void )
{
    return 0;
}


int com_send_byte( uint8_t byte )
{
    printf( "COM_STDIO > 0x%02x\n", byte );

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

    printf( "COM_STDIO < 0x" );

    scanf( "%x", &x );
    *byte = (uint8_t)x;

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
