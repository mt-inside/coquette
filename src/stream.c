#include "common.h"

#include <stdlib.h>

#include "stream.h"
#include "observers/observer_base.h"


struct _stream_t
{
    observer_base_t **observers;
    unsigned observers_len;
};


stream_t *stream_new( unsigned num_observers, ... )
{
    stream_t *stream = malloc( sizeof(stream_t) );
    va_list ap;
    unsigned i;


    stream->observers_len = num_observers;
    stream->observers = malloc( sizeof(observer_base_t *) * stream->observers_len );

    va_start( ap, num_observers );
    for( i = 0; i < num_observers; i++ )
    {
        stream->observers[i] = va_arg( ap, observer_base_t * );
    }
    va_end( ap );


    return stream;
}

void stream_delete( stream_t *stream )
{
    unsigned i;

    for( i = 0; i < stream->observers_len; i++ )
    {
        observer_base_delete( stream->observers[i] );
    }

    free( stream->observers );
    free( stream );
}

void stream_get_observers( stream_t *stream, observer_base_t ***observers, unsigned *len )
{
    *observers = stream->observers;
    *len = stream->observers_len;
}
