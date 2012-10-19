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
 * Some utility functions for working with opcodes, and reading ROM data.
 */
#ifndef __GUARD_UTILITY__
#define __GUARD_UTILITY__
#include <stdlib.h>

int read_rom(const char* rom_file, uint8_t *data, size_t data_size);
uint8_t nibble(uint16_t word, uint32_t which);
uint8_t low_byte(uint16_t word);
uint16_t get_addr(uint16_t opcode);

#endif /* __GUARD_UTILITY__ */