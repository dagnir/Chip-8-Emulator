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
 * A simple emulator for the Chip-8 Programming Language by 
 * Dongie Agnir <dongie.agnir@gmail.com>
 * 
 * All work based on the Chip-8 Technical Reference by Thomas P. Greene which
 * can be found at http://devernay.free.fr/hacks/chip8/C8TECH10.HTM.
 */

#ifndef __GUARD_CHIP8EMU_H__
#define __GUARD_CHIP8EMU_H__
#include <stdint.h>
#include <SDL/SDL.h>

#include "keyboard.h"
#include "display.h"

/* 4 KB RAM */
#define CHIP8_RAM_SIZE 4096
/* Stack stores only return addresses, 16 levels of nesting */
#define CHIP8_STACK_SIZE 16
#define CHIP8_STACK_ORIGIN 0xFF 
/* Chip-8 Program start at 0x200 */
static const unsigned int CHIP8_PROGRAM_START_OFFSET = 0x200;

#define CHIP8_DISPLAY_WIDTH 64 /* pixels */
#define CHIP8_DISPLAY_HEIGHT 32 /* pixels */
/* TODO: Make these user selectable. */
#define CHIP8_DISPLAY_PIX_COL_ON 0x33b5e5
#define CHIP8_DISPLAY_PIX_COL_OFF 0x0


static const unsigned int FONT_START_OFFSET = 0x0;
static const unsigned int FONT_LEN = 0x5; /* 5 bytes long */
static const unsigned int FONT_TABLE_LEN = 90; /* 15 * 5 */

/*
 * The timers are updated at 60 Hz, so 1s / 60.  Actually 16.67 */
#define TIMER_TICK_INTERVAL 16

/*
 * The "time" it takes to execute an opcode, in ms.  2 is about 512Hz. */
#define OPCODE_EXECUTION_TIME 2

/* Indices are calculated this way
 * 
 * FONT_START_OFFSET + FONT_LEN * value
 */
static const uint8_t FONT_TABLE[] = {
        /* "0" */
        0b11110000,
        0b10010000,
        0b10010000,
        0b10010000,
        0b11110000,
        /* "1" */
        0b00100000,
        0b01100000,
        0b00100000,
        0b00100000,
        0b01110000,
        /* "2" */
        0b11110000,
        0b00010000,
        0b11110000,
        0b10000000,
        0b11110000,
        /* "3" */
        0b11110000,
        0b00010000,
        0b11110000,
        0b00010000,
        0b11110000,
        /* "4" */
        0b10010000,
        0b10010000,
        0b11110000,
        0b00010000,
        0b00010000,
        /* "5" */
        0b11110000,
        0b10000000,
        0b11110000,
        0b00010000,
        0b11110000,
        /* "6" */
        0b11110000,
        0b10000000,
        0b11110000,
        0b10010000,
        0b11110000,
        /* "7" */
        0b11110000,
        0b00010000,
        0b00100000,
        0b01000000,
        0b01000000,
        /* "8" */
        0b11110000,
        0b10010000,
        0b11110000,
        0b10010000,
        0b11110000,
        /* "9" */
        0b11110000,
        0b10010000,
        0b11110000,
        0b00010000,
        0b11110000,
        /* "A" */
        0b11110000,
        0b10010000,
        0b11110000,
        0b10010000,
        0b10010000,
        /* "B" */
        0b11100000,
        0b10010000,
        0b11100000,
        0b10010000,
        0b11100000,
        /* "C" */
        0b11110000,
        0b10000000,
        0b10000000,
        0b10000000,
        0b11110000,
        /* "D" */
        0b11100000,
        0b10010000,
        0b10010000,
        0b10010000,
        0b11100000,
        /* "E" */
        0b11110000,
        0b10000000,
        0b11110000,
        0b10000000,
        0b11110000,
        /* "F" */
        0b11110000,
        0b10000000,
        0b11110000,
        0b10000000,
        0b10000000
};

/*
 * _chip8 is the main structure of the emulator.  It contains the entire
 * state of the emulator.
 */
struct _chip8 {
        struct _registers {
                /* General purpose registers, 0x0-0xF */
                uint8_t gen[16];
                /* Memory address register */
                uint16_t I;
                /* 
                 * The two timers below (DT and ST) are decremented at 60 Hz 
                 * Note: 
                 * Chip-8 Programs will set these registers to values [0-255],
                 * because they should be decremented at 60 Hz.  However, to 
                 * make emulation most straightforward, when they are being
                 * set, the values are converted to milliseconds instead, which
                 * simply means they are multiplied by 16.67.  This also means
                 * that they will have maximum value of 4250, so they must
                 * be 16 bits wide instead of 8 on the Chip-8.
                 */
                /* Delay Timer register */
                uint16_t DT;
                /* Sound Timer register */
                uint16_t ST;
                /* Program counter */
                uint16_t PC;
                /* Stack Pointer */
                uint8_t SP;

        } R;

        uint8_t ram[CHIP8_RAM_SIZE];
        uint16_t stack[CHIP8_STACK_SIZE];
        struct _kb kb;
        struct _display disp;

} chip8; /* global that will be shared among all instructions. */

/*
 * Contains all relevant information carried by an opcode.
 * addr - The lower 12 bits of the opcode
 * x - the third nibble in the opcode
 * y - the second nibble in the opcode
 * n - the first nibble in the opcode
 * kk - the lower byte of the opcode
 * ins - integer value [0-35] that identifies this instruction.  Also used to
         index into the jump table of instructions.
 */
struct _instruction {
        uint16_t addr;
        uint8_t x;
        uint8_t y;
        uint8_t n;
        uint8_t kk;
        uint8_t ins;
};

/*
 * Zeroes out the registers (except PC which starts at 0x200), clears the 
 * display,and loads the font table into memory.
 * Also does the following:
        -Seed the RNG
        -Initializes the instruction jump table
        -Initialize SDL
 */
void chip8emu_init();

/*
 * Loads the ROM into the emulator's memory.
 */
void chip8emu_load_rom(uint8_t *rom, unsigned short rom_size);

/*
 * Begins emulating the program loaded into the Chip-8 memory.
 */
void chip8emu_begin_emulate();

void chip8emu_begin_emulate_dummy();

struct _instruction parse_opcode(uint16_t opcode);

/* 
 * Executes the opcode at PC.
 */
void execute_next_ins();

void debug_print_registers();

/*------------------------Chip-8 Instructions-----------------------------*/
void ins_SYS_addr(struct _instruction ins);
void ins_CLS(struct _instruction ins);
void ins_RET(struct _instruction ins);
void ins_JP_addr(struct _instruction ins);
void ins_CALL_addr(struct _instruction ins);
void ins_SE_Vx_byte(struct _instruction ins);
void ins_SNE_Vx_byte(struct _instruction ins);
void ins_SE_Vx_Vy(struct _instruction ins);
void ins_LD_Vx_byte(struct _instruction ins);
void ins_ADD_Vx_byte(struct _instruction ins);
void ins_LD_Vx_Vy(struct _instruction ins);
void ins_OR_Vx_Vy(struct _instruction ins);
void ins_AND_Vx_Vy(struct _instruction ins);
void ins_XOR_Vx_Vy(struct _instruction ins);
void ins_ADD_Vx_Vy(struct _instruction ins);
void ins_SUB_Vx_Vy(struct _instruction ins);
void ins_SHR_Vx(struct _instruction ins);
void ins_SUBN_Vx_Vy(struct _instruction ins);
void ins_SHL_Vx(struct _instruction ins);
void ins_SNE_Vx_Vy(struct _instruction ins);
void ins_LD_I_addr(struct _instruction ins);
void ins_JP_V0_addr(struct _instruction ins);
void ins_RND_Vx_byte(struct _instruction ins);
void ins_DRW_Vx_Vy_nib(struct _instruction ins);
void ins_SKP_Vx(struct _instruction ins);
void ins_SKNP_Vx(struct _instruction ins);
void ins_LD_Vx_DT(struct _instruction ins);
void ins_LD_Vx_K(struct _instruction ins);
void ins_LD_DT_Vx(struct _instruction ins);
void ins_LD_ST_Vx(struct _instruction ins);
void ins_ADD_I_Vx(struct _instruction ins);
void ins_LD_F_Vx(struct _instruction ins);
void ins_LD_B_Vx(struct _instruction ins);
void ins_LD_memI_Vx(struct _instruction ins);
void ins_LD_Vx_memI(struct _instruction ins);  
/*------------------------Chip-8 Instructions-----------------------------*/

#endif /* __GUARD_CHIP8EMU_H__ */