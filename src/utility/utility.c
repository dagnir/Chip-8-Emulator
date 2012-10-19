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

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "utility.h"

uint8_t nibble(uint16_t word, uint32_t which) {
        uint16_t n = (uint16_t)(0xF << (which * 4));
        return (uint8_t)(((word & n) >> (which * 4)) & 0xFF);
}

uint8_t low_byte(uint16_t word) {
        return (uint8_t)(word & 0xFF);
}

uint16_t get_addr(uint16_t opcode) {
        return opcode & 0x0FFF;
}

int read_rom(const char *rom_file, uint8_t *data, size_t data_len) {
        FILE *rom = NULL;

        rom = fopen(rom_file, "rb");
        if (rom != NULL) {
                size_t size = fread((void*)data, sizeof(uint8_t), data_len, rom);
                fclose(rom);
                printf("read_rom() read %d bytes.\n", size);
                return (int)size;
        } else {
                return -1;
        }
}