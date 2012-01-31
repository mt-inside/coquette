#include <assert.h>
#include <stdlib.h>

#include "registers.h"
#include "read_frame.h"


static int read_register_1byte( engine_reg_t reg, int *out );
static int read_register_2bytes( engine_reg_t reg, int *out );


/* TODO: add unit info */
static reg_info_t reg_infos[ ] =
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


reg_info_t *registers_get_reg_info( engine_reg_t reg )
{
    assert( reg < sizeof(reg_infos) / sizeof(reg_infos[0]) );

    return reg_infos + reg;
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
