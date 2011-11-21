#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stddef.h>

#include "commands.h"
#include "constants.h"
#include "com/com.h"


typedef enum
{
    cmd_STOP = 0x30,
    cmd_READ_DTC = 0xd1,
    /* cmd_CLEAR_DTC = 0xc1; - This is not supported */
    cmd_READ_ECU_PART_NUMBER = 0xd0,
    cmd_READ_REGISTER = 0x5a,
    cmd_PEEK_MEMORY = 0xc9 /* ROM or RAM. */
    /* TODO */
} cmd_t;

static int read_frame_inner( cmd_t cmd,
                             uint8_t *args,  unsigned arg_len,
                             uint8_t **data, unsigned *data_len )
{
    unsigned i;
    uint8_t out;

    com_send_byte( (uint8_t)cmd );
    for( i = 0; i < arg_len; ++i ) com_send_byte( args[i] );
    com_send_byte( c_end_of_request );

    com_read_byte( &out );
    assert( out == (uint8_t)~cmd );
    for( i = 0; i < arg_len; ++i )
    {
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
    out = 0;
    do { com_read_byte( &out ); } while( out != c_end_of_response );


    return 0;
}

static int read_frame( cmd_t cmd,
                       uint8_t **data, unsigned *data_len )
{
    return read_frame_inner( cmd, NULL, 0, data, data_len );
}

static int read_frame_arg( cmd_t cmd,
                           uint8_t arg,
                           uint8_t **data, unsigned *data_len )
{
    return read_frame_inner( cmd, &arg, 1, data, data_len );
}

int read_dtc( fault_report_t **faults )
{
    unsigned len, i;
    uint8_t *data;
    fault_report_t *head = NULL, *item;

    read_frame( cmd_READ_DTC, &data, &len );
    assert( !(len % 2) );

    for( i = 0; i < len; i += 2 )
    {
        item = malloc( sizeof( fault_report_t ) );

        item->code = data[i];
        item->last_seen = data[i+1];

        item->next = head;
        head = item;
    }

    free( data );

    *faults = head;
    return 0;
}

void faults_free( fault_report_t *faults )
{
    fault_report_t *prev, *item;

    item = faults;
    while( item )
    {
        prev = item;
        item = item->next;
        free( prev );
    }
}

int read_ecu_part_no( ecu_part_no_t **part_no )
{
    unsigned len;
    uint8_t *data;

    read_frame( cmd_READ_ECU_PART_NUMBER, &data, &len );
    assert( len == 22 );

    *part_no = malloc( sizeof( ecu_part_no_t ) );
    /* FIXME: no no no, need to work out how these numbers come off the wire. */
    memcpy( *part_no + offsetof(ecu_part_no_t, part1),  data +  2, 2 );
    (*part_no)->part2 = 0x23710;
    memcpy( *part_no + offsetof(ecu_part_no_t, sw_ver), data + 19, 4 );

    free( data );

    return 0;
}

/* TODO: deal with addressing aircon computer */
int read_register( engine_reg_t reg, uint16_t *out )
{
    unsigned len;
    uint8_t *data;
    //read_frame( cmd_READ_REGISTER, reg, &data, &len );


    free( data );
    return 0;
}
