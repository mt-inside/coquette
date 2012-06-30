#include "common.h"

#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>
#include <errno.h>

#include "com.h"


static int s_tty;


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

    /* Setting NDELAY causes VTIME & VMIN to be ignored */
    /* TODO: turns off NDELAY. try setting FNDELAY. */
    if( fcntl( s_tty, F_SETFL, 0 ) )
    {
        perror( "Failed to fcntl() COM port" );
        goto error0;
    }

    tios = calloc( sizeof( struct termios ), 1 );

    /* TODO: do we want to read these at all, or just assert our own set? */
    /* TODO: try me off */
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

    /* Input modes */
    //tios->c_iflag |= 0;
    //tios->c_iflag &= ~( 0 );

    /* Output modes */
    //tios->c_oflag |= 0;
    //tios->c_oflag &= ~( 0 );

    /* Control modes */
    /* Magic runes that equate to 8N1 */
    tios->c_cflag &= ~( PARENB | CSTOPB ); /* No parity | 1 stop bit */
    tios->c_cflag &= ~CSIZE; tios->c_cflag |= CS8; /* 8bits */

    tios->c_cflag |= CREAD | CLOCAL; /* Enable reading | Ign ctrl lines */

    tios->c_cflag &= ~CRTSCTS; /* TODO: hw flow control, try me */
    //tios->c_cflag |= CRTSCTS; /* TODO: hw flow control, try me */

    tios->c_cflag &= ~( IXON | IXOFF | IXANY ); /* TODO: S/W flow ctrl, try me */
    //tios->c_cflag |= IXON | IXOFF | IXANY; /* TODO: S/W flow ctrl, try me */

    /* Local modes */
    //tios->c_lflags |= 0;
    //tios->c_lflags &= ~( 0 );

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
