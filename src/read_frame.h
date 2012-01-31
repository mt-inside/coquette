#ifndef _INCLUDED_READ_FRAME_H
#define _INCLUDED_READ_FRAME_H

#include "commands.h"


extern int read_frame( cmd_t cmd,
                       uint8_t **data, unsigned *data_len );

extern int read_frame_arg( cmd_t cmd,
                           uint8_t arg,
                           uint8_t **data, unsigned *data_len );

extern int read_frame_args( cmd_t cmd,
                            uint8_t **data, unsigned *data_len,
                            unsigned args_len, ... );

#endif /* defined _INCLUDED_READ_FRAME_H */
