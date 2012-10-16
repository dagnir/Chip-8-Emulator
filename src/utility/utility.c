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