#include <stdlib.h>
#include <stdio.h>

#include "utility.h"

char nibble(short word, unsigned int which) {
        short n = (short)(0xF << (which * 4));
        return (char)(((word & n) >> (which * 4)) & 0xFF);
}

char low_byte(short word) {
        return (char)(word & 0xFF);
}

short get_addr(short opcode) {
        return opcode & 0x0FFF;
}

int read_rom(const char *rom_file, char *data, size_t data_len) {
        FILE *rom = NULL;

        rom = fopen(rom_file, "rb");
        if (rom != NULL) {
                size_t size = fread((void*)data, sizeof(char), data_len, rom);
                fclose(rom);
                printf("read_rom() read %d bytes.\n", size);
                return (int)size;
        } else {
                return -1;
        }
}