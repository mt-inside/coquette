#ifndef _INCLUDED_COMMANDS_H
#define _INCLUDED_COMMANDS_H

#include <stdint.h>

#include "consult_constants.h"


typedef enum
{
    ecu_ENGINE,
    ecu_AT,
    ecu_HICAS,
    ecu_AIRCON
} ecu_t;

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
