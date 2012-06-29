#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>
#include <errno.h>

#include "common.h"
#include "com.h"
#include "com_utils.h"


static int s_tty;


int com_init( char *tty_dev_name )
{
    int ret = 1;
    struct termios *tios;

    s_tty = open( tty_dev_name, O_RDWR | O_NOCTTY | O_NDELAY );

    if( s_tty != -1 )
    {
        tios = calloc( sizeof( struct termios ), 1 );

        /* TODO: do we want to read these at all, or just assert our own set? */
        tcgetattr( s_tty, tios );

        /* "something like the "raw" mode of the old Version 7 terminal driver"
         * Good baseline */
        cfmakeraw( tios );

        /* Input modes */
        //tios->c_iflag |= IGNBRK;
        //tios->c_iflag &= ~( 0 );

        /* Output modes */
        //tios->c_oflag |= 0;
        //tios->c_oflag &= ~( OPOST );

        /* Control modes */
        /* Magic runes that equate to 8N1 */
        tios->c_cflag &= ~( PARENB | CSTOPB | CSIZE );
        tios->c_cflag |= CS8;

        //tios->c_cflag |= CREAD | CLOCAL | CRTSCTS; /* Enable reading | Ign ctrl lines | H/W flow ctrl */
        //tios->c_cflag &= ~( IXON | IXOFF ); /* S/W flow ctrl */

        /* Local modes */
        //tios->c_lflags |= 0;
        //tios->c_lflags &= ~( ICANON | ISIG | ECHO | IEXTEN ); /* Canonical mode | Raise signals for ctrl chars | echo input chars | impl-defined input proc */

        /* Wait for at least one character (as sometimes we only want to read
         * one) */
        tios->c_cc[VTIME] = 0; /* deciseconds */
        tios->c_cc[VMIN ] = 1;

        /* 9600 baud */
        assert( !cfsetspeed( tios, B9600 ) );


        if( tcsetattr( s_tty, TCSADRAIN, tios ) )
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


static ssize_t write_wrapper( void *buf, size_t count )
{
    return write( s_tty, buf, count );
}
int com_send_bytes( uint8_t *buf, unsigned count )
{
    return com_read_write_wrapper( buf, count, &write_wrapper );
}

static ssize_t read_wrapper( void *buf, size_t count )
{
    return read( s_tty, buf, count );
}
int com_read_bytes( uint8_t *buf, unsigned count )
{
    return com_read_write_wrapper( buf, count, &read_wrapper );
}
