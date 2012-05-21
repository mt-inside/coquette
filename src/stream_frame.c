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
#include "queue.h"


typedef struct _obs_list_t
{
    observer_base_t *obs;
    SLIST_ENTRY(_obs_list_t) next;
} obs_list_item_t;
typedef struct _obs_list_list_item_t
{
    engine_reg_t reg;
    SLIST_HEAD(, _obs_list_t) obs_list;
    STAILQ_ENTRY(_obs_list_list_item_t) next;
} obs_list_list_item_t;
STAILQ_HEAD(_obs_list_list_t, _obs_list_list_item_t);
typedef struct _obs_list_list_t obs_list_list_t;

typedef struct
{
    obs_list_list_t *streams;
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

typedef enum
{
    state_NOT_RUNNING,
    state_RUNNING,
    state_EXITING
} state_t;


/* TODO: lock around me */
static state_t s_state = state_NOT_RUNNING;
static pthread_t s_thread;


static void stream_cb( void *stream_cb_ctxt, uint8_t *data, unsigned data_len );
static void *stream_frame_thread( void *ctxt );


int stream_registers_start( stream_t *stream )
{
    stream_thread_args_t *thread_args =
        malloc( sizeof(stream_thread_args_t) );
    stream_cb_ctxt_t *cb_ctxt = malloc( sizeof(stream_cb_ctxt_t) );
    uint8_t *args = malloc( c_max_streaming_regs );
    reg_info_t *reg_info;
    unsigned i, j;
    unsigned args_offset = 0;
    obs_list_list_t *list_list = malloc( sizeof(obs_list_list_t) );
    obs_list_list_item_t *lli;
    obs_list_item_t *li;
    observer_base_t **observers;
    unsigned observers_len;


    assert( s_state == state_NOT_RUNNING );

    stream_get_observers( stream, &observers, &observers_len );

    /* Really crappy O(n^2) algorithm for putting this data structure together
     */
    STAILQ_INIT( list_list );
    for( i = 0; i < observers_len; ++i )
    {
        observer_base_t *obs = observers[i];
        engine_reg_t new_reg = observer_base_get_reg( obs );

        STAILQ_FOREACH(lli, list_list, next)
        {
            if( lli->reg == new_reg )
            {
                li = malloc( sizeof(obs_list_item_t) );
                SLIST_INSERT_HEAD(&lli->obs_list, li, next);

                li->obs = obs;

                break;
            }
        }
        if( !lli )
        {
            lli = malloc( sizeof(obs_list_list_item_t) );
            STAILQ_INSERT_TAIL(list_list, lli, next);

            SLIST_INIT( &lli->obs_list );
            lli->reg = new_reg;

            li = malloc( sizeof(obs_list_item_t) );
            SLIST_INSERT_HEAD(&lli->obs_list, li, next);
            li->obs = obs;

            reg_info = registers_get_reg_info( new_reg );
            assert( args_offset + reg_info->width <= c_max_streaming_regs );
            for( j = 0; j < reg_info->width; ++j )
            {
                args[args_offset++] = new_reg + j;
            }
        }
    }

    /* fixme make me in the thread function */
    cb_ctxt->streams  = list_list;
    cb_ctxt->data_len = args_offset;

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

    s_state = state_RUNNING;


    return 0;
}

void stream_registers_end( void )
{
    void *retval;

    assert( s_state == state_RUNNING );

    s_state = state_EXITING;
    pthread_join( s_thread, &retval );

    s_state = state_NOT_RUNNING;
}


static void stream_cb( void *stream_cb_ctxt, uint8_t *data, unsigned data_len )
{
    static int first_time = 1;
    static uint8_t *old_data;
    static unsigned old_data_len;

    stream_cb_ctxt_t *ctxt = (stream_cb_ctxt_t *)stream_cb_ctxt;
    obs_list_list_t *list_list = ctxt->streams;
    obs_list_list_item_t *lli;
    obs_list_item_t *li;
    unsigned offset;
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
    STAILQ_FOREACH( lli, list_list, next)
    {
        reg_info_t *reg_info = registers_get_reg_info( lli->reg );

        datum = reg_info->reader( data + offset );
        datum = reg_info->scaler( datum );

        offset += reg_info->width;

        SLIST_FOREACH( li, &lli->obs_list, next)
        {
            observer_base_update( li->obs, datum );
        }
    }
}

static void cb_ctxt_free( stream_cb_ctxt_t *ctxt )
{
    obs_list_list_item_t *lli, *llit;
    obs_list_item_t *li, *lit;

    STAILQ_FOREACH_SAFE(lli, ctxt->streams, next, llit)
    {
        SLIST_FOREACH_SAFE(li, &lli->obs_list, next, lit)
        {
            free( li );
        }
        free( lli );
    }
    free( ctxt->streams );
    free( ctxt );
}
static void thread_args_free( stream_thread_args_t *ta )
{
    cb_ctxt_free( (stream_cb_ctxt_t *)ta->cb_ctxt );
    free( ta->regs );
    free( ta );
}

static void *stream_frame_thread( void *ctxt )
{
    stream_thread_args_t *thread_args = (stream_thread_args_t *)ctxt;
    uint8_t *args    = thread_args->regs;
    unsigned arg_len = thread_args->regs_len;
    stream_cb_t cb   = thread_args->cb;
    void *cb_ctxt    = thread_args->cb_ctxt;

    unsigned i;
    uint8_t *data = malloc( arg_len );
    uint8_t out;


    assert( arg_len > 0 && arg_len <= c_max_streaming_regs );

    /* Send command and arguments */
    for( i = 0; i < arg_len; ++i )
    {
        com_send_byte( (uint8_t)cmd_READ_REGISTER );
        com_send_byte( args[i] );
    }

    com_send_byte( c_end_of_request );


    /* Check response */
    for( i = 0; i < arg_len; ++i )
    {
        com_read_byte( &out );
        assert( out == (uint8_t)~cmd_READ_REGISTER );

        com_read_byte( &out );
        assert( out == args[i] );
    }


    /* Stream data */
    while( s_state != state_EXITING )
    {
        assert( s_state == state_RUNNING );

        com_read_byte( &out );
        assert( out == c_response_frame_start );

        com_read_byte( &out );
        assert( out == arg_len );

        /* I assume this blocks until this many bytes have come off the serial
         * port, and thus that this loop doesn't spin */
        com_read_bytes( data, arg_len );

        cb( cb_ctxt, data, arg_len );
    }

    com_send_byte( cmd_STOP );

    /* Read (hence don't return) to the end of this frame so that we're in a
     * sane state for the next request */
    do { com_read_byte( &out ); } while( out != c_end_of_response );


    thread_args_free( thread_args );
    free( data );

    return NULL;
}
