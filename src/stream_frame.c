#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "stream_frame.h"
#include "commands.h"
#include "registers.h"
#include "observer.h"
#include "com/com.h"
#include "consult_constants.h"


typedef struct
{
    stream_t **streams;
    unsigned streams_len;
} stream_cb_ctxt_t;

typedef void (*stream_cb_t)( void *stream_cb_ctxt, uint8_t *data, unsigned data_len );
typedef void (*mapper_fn_t)( void *in, void *out );


static void stream_cb( void *stream_cb_ctxt, uint8_t *data, unsigned data_len );
void *map( void *in, unsigned in_stride, unsigned out_stride, unsigned count, mapper_fn_t mapper );
static void stream_reg_mapper_fn( void *in, void *out );
static int stream_frame_inner( cmd_t cmd,
                               uint8_t *args,  unsigned arg_len,
                               stream_cb_t cb, void *stream_cb_ctxt );


int stream_registers( stream_t **streams, unsigned streams_len )
{
    stream_cb_ctxt_t *cb_ctxt = malloc( sizeof(stream_cb_ctxt_t) );
    uint8_t *args = map( streams, sizeof(stream_t *), sizeof(uint8_t), streams_len, &stream_reg_mapper_fn );

    cb_ctxt->streams = streams;
    cb_ctxt->streams_len = streams_len;

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
    static uint8_t *old_data;
    static unsigned old_data_len;

    stream_cb_ctxt_t *ctxt = (stream_cb_ctxt_t *)stream_cb_ctxt;
    stream_t **streams = ctxt->streams;
    unsigned streams_len = ctxt->streams_len;
    unsigned offset;
    unsigned i, j;
    int datum;


    assert( data_len == old_data_len );
    if( !memcmp( data, old_data, data_len ) ) return;

    offset = 0;
    for( i = 0; i < streams_len; ++i )
    {
        stream_t *stream = streams[i];
        reg_info_t *reg_info = registers_get_reg_info( stream->reg );

        datum = 0;
        memcpy( &datum, data + offset, reg_info->width );
        offset += reg_info->width;

        /* FIXME: only if changed */
        /* FIXME apply scaling */
        for( j = 0; j < stream->observers_len; ++j )
        {
            observer_update( stream->observers[j], datum );
        }
    }
}

/* todo: move me */
void *map( void *in, unsigned in_stride, unsigned out_stride, unsigned count, mapper_fn_t mapper )
{
    void *out = malloc( count * out_stride );
    unsigned i;

    for( i = 0; i < count; ++i )
    {
        mapper( (uint8_t *)in + i * in_stride, (uint8_t *)out + i * out_stride );
    }

    return out;
}

static void stream_reg_mapper_fn( void *in, void *out )
{
    stream_t *s = (stream_t *)in;
    uint8_t *r = (uint8_t *)out;

    *r = s->reg; /* also truncates enum type to uint8_t */
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
