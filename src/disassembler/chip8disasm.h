#ifndef __CHIP8_DISASM__
#define __CHIP8_DISASM__
#include <stdlib.h>
#include <stdint.h>

/*
 * Main function that disassembles the ROM code in rom_file, and writes it to
 * out_put.
 */
void chip8disasm(const char *rom_file, const char* output);

/*
 * Identifies which instruction this opcode belongs to.  Should emit a number
 * in the range [0, 35], inclusive.  See the Wikipedia for the list of 
 * instructions.
 */
int identify_ins(uint16_t opcode);

#endif /* __CHIP8_DISASM__ */