#ifndef _INCLUDED_UTILS_H
#define _INCLUDED_UTILS_H

typedef void (*mapper_fn_t)( void *in, void *out );

extern void *map( void *in, unsigned in_stride, unsigned out_stride, unsigned count, mapper_fn_t mapper );

#endif /* defined _INCLUDED_UTILS_H */
