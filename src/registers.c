#include <assert.h>
#include <stdlib.h>

#include "registers.h"


static int read_register_1byte(  uint8_t *data );
static int read_register_2bytes( uint8_t *data );


/* TODO: add unit info */
static reg_info_t reg_infos[ ] =
{
    { 2, &read_register_2bytes, &scale_engine_speed },
    /* 0x01 */ { 0, NULL, NULL },
    { 2, &read_register_2bytes, &scale_rpm_ref },
    /* 0x03 */ { 0, NULL, NULL },
    { 2, &read_register_2bytes, &scale_maf },
    /* 0x05 */ { 0, NULL, NULL },
    { 2, &read_register_2bytes, &scale_maf },
    /* 0x07 */ { 0, NULL, NULL },
    { 1, &read_register_1byte,  &scale_temp },
    { 1, &read_register_1byte,  &scale_o2 },
    { 1, &read_register_1byte,  &scale_o2 },
    { 1, &read_register_1byte,  &scale_road_speed },
    { 1, &read_register_1byte,  &scale_batt_volt },
    { 1, &read_register_1byte,  &scale_tps },
    /* 0x07 */ { 0, NULL, NULL },
    { 1, &read_register_1byte,  &scale_temp },
    /* 0x10 */ { 0, NULL, NULL },
    { 1, &read_register_1byte,  &scale_temp },
    { 1, &read_register_1byte,  &scale_egt },
    { 1, &read_register_1byte,  &scale_id },
    { 2, &read_register_2bytes, &scale_inj_time },
    /* 0x15 */ { 0, NULL, NULL },
    { 1, &read_register_1byte,  &scale_ign_time },
    { 1, &read_register_1byte,  &scale_aac_valve },
    /* 0x18 */ { 0, NULL, NULL },
    /* 0x19 */ { 0, NULL, NULL },
    { 1, &read_register_1byte,  &scale_id },
    { 1, &read_register_1byte,  &scale_id },
    { 1, &read_register_1byte,  &scale_id },
    { 1, &read_register_1byte,  &scale_id },
    { 1, &read_register_1byte,  &scale_id },
    /* 0x1f */ { 0, NULL, NULL },
    { 1, &read_register_1byte,  &scale_id },
    /* 0x20 */ { 0, NULL, NULL },
    { 1, &read_register_1byte,  &scale_id },
    { 2, &read_register_2bytes, &scale_inj_time },
    /* 0x23 */ { 0, NULL, NULL },
    /* 0x24 */ { 0, NULL, NULL },
    { 1, &read_register_1byte,  &scale_id },
    { 1, &read_register_1byte,  &scale_id },
    { 1, &read_register_1byte,  &scale_id },
    { 1, &read_register_1byte,  &scale_id },
    { 1, &read_register_1byte,  &scale_id },
    { 1, &read_register_1byte,  &scale_id },
    /* 0x2b */ { 0, NULL, NULL },
    /* 0x2c */ { 0, NULL, NULL },
    /* 0x2d */ { 0, NULL, NULL },
    { 1, &read_register_1byte,  &scale_id },
    { 1, &read_register_1byte,  &scale_id },
    { 1, &read_register_1byte,  &scale_id },
    { 1, &read_register_1byte,  &scale_id },
    { 1, &read_register_1byte,  &scale_id },
    { 1, &read_register_1byte,  &scale_id },
    { 1, &read_register_1byte,  &scale_id },
    { 1, &read_register_1byte,  &scale_id },
    { 1, &read_register_1byte,  &scale_id },
    { 1, &read_register_1byte,  &scale_id },
    { 1, &read_register_1byte,  &scale_id },
    { 1, &read_register_1byte,  &scale_id }
};

static engine_reg_t engine_bit_regs[] = { reg_engine_FLAGS0, reg_engine_FLAGS1, reg_engine_FLAGS2, reg_engine_FLAGS3 };
unsigned c_num_engine_flags_registers = sizeof(engine_bit_regs) / sizeof(engine_bit_regs[0]);


reg_info_t *registers_get_reg_info( engine_reg_t reg )
{
    assert( reg < sizeof(reg_infos) / sizeof(reg_infos[0]) );

    return reg_infos + reg;
}

void engine_flag_to_ordinates( engine_bit_t flag, engine_reg_t *reg, unsigned *offset )
{
    *reg = engine_bit_regs[ flag / 8 ];
    *offset = flag % 8;
}


static int read_register_1byte( uint8_t *data )
{
    return *data;
}

static int read_register_2bytes( uint8_t *data )
{
    return data[0] << 8 | data[1];
}
