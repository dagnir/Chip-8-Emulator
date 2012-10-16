#ifndef __GUARD_UTILITY__
#define __GUARD_UTILITY__
#include <stdlib.h>

int read_rom(const char* rom_file, char *data, size_t data_size);
char nibble(short word, unsigned int which);
char low_byte(short word);
short get_addr(short opcode);

#endif //__GUARD_UTILITY__