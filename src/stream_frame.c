#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <pthread.h>

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

typedef struct
{
    uint8_t *regs;
    unsigned regs_len;
    stream_cb_t cb;
    void *cb_ctxt;
} stream_thread_args_t;


/* TODO: lock around me */
static int s_state = 0; /* 0 - not running, 1 - running, 2 - exiting */
static pthread_t s_thread;


static void stream_cb( void *stream_cb_ctxt, uint8_t *data, unsigned data_len );
static void *stream_frame_thread( void *ctxt );


int stream_registers_start( stream_t **streams, unsigned streams_len )
{
    stream_thread_args_t *thread_args =
        malloc( sizeof(stream_thread_args_t) );
    stream_cb_ctxt_t *cb_ctxt = malloc( sizeof(stream_cb_ctxt_t) );
    reg_info_t *reg_info;
    unsigned i, j;
    uint8_t args[20]; /* consult protocol maximum arg size for read_register */
    unsigned args_offset = 0;


    assert( s_state == 0 );

    for( i = 0; i < streams_len; ++i )
    {
        reg_info = registers_get_reg_info( streams[i]->reg );
        for( j = 0; j < reg_info->width; ++j )
        {
            assert( args_offset < 20 );
            args[args_offset++] = streams[i]->reg + j;
        }
    }

    /* fixme make me in the thread function */
    cb_ctxt->streams     = streams;
    cb_ctxt->streams_len = streams_len;
    cb_ctxt->data_len    = args_offset;

    thread_args->regs     = args;
    thread_args->regs_len = args_offset;
    thread_args->cb       = &stream_cb;
    thread_args->cb_ctxt  = cb_ctxt;

    pthread_create(
        &s_thread,
        NULL,
        &stream_frame_thread,
        thread_args
    );

    s_state = 1;


    return 0;
}

void stream_registers_end( void )
{
    void *retval;

    assert( s_state == 1 );

    s_state = 2;
    pthread_join( s_thread, &retval );

    s_state = 0;
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


static void *stream_frame_thread( void *ctxt )
{
    stream_thread_args_t *thread_args = (stream_thread_args_t *)ctxt;
    uint8_t *args    = thread_args->regs;
    unsigned arg_len = thread_args->regs_len;
    stream_cb_t cb   = thread_args->cb;
    void *cb_ctxt    = thread_args->cb_ctxt;

    unsigned i, data_len;
    uint8_t *data = NULL;
    uint8_t out;


    /* Send command and arguments */

    if( arg_len == 0 ) com_send_byte( (uint8_t)cmd_READ_REGISTER );
    for( i = 0; i < arg_len; ++i )
    {
        com_send_byte( (uint8_t)cmd_READ_REGISTER );
        com_send_byte( args[i] );
    }

    com_send_byte( c_end_of_request );


    /* Check response */

    if( arg_len == 0 ) { com_read_byte( &out ); assert( out == (uint8_t)~cmd_READ_REGISTER ); }
    for( i = 0; i < arg_len; ++i )
    {
        com_read_byte( &out );
        assert( out == (uint8_t)~cmd_READ_REGISTER );

        com_read_byte( &out );
        assert( out == args[i] );
    }


    /* Stream data */
    while( s_state != 2 )
    {
        com_read_byte( &out );
        assert( out == c_response_frame_start );

        com_read_byte( &out );
        data_len = out;
        assert( data_len > 0 );

        if( !data ) data = malloc( data_len );
        data = malloc( data_len );

        /* I assume this blocks until this many bytes have come off the serial
         * port, and thus that this loop doesn't spin */
        com_read_bytes( data, data_len );

        cb( cb_ctxt, data, data_len );
    }

    com_send_byte( cmd_STOP );

    /* Read (hence don't return) to the end of this frame so that we're in a
     * sane state for the next request */
    do { com_read_byte( &out ); } while( out != c_end_of_response );


    return NULL;
}
