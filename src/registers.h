#ifndef _INCLUDED_REGISTERS_H
#define _INCLUDED_REGISTERS_H

#include <stdint.h>

#include "scalers.h"


typedef enum
{
    reg_ENGINE_SPEED = 0x00,
    /* 0x01 */
    reg_RPM_REF = 0x02,
    /* 0x03 */
    reg_LH_MAF = 0x04,
    /* 0x05 */
    reg_RH_MAF = 0x06,
    /* 0x07 */
    reg_engine_COOLANT_TEMP = 0x08,
    reg_LH_O2 = 0x09,
    reg_RH_O2 = 0x0a,
    reg_ROAD_SPEED = 0x0b,
    reg_BATT_VOLT = 0x0c,
    reg_TPS = 0x0d,
    /* 0x0e */
    reg_FUEL_TEMP = 0x0f,
    /* 0x10 */
    reg_INTAKE_TEMP = 0x11,
    reg_EXHAUST_TEMP = 0x12,
    reg_engine_FLAGS0 = 0x13,
    reg_LH_INJ_PULSE_TIME = 0x14,
    /* 0x15 */
    reg_IGN_TIMING = 0x16,
    reg_IDLE_AIR_VALVE = 0x17,
    /* 0x18 */
    /* 0x19 */
    reg_LH_ALPHA = 0x1a,
    reg_RH_ALPHA = 0x1b,
    reg_LH_ALPHA_SELFLEARN = 0x1c,
    reg_RH_ALPHA_SELFLEARN = 0x1d,
    reg_engine_FLAGS1 = 0x1e,
    /* 0x1f */
    reg_engine_FLAGS2 = 0x1f,
    /* 0x20 */
    reg_engine_FLAGS3 = 0x21,
    reg_RH_INJ_PULSE_TIME = 0x22,
    /* 0x23 */
    /* 0x24 */
    reg_PURG_VOL_CTRL_VALVE = 0x25,
    reg_FUEL_TANK_TEMP = 0x26,
    reg_FPCM_DR_VOLT = 0x27,
    reg_WASTE_GATE_SOLENOID = 0x28,
    reg_BOOST = 0x29,
    reg_ENGINE_MOUNT = 0x2a,
    /* 0x2b */
    /* 0x2c */
    /* 0x2d */
    reg_POSITION_COUNTER = 0x2e,
    reg_FUEL_LEVEL = 0x2f,
    reg_LH_FR_O2_HEATER = 0x30,
    reg_RH_FR_O2_HEATER = 0x31,
    reg_IGN_SWITCH = 0x32,
    reg_CAL_LD = 0x33,
    reg_B_FUEL_SCHEDULE = 0x34,
    reg_LH_O2_SENSOR_VOLTS = 0x35,
    reg_RH_O2_SENSOR_VOLTS = 0x36,
    reg_TPS_ABS = 0x37,
    reg_MAF = 0x38,
    reg_EVAP_PRES = 0x39
} engine_reg_t;


typedef int (*reader_fn_t)( uint8_t *data ); /* FIXME: these shouldn't call COM, they should be passed something and munge it */


typedef struct
{
    unsigned width;
    reader_fn_t reader; /* Converts raw data into an int */
    scale_fn_t scaler;
} reg_info_t;


extern reg_info_t *registers_get_reg_info( engine_reg_t reg );

#endif /* defined _INCLUDED_REGISTERS_H */
