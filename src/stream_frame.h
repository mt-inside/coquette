#ifndef _INCLUDED_STREAM_FRAME_H
#define _INCLUDED_STREAM_FRAME_H

#include "registers.h"
#include "observers/observer.h"


typedef struct
{
    engine_reg_t reg;
    observer_t **observers;
    unsigned observers_len;
} stream_t;


extern int stream_registers_start( stream_t **streams, unsigned streams_len );
extern void stream_registers_end( void );

#endif /* defined _INCLUDED_STREAM_FRAME_H */
