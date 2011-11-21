#include <stdio.h>

#include "com.h"


int com_send_byte( uint8_t byte )
{
    printf( "COM_STDIO > %#x\n", byte );
    return 1;
}

int com_send_bytes( uint8_t *bytes, unsigned count )
{
    unsigned i;

    for( i = 0; i < count; ++i )
    {
        com_send_byte( bytes[i] );
    }

    return count;
}

int com_read_byte( uint8_t *byte )
{
    unsigned x;

    printf( "COM STDIO < 0x" );

    scanf( "%x", &x );
    *byte = (uint8_t)x;

    return 1;
}


int com_read_bytes( uint8_t *bytes, unsigned count )
{
    unsigned i;

    for( i = 0; i < count; ++i )
    {
        com_read_byte( &(bytes[i]) );
    }

    return count;
}
