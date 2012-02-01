#ifndef _INCLUDED_COMMANDS_H
#define _INCLUDED_COMMANDS_H

#include <stdint.h>

#include "registers.h"


typedef enum
{
    ecu_ENGINE,
    ecu_AT,
    ecu_HICAS,
    ecu_AIRCON
} ecu_t;

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
    bit_TPS_CLOSED = 0x00,
    bit_CRANKING = 0x01,
    bit_NEUTRAL = 0x02,
    bit_POWER_STEERING = 0x03,
    bit_AC_ON_SWITCH = 0x04,
    bit_COOLANT_FAN_LO = 0x08,
    bit_COOLAND_FAN_HI = 0x09,
    bit_VTC_SOLENOID = 0x0d,
    bit_FUEL_PUMP_RELAY = 0x0e,
    bit_AIRCON_RELAY = 0x0f,
    bit_EGR_SOLENOID = 0x10,
    bit_IAVC_FICD_SOLENOID = 0x13,
    bit_WASTEGATE_SOLENOID = 0x15,
    bit_P_REG_CTRL_VALVE = 0x16,
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

typedef struct _fault_report_t
{
    fault_code_t code;
    unsigned last_seen;
    struct _fault_report_t* next;
} fault_report_t;

typedef struct
{
    uint16_t part1;
    uint32_t part2;
    char sw_ver[6];
} ecu_part_no_t;


extern int handshake( ecu_t ecu );
extern int read_dtc( fault_report_t **faults );
extern void faults_free( fault_report_t *faults );
extern int read_ecu_part_no( ecu_part_no_t **part_no );
extern int read_register( engine_reg_t reg, int *out );
extern int read_flag( engine_bit_t flag, int *out );
extern int peek_memory( uint16_t addr, uint8_t *out );

#endif /* defined _INCLUDED_COMMANDS_H */
