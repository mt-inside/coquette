#include <stdlib.h>
#include <stdarg.h>
#include <assert.h>

#include "read_frame.h"
#include "com/com.h"
#include "consult_constants.h"


static int read_frame_inner( cmd_t cmd,
                             uint8_t *args,  unsigned arg_len,
                             uint8_t **data, unsigned *data_len );


int read_frame( cmd_t cmd,
                uint8_t **data, unsigned *data_len )
{
    return read_frame_inner( cmd, NULL, 0, data, data_len );
}

int read_frame_arg( cmd_t cmd,
                    uint8_t arg,
                    uint8_t **data, unsigned *data_len )
{
    return read_frame_inner( cmd, &arg, 1, data, data_len );
}

int read_frame_args( cmd_t cmd,
                     uint8_t **data, unsigned *data_len,
                     unsigned args_len, ... )
{
    int ret;
    unsigned i;
    va_list va_args;
    uint8_t *args;

    va_start( va_args, args_len );

    args = malloc( args_len );
    for( i = 0; i < args_len; ++i )
    {
        /* ... promotes uint8_t to int */
        args[ i ] = (uint8_t)va_arg( va_args, int );
    }
    ret = read_frame_inner( cmd, args, args_len, data, data_len );

    free( args );
    va_end( va_args );

    return ret;
}

/* TODO this assumes args are 1 byte each so can't be used e.g. to read ROM
 * data. Need an arg_stride argument */
static int read_frame_inner( cmd_t cmd,
                             uint8_t *args,  unsigned arg_len,
                             uint8_t **data, unsigned *data_len )
{
    unsigned i;
    uint8_t out;

    if( arg_len == 0 ) com_send_byte( (uint8_t)cmd );
    for( i = 0; i < arg_len; ++i )
    {
        com_send_byte( (uint8_t)cmd );
        com_send_byte( args[i] );
    }

    com_send_byte( c_end_of_request );

    if( arg_len == 0 ) { com_read_byte( &out ); assert( out == (uint8_t)~cmd ); }
    for( i = 0; i < arg_len; ++i )
    {
        com_read_byte( &out );
        assert( out == (uint8_t)~cmd );

        com_read_byte( &out );
        assert( out == args[i] );
    }

    com_read_byte( &out );
    assert( out == c_response_frame_start );

    com_read_byte( &out );
    *data_len = out;
    assert( *data_len > 0 );

    *data = malloc( *data_len );
    com_read_bytes( *data, *data_len );

    /* ECU continues to steam... */

    com_send_byte( cmd_STOP );

    /* Read (hence don't return) to the end of this frame so that we're in a
     * sane state for the next request */
    do { com_read_byte( &out ); } while( out != c_end_of_response );


    return 0;
}
