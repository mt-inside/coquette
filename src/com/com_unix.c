#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>

#include "com.h"


static int s_tty;


int com_init( char *tty_dev_name )
{
    struct termios *tios = malloc( sizeof( struct termios ) );

    s_tty = open( tty_dev_name, O_RDWR | O_EXLOCK );

    assert( tcgetattr( s_tty, tios ) );

    /* Set to 8N1 */
    tios->c_cflag &= ~CSIZE;  /* clear 5,6,7,8 char size bits */
    tios->c_cflag |= CS8;     /* 8 bit chars */
    tios->c_cflag &= ~CSTOPB; /* Disable 2 stop bit => enable 1 */
    tios->c_cflag &= ~PARENB; /* No parity */

    /* 9600 baud */
    assert( !cfsetspeed( tios, B9600 ) );

    /* raw: no buffering? necessary? */
    cfmakeraw( tios );

    assert( tcsetattr( s_tty, TCSANOW, tios ) );

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
