#include <stdint.h>
#include <stdlib.h>

#include "utils.h"


void *map( void *in, unsigned in_stride, unsigned out_stride, unsigned count, mapper_fn_t mapper )
{
    void *out = malloc( count * out_stride );
    unsigned i;

    for( i = 0; i < count; ++i )
    {
        mapper( (uint8_t *)in + i * in_stride, (uint8_t *)out + i * out_stride );
    }

    return out;
}
