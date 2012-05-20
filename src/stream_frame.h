#ifndef _INCLUDED_STREAM_FRAME_H
#define _INCLUDED_STREAM_FRAME_H

#include "registers.h"
#include "observers/observer_base.h"


typedef struct
{
    observer_base_t **observers;
    unsigned observers_len;
} stream_t;


extern int stream_registers_start( stream_t *stream );
extern void stream_registers_end( void );

#endif /* defined _INCLUDED_STREAM_FRAME_H */
