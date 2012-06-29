#include "common.h"
#include "com_utils.h"
#include "com.h"


int com_read_byte( uint8_t *byte )
{
    return com_read_bytes( byte, 1 );
}
int com_send_byte( uint8_t byte )
{
    return com_send_bytes( &byte, 1 );
}

int com_read_write_wrapper(
    uint8_t *buf, unsigned count,
    com_wrapper_t fn
)
{
    ssize_t rc;

    do
    {
        rc = fn( buf, count );
        if( rc == -1 ) LOG( "com: fn() got -1" );
        if( rc ==  0 ) LOG( "com: fn() got 0" );
        /* should never get 0 bytes as we have a 1 byte timeout on the socket
         * */
        if( rc == -1 || rc == 0 ) return 1;
        buf += rc;
        count -= rc;
    } while( count );

    return 0;
}
