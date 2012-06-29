#include <unistd.h>
#include <stdint.h>

typedef ssize_t (*com_wrapper_t)( void *, size_t );

extern int com_read_write_wrapper(
    uint8_t *buf, unsigned count, com_wrapper_t fn
);
