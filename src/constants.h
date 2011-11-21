#include <stdint.h>

const unsigned c_baud_rate = 9600;

const uint8_t c_handshake_host[] = { 0xff, 0xff, 0xef };
const uint8_t c_handshake_ecu = 0x10;

const uint8_t c_end_of_request = 0xf0;
const uint8_t c_response_frame_start = 0xff;
const uint8_t c_end_of_response = 0xcf;

const uint16_t c_memory_start = 0x8000;
const uint16_t c_memory_end = 0xffff;
