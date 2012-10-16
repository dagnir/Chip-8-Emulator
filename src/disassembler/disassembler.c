#include <stdio.h>

#include "chip8disasm.h"

int main(int argc, char* argv[]) {
        if(argc > 2) {
                printf("Disassembling ROM file: %s\n"
                        "Saving disassembly to: %s.\n", argv[1], argv[2]);
                
                chip8disasm(argv[1], argv[2]);
        } else {
                printf("User must supply rom file and output file.\n");
        }

        return 0;
}