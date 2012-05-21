#ifndef _INCLUDED_STREAM_H
#define _INCLUDED_STREAM_H

#include <stdarg.h>

#include "observers/observer_base.h"


typedef struct _stream_t stream_t;


extern stream_t *stream_new( unsigned num_observers, ... );
extern void stream_delete( stream_t *stream );

extern void stream_get_observers( stream_t *stream, observer_base_t ***observers, unsigned *len );

#endif /* defined _INCLUDED_STREAM_H */
