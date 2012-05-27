#ifndef _INCLUDED_REGISTERS_H
#define _INCLUDED_REGISTERS_H

#include <stdint.h>

#include "consult_constants.h"
#include "scalers.h"


typedef int (*reader_fn_t)( uint8_t *data );


typedef struct
{
    unsigned width;
    reader_fn_t reader; /* Converts raw data into an int */
    scale_fn_t scaler;
} reg_info_t;


extern reg_info_t *registers_get_reg_info( engine_reg_t reg );

extern void engine_flag_to_ordinates( engine_flag_t bit, engine_reg_t *reg, unsigned *offset );


extern unsigned c_num_engine_flags_registers;

#endif /* defined _INCLUDED_REGISTERS_H */
