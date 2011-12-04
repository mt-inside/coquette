#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stddef.h>
#include <stdarg.h>

#include "commands.h"
#include "scalers.h"
#include "constants.h"
#include "com/com.h"


const uint8_t *handshakes[] =
{
    c_handshake_ecu,
    c_handshake_at,
    c_handshake_hicas,
    c_handshake_aircon
};

typedef enum
{
    /* cmd_ACTIVATION = 0x0a, - Writing is not supported! */
    cmd_STOP = 0x30,
    cmd_READ_REGISTER = 0x5a,
    /* cmd_CLEAR_DTC = 0xc1, - Writing is not supported! */
    cmd_PEEK_MEMORY = 0xc9, /* ROM or RAM. */
    cmd_READ_ECU_PART_NUMBER = 0xd0,
    cmd_READ_DTC = 0xd1
} cmd_t;

typedef int (*scale_fn_t)( int in );
typedef int (*reader_fn_t)( engine_reg_t reg, int *out );
typedef struct
{
    reader_fn_t reader;
    scale_fn_t scaler;
} reg_info_t;

static int read_register_1byte(  engine_reg_t reg, int *out );
static int read_register_2bytes( engine_reg_t reg, int *out );

/* TODO: add unit info */
reg_info_t reg_infos[ ] =
{
    { &read_register_2bytes, &scale_tacho },
    /* 0x01 */ { NULL, NULL },
    { &read_register_2bytes, &scale_rpm_ref },
    /* 0x03 */ { NULL, NULL },
    { &read_register_2bytes, &scale_maf },
    /* 0x05 */ { NULL, NULL },
    { &read_register_2bytes, &scale_maf },
    /* 0x07 */ { NULL, NULL },
    { &read_register_1byte,  &scale_temp },
    { &read_register_1byte,  &scale_o2 },
    { &read_register_1byte,  &scale_o2 },
    { &read_register_1byte,  &scale_road_speed },
    { &read_register_1byte,  &scale_batt_volt },
    { &read_register_1byte,  &scale_tps },
    /* 0x07 */ { NULL, NULL },
    { &read_register_1byte,  &scale_temp },
    /* 0x10 */ { NULL, NULL },
    { &read_register_1byte,  &scale_temp },
    { &read_register_1byte,  &scale_egt },
    { &read_register_1byte,  &scale_id },
    { &read_register_2bytes, &scale_inj_time },
    /* 0x15 */ { NULL, NULL },
    { &read_register_1byte,  &scale_ign_time },
    { &read_register_1byte,  &scale_aac_valve },
    /* 0x18 */ { NULL, NULL },
    /* 0x19 */ { NULL, NULL },
    { &read_register_1byte,  &scale_id },
    { &read_register_1byte,  &scale_id },
    { &read_register_1byte,  &scale_id },
    { &read_register_1byte,  &scale_id },
    { &read_register_1byte,  &scale_id },
    /* 0x1f */ { NULL, NULL },
    { &read_register_1byte,  &scale_id },
    /* 0x20 */ { NULL, NULL },
    { &read_register_1byte,  &scale_id },
    { &read_register_2bytes, &scale_inj_time },
    /* 0x23 */ { NULL, NULL },
    /* 0x24 */ { NULL, NULL },
    { &read_register_1byte,  &scale_id },
    { &read_register_1byte,  &scale_id },
    { &read_register_1byte,  &scale_id },
    { &read_register_1byte,  &scale_id },
    { &read_register_1byte,  &scale_id },
    { &read_register_1byte,  &scale_id },
    /* 0x2b */ { NULL, NULL },
    /* 0x2c */ { NULL, NULL },
    /* 0x2d */ { NULL, NULL },
    { &read_register_1byte,  &scale_id },
    { &read_register_1byte,  &scale_id },
    { &read_register_1byte,  &scale_id },
    { &read_register_1byte,  &scale_id },
    { &read_register_1byte,  &scale_id },
    { &read_register_1byte,  &scale_id },
    { &read_register_1byte,  &scale_id },
    { &read_register_1byte,  &scale_id },
    { &read_register_1byte,  &scale_id },
    { &read_register_1byte,  &scale_id },
    { &read_register_1byte,  &scale_id },
    { &read_register_1byte,  &scale_id }
};


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
static int read_frame_args( cmd_t cmd,
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

int handshake( ecu_t ecu )
{
    unsigned i;
    const uint8_t *handshake = handshakes[ecu];
    uint8_t out;

    for( i = 0; i < 3; ++i )
    {
        com_send_byte( handshake[i] );
    }

    com_read_byte( &out );
    assert( out == c_handshake_response );

    return 0;
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

    /*int i;
    for( i = 0; i < 22; ++i )
    {
        printf( "trosl %u: %hhx\n", i, data[ i ] );
    }*/

    *part_no = malloc( sizeof( ecu_part_no_t ) );

    memcpy( &((*part_no)->part1),  data +  1, 2 );
    (*part_no)->part2 = 0x23710;
    memcpy( &((*part_no)->sw_ver), data + 18, 4 );

    free( data );

    return 0;
}

/* TODO: deal with addressing aircon computer */
int read_register( engine_reg_t reg, int *out )
{
    reg_info_t reg_info;

    assert( reg < sizeof(reg_infos) / sizeof(reg_infos[0]) );

    reg_info = reg_infos[ reg ];

    reg_info.reader( reg, out );
    *out = reg_info.scaler( *out );

    return 0;
}

engine_reg_t engine_bit_regs[] = { reg_engine_FLAGS0, reg_engine_FLAGS1, reg_engine_FLAGS2, reg_engine_FLAGS3 };
int read_flag( engine_bit_t flag, int *out )
{
    int data;
    engine_reg_t reg = engine_bit_regs[ flag / 8 ];
    unsigned offset = flag % 8;

    read_register( reg, &data );

    *out = ( data & ( 1 << offset ) ) >> offset;

    return 0;
}

static int read_register_1byte( engine_reg_t reg, int *out )
{
    unsigned len;
    uint8_t *data;

    read_frame_arg( cmd_READ_REGISTER, reg, &data, &len );
    assert( len == 1 );

    *out = *data;

    free( data );
    return 0;
}

static int read_register_2bytes( engine_reg_t reg, int *out )
{
    unsigned len;
    uint8_t *data;

    /* assume both registers are adjacent */
    read_frame_args( cmd_READ_REGISTER, &data, &len, 2, reg, reg + 1 );
    assert( len == 2 );

    *out = data[1] | data[0] << 8;

    free( data );
    return 0;
}

int peek_memory( uint16_t addr, uint8_t *out )
{
    unsigned len;
    uint8_t *data;

    assert( addr >= c_memory_start && addr <= c_memory_end );

    read_frame_args( cmd_PEEK_MEMORY, &data, &len, 2, (addr & 0xff00) >> 8, addr & 0x00ff );
    assert( len == 1 );

    *out = *data;

    free( data );
    return 0;
}
