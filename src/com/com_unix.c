#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>
#include <errno.h>

#include "common.h"
#include "com.h"


static int s_tty;


int com_init( char *tty_dev_name )
{
    int ret = 1;
    struct termios *tios;

    s_tty = open( tty_dev_name, O_RDWR | O_NOCTTY | O_NDELAY );

    if( s_tty != -1 )
    {
        tios = calloc( sizeof( struct termios ), 1 );

        tcgetattr( s_tty, tios );

        /* Magic runes that equate to 8N1 */
        tios->c_cflag |= ( CS8 );
        tios->c_cflag &= ~( PARENB | CSTOPB | CSIZE );
        /* tios->c_cflag |= ( CRTSCTS | CLOCAL | CREAD ); */
        /* tios->c_cflag &= ~( ICANON | ECHO | ECHOE | ISIG ); */

        /* tios->c_oflag &= ~OPOST; */

        /* No waiting for time or multiple characters */
        /* tios->c_cc[VTIME] = 0; */
        /* tios->c_cc[VMIN ] = 0; */

        /* 9600 baud */
        assert( !cfsetspeed( tios, B9600 ) );

        /* raw: no buffering? necessary? */
        /* cfmakeraw( tios ); */

        if( tcsetattr( s_tty, TCSANOW, tios ) )
        {
            perror( "failed to tcsetattr" );
        }
        else
        {
            ret = 0;
        }

        free( tios );

        LOG( "com_init done" );
    }

    return ret;
}

int com_finalise( void )
{
    return close( s_tty );
}


int com_send_byte( uint8_t byte )
{
    return com_send_bytes( &byte, 1 );
}

int com_send_bytes( uint8_t *bytes, unsigned count )
{
    return write( s_tty, bytes, count ) != count;
}


int com_read_byte( uint8_t *byte )
{
    return com_read_bytes( byte, 1 );
}

int com_read_bytes( uint8_t *bytes, unsigned count )
{
    return read( s_tty, bytes, count ) != count;
}
