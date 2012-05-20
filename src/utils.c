#include <stdint.h>
#include <stdlib.h>

#include "utils.h"


void *map( void *in, unsigned in_stride, unsigned out_stride, unsigned count, mapper_fn_t mapper )
{
    void *out = malloc( count * out_stride );
    unsigned i;

    for( i = 0; i < count; ++i )
    {
        mapper( (char *)in + i * in_stride, (char *)out + i * out_stride );
    }

    return out;
}

int timeval_to_usec( struct timeval *tv )
{
    return (tv->tv_sec * 1000000) + tv->tv_usec;
}

int timeval_to_msec( struct timeval *tv )
{
    return (tv->tv_sec * 1000) + (tv->tv_usec / 1000);
}
