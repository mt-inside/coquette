#include <stdio.h>

#include "com.h"


static FILE *s_file;


int com_init( char *path )
{
    return( !( s_file = fopen( path, "r" ) ) );
}

int com_finalise( void )
{
    return fclose( s_file );
}


int com_send_byte( uint8_t byte )
{
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

    fscanf( s_file, "%x ", &x );
    *byte = (uint8_t)x;

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
