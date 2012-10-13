#include <stdio.h>
#include <stdlib.h>

#include "chip8disasm.h"
#include "opcodes.h"

int identify_ins(short opcode) {

        switch (opcode & 0xF000) {
        case 0x0000: 
                if (opcode == 0x00E0) { //CLS
                        return CLS;
                } else if (opcode == 0x00EE) { //RET
                        return RET;
                } else { //SYS
                        return SYS;
                }
        case 0x1000: //JP addr
                return JP_addr;
        case 0x2000: //CALL addr
                return CALL_addr;
        case 0x3000: //SE Vx, byte
                return SE_Vx_byte;
        case 0x4000: //SNE Vx, byte
                return SNE_Vx_byte;
        case 0x5000: //SE Vx, Vy
                return SE_Vx_Vy;
        case 0x6000: //LD Vx, byte
                return LD_Vx_byte;
        case 0x7000: //ADD Vx, byte
                return ADD_Vx_byte;
        case 0x8000: //Group of INSTRUCTIONS
                switch (ins & 0xF) {
                case 0x0: //LD Vx, Vy
                        return LD_Vx_Vy;
                case 0x1: //OR Vx, Vy
                        return OR_Vx_Vy;
                case 0x2: //AND Vx, Vy
                        return AND_Vx_Vy;
                case 0x3: //XOR Vx, Vy
                        return XOR_Vx_Vy;
                case 0x4: //ADD Vx, Vy
                        return ADD_Vx_Vy;
                case 0x5: //SUB Vx, Vy
                        return SUB_Vx_Vy;
                case 0x6: //SHR Vx {, Vy}
                        return SHR_Vx;
                case 0x7: //SUBN Vx, Vy
                        return SUBN_Vx_Vy;
                case 0xE: //SHL Vx {, Vy}
                        return SHL_Vx;
                }
                break;
        }
        case 0xA000: { //LD I, addr
                return LD_I_addr;
        }
        case 0xD000: { //DRW Vx, Vy, nibble
                return DRW_Vx_Vy_nibble;
        }
        default:
                return -1;
        }

}

void chip8disasm(const char* rom_file, const char* output) {
        char rom_data[4096];
        size_t rom_size = read_rom(rom_file, &(rom_data[0]), 4096);

        FILE *output_file = fopen(output, "w");

        if (rom_size != -1 && output_file != NULL) {
                printf("%d bytes read.\n", rom_size);

                for (size_t i = 0; i < rom_size; i += 2) {
                        fprintf(output_file, "%x: ", i);

                        short opcode = rom_data[i] << 8;
                        opcode |= rom_data[i + 1] & 0xFF;

                        // Pieces of interest.  The vast majority of the 
                        // instructions will use one of these.  
                        
                        // x and y represent one of the 16 registers.
                        // n is a nibble representing a constant.
                        // kk is a byte constant.
                        // addr is a 12-bit address vaue.
                        char x = nibble(opcode, 2);
                        char y = nibble(opcode, 1);
                        char n = nibble(opcode, 0);
                        char kk = low_byte(opcode);
                        short addr = get_addr(opcode);

                        switch (identify_ins(opcode)) {
                        case SYS_addr:
                                fprintf(output_file, "SYS %x", 
                                                        get_addr(opcode));
                                break;
                        case CLS:
                                fprintf(output_file, "CLS");
                                break;
                        case RET:
                                fprintf(output_file, "RET");
                                break;
                        case JP_addr:
                                fprintf(output_file, "JP %x", 
                                                        get_addr(opcode));
                                break;
                        case CALL_addr:
                                fprintf(output_file, "CALL %x", 
                                                        get_addr(opcode));
                                break;
                        case SE_Vx_byte:
                                fprintf(output_file, "SE V%x, %x", x, kk);
                                break;
                        case SNE_Vx_byte:
                                fprintf(output_file, "SNE, V%x, %x", x, kk);
                                break;
                        case SE_Vx_Vy:
                                fprintf(output_file, "SE V%x, V%x", x, y);
                                break;
                        case LD_Vx_byte:
                                fprintf(output_file, "LD V%x, %x", x, kk);
                                break;
                        case ADD_Vx_byte:
                                fprintf(output_file, "ADD V%x, %x", x, kk);
                                break;
                        case LD_Vx_Vy:
                                fprintf(output_file, "LD V%x, V%x", x, y);
                                break;
                        case OR_Vx_Vy:
                                fprintf(output_file, "OR V%x, V%x", x, y);
                                break;
                        case AND_Vx_Vy:
                                fprintf(output_file, "AND V%x, V%x", x, y);
                                break;
                        case XOR_Vx_Vy:
                                fprintf(output_file, "XOR V%x, V%x", x, y);
                                break;
                        case ADD_Vx_Vy:
                                fprintf(output_file, "ADD V%x, V%x", x, y);
                                break;
                        case SUB_Vx_Vy:
                                fprintf(output_file, "SUB V%x, V%x", x, y);
                                break;
                        case SHR_Vx:
                                fprintf(output_file, "SHR V%x", x);
                                break;
                        case SUBN_Vx_Vy:
                                fprintf(output_file, "SUBN V%x, V%x", x, y);
                                break;
                        case SHL_Vx:
                                fprintf(output_file, "SHL V%x", x);
                                break;
                        case SNE_Vx_Vy:
                                fprintf(output_file, "UNKNOWN");
                                break;
                        case LD_I_addr:
                                fprintf(output_file, "UNKNOWN");
                                break;
                        case JP_V0_addr:
                                fprintf(output_file, "UNKNOWN");
                                break;
                        case RND_Vx_byte;
                                fprintf(output_file, "UNKNOWN");
                                break;
                        case DRW_Vx_Vy_nibble;
                                fprintf(output_file, "UNKNOWN");
                                break;
                        case SKP_Vx:
                                fprintf(output_file, "UNKNOWN");
                                break;
                        case SKNP_Vx:
                                fprintf(output_file, "UNKNOWN");
                                break;
                        case LD_Vx_DT:
                                fprintf(output_file, "UNKNOWN");
                                break;
                        case LD_Vx_K:
                                fprintf(output_file, "UNKNOWN");
                                break;
                        case LD_DT_Vx:
                                fprintf(output_file, "UNKNOWN");
                                break;
                        case LD_ST_Vx:
                                fprintf(output_file, "UNKNOWN");
                                break;
                        case ADD_I_Vx:
                                fprintf(output_file, "UNKNOWN");
                                break;
                        case LD_F_Vx:
                                fprintf(output_file, "UNKNOWN");
                                break;
                        case LD_B_Vx:
                                fprintf(output_file, "UNKNOWN");
                                break;
                        case LD_memI_Vx:
                                fprintf(output_file, "UNKNOWN");
                                break;
                        case LD_Vx_memI:
                                fprintf(output_file, "UNKNOWN");
                                break;
                        }
                }
        }
}


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