#ifndef __CHIP8_DISASM__
#define __CHIP8_DISASM__
#include <stdlib.h>

/*
 * Main function that disassembles the ROM code in rom_file, and writes it to
 * out_put.
 */
void chip8disasm(const char *rom_file, const char* output);

#endif //__CHIP8_DISASM__