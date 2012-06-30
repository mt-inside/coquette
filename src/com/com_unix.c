#include "common.h"

#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>
#include <errno.h>

#include "com.h"


static int s_tty;


/* All of these magic runes are neccessary, i.e. setting those settings to
 * something different stops it working. However in testing it was not necessary
 * to assert all of them; some of them are the same as the defaults.
 * However they're left here to a) document the protocol and b) incase future
 * drivers have different defaults.
 * Any setting not explicitly set is either a default that I missed, or
 * something to which it's not sensitive, e.g. s/w flow control */
int com_init( char *tty_dev_name )
{
    int rc = 1;
    struct termios *tios;


    errno = 0;

    if( (s_tty = open( tty_dev_name, O_RDWR | O_NOCTTY | O_NDELAY )) == -1 )
    {
        perror( "Failed to open() COM port" );
        goto error0;
    }

    /* Clear NDELAY.
     * Setting NDELAY would cause VTIME & VMIN to be ignored */
    if( fcntl( s_tty, F_SETFL, 0 ) )
    {
        perror( "Failed to fcntl() COM port" );
        goto error0;
    }

    tios = calloc( sizeof( struct termios ), 1 );

    /* In testing this didn't seem to be neccessary, but future drivers might
     * stash funny things in this that we need to preserve. It doesn't hurt to
     * do this because we re-assert everything upon which we depend */
    if( tcgetattr( s_tty, tios ) )
    {
        perror( "Failed to tcgetattr() on COM port" );
        goto error1;
    }

    /* "something like the "raw" mode of the old Version 7 terminal driver"
    Good baseline, equal to
    termios_p->c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP
            | INLCR | IGNCR | ICRNL | IXON);
    termios_p->c_oflag &= ~OPOST;
    termios_p->c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
    termios_p->c_cflag &= ~(CSIZE | PARENB);
    termios_p->c_cflag |= CS8;
    */
    cfmakeraw( tios );

    /* Control modes */
    tios->c_cflag &= ~( PARENB | CSTOPB ); /* No parity | 1 stop bit */
    tios->c_cflag &= ~CSIZE; tios->c_cflag |= CS8; /* 8bits */
    tios->c_cflag |= CREAD | CLOCAL; /* Enable reading | Ign ctrl lines */
    tios->c_cflag &= ~CRTSCTS; /* No h/w flow control */

    /* Control characters */
    /* Wait for at least one character (as sometimes we only want to read
     * one) */
    tios->c_cc[VTIME] = 0; /* deciseconds */
    tios->c_cc[VMIN ] = 1;

    /* Speed */
    /* 9600 baud */
    if( cfsetspeed( tios, B9600 ) )
    {
        perror( "Failed to cfsetspeed() on COM port" );
        goto error1;
    }

    if( tcsetattr( s_tty, TCSANOW, tios ) )
    {
        perror( "failed to tcsetattr() on COM port" );
        goto error1;
    }

    LOG( "com_init done" );
    rc = 0;

error1:
    free( tios );

error0:
    return rc;
}

int com_finalise( void )
{
    return close( s_tty );
}


ssize_t write_wrapper( void *buf, size_t count )
{
    return write( s_tty, buf, count );
}

ssize_t read_wrapper( void *buf, size_t count )
{
    return read( s_tty, buf, count );
}
