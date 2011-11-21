#include <fcntl.h>
#include <unistd.h>

#include "com.h"


static int s_tty;


int com_init( char *tty_dev_name )
{
    s_tty = open( tty_dev_name, O_RDWR | O_EXLOCK );

    return( s_tty != -1 );
}

int com_finalise( void )
{
    return close( s_tty );
}


int com_send_byte( uint8_t byte )
{
    return( com_send_bytes( &byte, 1 ) );
}

int com_send_bytes( uint8_t *bytes, unsigned count )
{
    return( write( s_tty, bytes, count ) == count );
}


int com_read_byte( uint8_t *byte )
{
    return( com_read_bytes( byte, 1 ) );
}

int com_read_bytes( uint8_t *bytes, unsigned count )
{
    return( read( s_tty, bytes, count ) == count );
}
