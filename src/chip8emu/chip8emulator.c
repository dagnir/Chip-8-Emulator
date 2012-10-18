#include <stdio.h>

#include "chip8emu.h"

int main(int argc, char** argv) {
        chip8emu_init();

        uint8_t test_rom[] = {
                0xFF, 0x65,
                0x12, 0x02
        };

        chip8emu_load_rom(test_rom, 4);

        chip8emu_begin_emulate();

        debug_print_registers();
        /*
        printf("ram[1] is %x\n", chip8.ram[0x1]);
        struct _instruction ins = parse_opcode(0xF555);
        printf("size of ins is %u\n", sizeof(ins));
        printf("instruction is %u, x: %u", ins.ins, ins.x);
        */
        return 0;
}

