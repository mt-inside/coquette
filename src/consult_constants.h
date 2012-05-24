/* TODO: this should probably be less visible than this - most modules shouldn't
 * be directly communicating with the ECU.
 * TODO: a lot of the stuff in commands.[ch] should be in here too
 */
#ifndef _INCLUDED_CONSULT_CONSTANTS_H
#define _INCLUDED_CONSULT_CONSTANTS_H

#include <stdint.h>


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

typedef enum
{
    /* reg_engine_FLAGS0 */
    bit_TPS_CLOSED = 0x00,
    bit_CRANKING = 0x01,
    bit_NEUTRAL = 0x02,
    bit_POWER_STEERING = 0x03,
    bit_AC_ON_SWITCH = 0x04,
    /* 0x05 */
    /* 0x06 */
    /* 0x07 */

    /* reg_engine_FLAGS1 */
    bit_COOLANT_FAN_LO = 0x08,
    bit_COOLAND_FAN_HI = 0x09,
    /* 0x0a */
    /* 0x0b */
    /* 0x0c */
    bit_VTC_SOLENOID = 0x0d,
    bit_FUEL_PUMP_RELAY = 0x0e,
    bit_AIRCON_RELAY = 0x0f,

    /* reg_engine_FLAGS2 */
    bit_EGR_SOLENOID = 0x10,
    /* 0x11 */
    /* 0x13 */
    bit_IAVC_FICD_SOLENOID = 0x13,
    /* 0x14 */
    bit_WASTEGATE_SOLENOID = 0x15,
    bit_P_REG_CTRL_VALVE = 0x16,
    /* 0x17 */

    /* reg_engine_FLAGS3 */
    /* 0x18 */
    /* 0x19 */
    /* 0x1a */
    /* 0x1b */
    /* 0x1c */
    /* 0x1d */
    bit_RH_LEAN = 0x1e,
    bit_LH_LEAN = 0x1f
} engine_bit_t;

typedef enum
{
    reg_AMBIENT_TEMP = 0x20,
    reg_IN_CAR_HEAD = 0x21,
    reg_IN_CAR_FEET = 0x22,
    reg_DEF_DUCT = 0x23,
    reg_VENT_DUCT = 0x24,
    reg_FLOOR_DUCT = 0x25,
    reg_SUNLOAD = 0x26,
    reg_aircon_COOLANT_TEMP = 0x27,
    reg_MODE_DOOR_PBR = 0x28,
    reg_OBJ_HEAD = 0x29,
    reg_OBJ_FEET = 0x2a,
    reg_AIRMIX_DOOR1 = 0x2b,
    reg_AIRMIX_DOOR2 = 0x2c,
    reg_MODE_DOOR_ANGLE = 0x2d,
    reg_INTAKE_DOOR_ANGLE = 0x2e,
    reg_FAN_MOTOR = 0x2f,
    reg_SET_TMP_ADJ = 0x40,
    reg_COMPRESSOR_STATE = 0x41,
    reg_SET_TEMP = 0x42,
    reg_aircon_FLAGS0 = 0x43,
    reg_aircon_FLAGS1 = 0x47
} aircon_reg_t;

typedef enum
{
    bit_FAN_LO = 0x02,
    bit_FAN_HI = 0x03,
    bit_RECIRC = 0x04,
    bit_ECON = 0x05,
    bit_DEFROST = 0x06,
    bit_AUTO = 0x07,
    bit_DOWN_SWITCH = 0x08,
    bit_UP_SWITCH = 0x09
} aircon_bits_t;

typedef enum
{
    fault_NONE = 0x55
    /* TODO */
} fault_code_t;

extern const uint8_t c_end_of_request;
extern const uint8_t c_response_frame_start;
extern const uint8_t c_end_of_response;

extern const unsigned c_max_streaming_regs;

#endif /* defined _INCLUDED_CONSULT_CONSTANTS_H */
