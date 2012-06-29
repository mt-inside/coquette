#include <windows.h>
#include <unistd.h>

#include "com.h"


static HANDLE s_tty;


int com_init( char *tty_dev_name )
{
    int ret = 1;

    s_tty = CreateFile( tty_dev_name,
                        GENERIC_READ | GENERIC_WRITE,
                        0,
                        0,
                        OPEN_EXISTING,
                        FILE_ATTRIBUTE_NORMAL,
                        0 );

    if( s_tty != INVALID_HANDLE_VALUE )
    {
        DCB *params = calloc( sizeof( DCB ), 1 );
        params->DCBlength = sizeof( DCB );

        if( GetCommState( s_tty, params ) )
        {
            params->BaudRate = CBR_9600;
            params->ByteSize = 8;
            params->StopBits = ONESTOPBIT;
            params->Parity   = NOPARITY;

            if( SetCommState( s_tty, params ) )
            {
                COMMTIMEOUTS *timeouts = calloc( sizeof( COMMTIMEOUTS ), 1 );

                /* TODO: these need looking at. We will always ask for no more
                 * characters than will be available under normal circumstances.
                 * Thus we should ask for an infinite timeout in case the ECU
                 * is slow. NB: in the real world these have always been fine.
                 * TODO: calculate the inter-byte time at 9600Baud. */
                /* Inter-byte timeout */
                timeouts->ReadIntervalTimeout = 50; /* ms */
                /* Total timeout for a read() equals:
                 *   Multiplier * no req. bytes + Constant */
                timeouts->ReadTotalTimeoutMultiplier = 10;
                timeouts->ReadTotalTimeoutConstant = 50;
                timeouts->WriteTotalTimeoutConstant = 50;
                timeouts->WriteTotalTimeoutMultiplier = 10;

                if( SetCommTimeouts( s_tty, timeouts ) )
                {
                    ret = 0;
                }
            }
        }
    }

    return ret;
}

int com_finalise( void )
{
    return !CloseHandle( s_tty );
}


ssize_t write_wrapper( void *buf, size_t count )
{
    DWORD sent;
    if( WriteFile( s_tty, buf, count, &sent, NULL ) == 0 ) return -1;
    return sent;
}

ssize_t read_wrapper( void *buf, size_t count )
{
    DWORD sent;
    if( ReadFile( s_tty, buf, count, &sent, NULL ) == 0 ) return -1;
    return sent;
}
