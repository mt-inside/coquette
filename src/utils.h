#ifndef _INCLUDED_UTILS_H
#define _INCLUDED_UTILS_H

#include "sys/time.h"

typedef void (*mapper_fn_t)( void *in, void *out );
extern void *map( void *in, unsigned in_stride, unsigned out_stride, unsigned count, mapper_fn_t mapper );

/* 32bit int can hold +/- 2k us, or about half an hour */
extern int timeval_to_usec( struct timeval *tv );
/* 32bit int can hold +/- 2M ms, or about 23 days */
extern int timeval_to_msec( struct timeval *tv );

#define MAX(x,y) (((x) > (y)) ? (x) : (y))
#define MIN(x,y) (((x) < (y)) ? (x) : (y))

#endif /* defined _INCLUDED_UTILS_H */
