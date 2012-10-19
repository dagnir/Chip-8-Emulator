/* 
 * Copyright 2012 Dongie Agnir
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 * http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

 /*
  * Library to facilitate the disassembly of Chip-8 Binaries.
  */
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