#include <stdio.h>
#include <stdlib.h>

#include "chip8disasm.h"

void chip8disasm(const char* rom_file, const char* output) {
        char rom_data[4096];
        size_t rom_size = read_rom(rom_file, &(rom_data[0]), 4096);

        FILE *output_file = fopen(output, "w");

        if (rom_size != -1 && output_file != NULL) {
                printf("%d bytes read.\n", rom_size);

                //ROM loaded, time to disassemble
                for (size_t i = 0; i < rom_size; i += 2) {
                        fprintf(output_file, "%x: ", i);
                        short opcode = (short)rom_data[i] << 8;
                        opcode |= rom_data[i + 1] & 0xFF;
                        //printf("opcode: %x\n", opcode);

                        switch (opcode & 0xF000) {
                        case 0x0000: 
                                if (opcode == 0x00E0) { //CLS
                                        fprintf(output_file, "CLS");
                                } else if (opcode == 0x00EE) { //RET
                                        fprintf(output_file, "RET");
                                } else { //SYS
                                        fprintf(output_file, "SYS %x", 
                                                get_addr(opcode));
                                }
                                break;
                        case 0x1000: //JP addr
                                fprintf(output_file, "JP %x", 
                                        get_addr(opcode));
                                break;
                        case 0x2000: //CALL addr
                                fprintf(output_file, "CALL %x", 
                                        get_addr(opcode));
                                break;
                        case 0x3000: { //SE Vx, byte
                                char x = nibble(opcode, 2);
                                char kk = low_byte(opcode);
                                fprintf(output_file, "SE V%x, %x", x, kk);
                                break;
                        }
                        case 0x4000: { //SNE Vx, byte
                                char x = nibble(opcode, 2);
                                char kk = low_byte(opcode);
                                fprintf(output_file, "SNE, V%x, %x", x, kk);
                                break;
                        }
                        case 0x5000: { //SE Vx, Vy
                                char x = nibble(opcode, 2);
                                char y = nibble(opcode, 1);
                                fprintf(output_file, "SE V%x, V%x", x, y);
                                break;
                        }
                        case 0x6000: { //LD Vx, byte
                                char x = nibble(opcode, 2);
                                char kk = low_byte(opcode);
                                fprintf(output_file, "LD V%x, %x", x, kk);
                                break;
                        }
                        case 0x7000: { //ADD Vx, byte
                                char x = nibble(opcode, 2);
                                char kk = low_byte(opcode);
                                fprintf(output_file, "ADD V%x, %x", x, kk);
                                break;
                        }
                        case 0x8000: { //Group of INSTRUCTIONS
                                char x = nibble(opcode, 2);
                                char y = nibble(opcode, 1);
                                char ins = nibble(opcode, 0);
                                switch (ins & 0xF) {
                                case 0x0: //LD Vx, Vy
                                        fprintf(output_file, "LD V%x, V%x", x, 
                                                                        y);
                                        break;
                                case 0x1: //OR Vx, Vy
                                        fprintf(output_file, "OR V%x, V%x", x, 
                                                                        y);
                                        break;
                                case 0x2: //AND Vx, Vy
                                        fprintf(output_file, "AND V%x, V%x", x, 
                                                                        y);
                                        break;
                                case 0x3: //XOR Vx, Vy
                                        fprintf(output_file, "XOR V%x, V%x", x, 
                                                                        y);
                                        break;
                                case 0x4: //ADD Vx, Vy
                                        fprintf(output_file, "ADD V%x, V%x", x, 
                                                                        y);
                                        break;
                                case 0x5: //SUB Vx, Vy
                                        fprintf(output_file, "SUB V%x, V%x", x, 
                                                                        y);
                                        break;
                                case 0x6: //SHR Vx {, Vy}
                                        fprintf(output_file, "SHR V%x", x);
                                        break;
                                case 0x7: //SUBN Vx, Vy
                                        fprintf(output_file, "SUBN V%x, V%x", 
                                                                        x, y);
                                        break;
                                case 0xE: //SHL Vx {, Vy}
                                        fprintf(output_file, "SHL V%x", x);
                                        break;
                                }
                                break;
                        }
                        case 0xA000: { //LDI I, addr
                                short addr = get_addr(opcode);
                                fprintf(output_file, "LDI I, %x", addr);
                                break;
                        }
                        case 0xD000: { //DRW Vx, Vy, nibble
                                char x = nibble(opcode, 2);
                                char y = nibble(opcode, 1);
                                char n = nibble(opcode, 0);
                                fprintf(output_file, "DRW V%x, V%x, %x", x, y, 
                                                                        n);
                                break;
                        }
                        default:
                                fprintf(output_file, "UNKNOWN");
                                break;
                        }
                        fprintf(output_file, "\n");
                }

                fclose(output_file);
        } else {
                perror(NULL);
                printf("Could not open %s.\n", rom_file);
        }
}