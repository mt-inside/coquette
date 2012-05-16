#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stddef.h>

#include "commands.h"
#include "registers.h"
#include "read_frame.h"
#include "com/com.h"


const unsigned c_baud_rate = 9600;

const uint8_t c_handshake_ecu[] =     { 0xff, 0xff, 0xef };
const uint8_t c_handshake_at[]  =     { 0xff, 0xff, 0xea };
const uint8_t c_handshake_hicas[]  =  { 0xff, 0xff, 0xe4 };
const uint8_t c_handshake_aircon[]  = { 0xff, 0xff, 0xdf };
const uint8_t c_handshake_response = 0x10;

const uint16_t c_memory_start = 0x8000;
const uint16_t c_memory_end = 0xffff;

const uint8_t *handshakes[] =
{
    c_handshake_ecu,
    c_handshake_at,
    c_handshake_hicas,
    c_handshake_aircon
};


int handshake( ecu_t ecu )
{
    unsigned i;
    const uint8_t *handshake = handshakes[ecu];
    uint8_t out;

    for( i = 0; i < 3; ++i )
    {
        assert( com_send_byte( handshake[i] ) );
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

    *part_no = malloc( sizeof( ecu_part_no_t ) );

    memcpy( &((*part_no)->part1),  data + 1, 2 );

    (*part_no)->part2 = 0x23710;

    memcpy( &((*part_no)->sw_ver), data + 17, 5 );
    (*part_no)->sw_ver[5] = '\0';

    free( data );

    return 0;
}

/* TODO: deal with addressing aircon computer */
int read_register( engine_reg_t reg, int *out )
{
    reg_info_t *reg_info = registers_get_reg_info( reg );

    assert( reg_info );

    reg_info->reader( reg, out );
    *out = reg_info->scaler( *out );

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
