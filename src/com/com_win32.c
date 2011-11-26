#include <windows.h>

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
                 * is slow. */
                timeouts->ReadIntervalTimeout = 50;
                timeouts->ReadTotalTimeoutConstant = 50;
                timeouts->ReadTotalTimeoutMultiplier = 10;
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


int com_send_byte( uint8_t byte )
{
    return com_send_bytes( &byte, 1 );
}

int com_send_bytes( uint8_t *bytes, unsigned count )
{
    DWORD sent;
    return !WriteFile( s_tty, bytes, count, &sent, NULL ) || count != sent;
}


int com_read_byte( uint8_t *byte )
{
    return com_read_bytes( byte, 1 );
}

int com_read_bytes( uint8_t *bytes, unsigned count )
{
    DWORD recvd;
    return !ReadFile( s_tty, bytes, count, &recvd, NULL ) || count != recvd;
}
