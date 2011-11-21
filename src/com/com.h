#include <stdint.h>

extern int com_init( char *tty_dev_name );
extern int com_finalise( void );

extern int com_send_byte( uint8_t byte );
extern int com_send_bytes( uint8_t *bytes, unsigned count );

extern int com_read_byte( uint8_t *byte );
extern int com_read_bytes( uint8_t *bytes, unsigned count );
