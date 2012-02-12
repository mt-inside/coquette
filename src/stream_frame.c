#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "stream_frame.h"
#include "commands.h"
#include "utils.h"
#include "com/com.h"
#include "consult_constants.h"


typedef struct
{
    stream_t **streams;
    unsigned streams_len;
    unsigned data_len;
} stream_cb_ctxt_t;

typedef void (*stream_cb_t)( void *stream_cb_ctxt, uint8_t *data, unsigned data_len );


static void stream_cb( void *stream_cb_ctxt, uint8_t *data, unsigned data_len );
static void stream_reg_mapper_fn( void *in, void *out );
static int stream_frame_inner( cmd_t cmd,
                               uint8_t *args,  unsigned arg_len,
                               stream_cb_t cb, void *stream_cb_ctxt );


int stream_registers( stream_t **streams, unsigned streams_len )
{
    stream_cb_ctxt_t *cb_ctxt = malloc( sizeof(stream_cb_ctxt_t) );

    uint8_t args[20]; /* consult protocol maximum arg size for read_register */
    unsigned args_offset = 0;
    for( i = 0; i < streams_len; ++i )
    {
        reg_info_t *reg_info = registers_get_reg_info( streams[i]->reg );
        for( j = 0; j < reg_info->width; ++j )
        {
            assert( args_offset < 20 );
            args[args_offset++] = streams[i]->reg + j;
        }
    }

    cb_ctxt->streams = streams;
    cb_ctxt->streams_len = streams_len;
    cb_ctxt->data_len = args_offset;

    stream_frame_inner(
        cmd_READ_REGISTER,
        args,
        streams_len,
        &stream_cb,
        cb_ctxt
    );

    return 0;
}


static void stream_cb( void *stream_cb_ctxt, uint8_t *data, unsigned data_len )
{
    static int first_time = 1;
    static uint8_t *old_data;
    static unsigned old_data_len;

    stream_cb_ctxt_t *ctxt = (stream_cb_ctxt_t *)stream_cb_ctxt;
    stream_t **streams = ctxt->streams;
    unsigned streams_len = ctxt->streams_len;
    unsigned offset;
    unsigned i, j;
    int datum;


    assert( data_len == ctxt->data_len );

    if( !first_time )
    {
        assert( data_len == old_data_len );
        if( !memcmp( data, old_data, data_len ) ) return;
    }
    else
    {
        old_data = malloc( sizeof(uint8_t) * data_len );
        first_time = 0;
    }

    old_data_len = data_len;
    memcpy( old_data, data, data_len );


    offset = 0;
    for( i = 0; i < streams_len; ++i )
    {
        stream_t *stream = streams[i];
        reg_info_t *reg_info = registers_get_reg_info( stream->reg );

        datum = 0;
        memcpy( &datum, data + offset, reg_info->width );
        datum = reg_info->scaler( datum );

        offset += reg_info->width;

        /* FIXME: only if changed - currently calls them all if at least one has
         * changed */
        for( j = 0; j < stream->observers_len; ++j )
        {
            observer_update( stream->observers[j], datum );
        }
    }
}

/* TODO this assumes args are 1 byte each so can't be used e.g. to read ROM
 * data. Need an arg_stride argument */
static int stream_frame_inner( cmd_t cmd,
                               uint8_t *args,  unsigned arg_len,
                               stream_cb_t cb, void *stream_cb_ctxt )
{
    unsigned i, data_len;
    uint8_t *data;
    uint8_t out;


    /* Send command and arguments */

    if( arg_len == 0 ) com_send_byte( (uint8_t)cmd );
    for( i = 0; i < arg_len; ++i )
    {
        com_send_byte( (uint8_t)cmd );
        com_send_byte( args[i] );
    }

    com_send_byte( c_end_of_request );


    /* Check response */

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


    /* Stream data */

    com_read_byte( &out );
    data_len = out;
    assert( data_len > 0 );

    data = malloc( data_len );

    while( 1 )
    {
        /* do i get a response frame start every loop? - I'm assuming I don't */
        /* do i get a data len every loop? - I'm assuming I don't */

        /* I assume this blocks until this many bytes have come off the serial
         * port, and thus that this loop doesn't spin */
        com_read_bytes( data, data_len );

        cb( stream_cb_ctxt, data, data_len );
    }

    com_send_byte( cmd_STOP );

    /* Read (hence don't return) to the end of this frame so that we're in a
     * sane state for the next request */
    do { com_read_byte( &out ); } while( out != c_end_of_response );


    return 0;
}
