#ifndef _INCLUDED_REGISTERS_H
#define _INCLUDED_REGISTERS_H

#include "commands.h"
#include "scalers.h"


typedef int (*reader_fn_t)( engine_reg_t reg, int *out );

typedef struct
{
    reader_fn_t reader;
    scale_fn_t scaler;
} reg_info_t;


extern reg_info_t *registers_get_reg_info( engine_reg_t reg );

#endif /* defined _INCLUDED_REGISTERS_H */
