#ifndef __GUARD_UTILITY__
#define __GUARD_UTILITY__
#include <stdlib.h>

int read_rom(const char* rom_file, uint8_t *data, size_t data_size);
uint8_t nibble(uint16_t word, uint32_t which);
uint8_t low_byte(uint16_t word);
uint16_t get_addr(uint16_t opcode);

#endif //__GUARD_UTILITY__