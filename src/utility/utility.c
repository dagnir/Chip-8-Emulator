#include "utility.h"

char nibble(short word, unsigned int which) {
        short n = 0xF << (which * 4);
        return ((word & n) >> (which * 4)) & 0xFF;
}

char low_byte(short word) {
        return word & 0xFF;
}

short get_addr(short opcode) {
        return opcode & 0x0FFF;
}

size_t read_rom(const char *rom_file, char *data, size_t data_len) {
        FILE *rom = NULL;

        rom = fopen(rom_file, "rb");
        if (rom != NULL) {
                size_t size = fread((void*)data, sizeof(data), data_len, rom);
                fclose(rom);
                return size;
        } else {
                return -1;
        }
}