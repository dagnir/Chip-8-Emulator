#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <SDL/SDL.h>

#include "chip8emu.h"
#include "opcodes.h"
#include "utility.h"
#include "chip8disasm.h"

#define DEBUG 0

/* Jump table of the instructions */
void (*ins_table[NUM_OPCODES])(struct _instruction ins);

void chip8emu_init() {
        /* Zero out everything */
        memset((void *)&chip8, 0, sizeof(chip8));
        /* Reset the PC */
        chip8.R.PC = CHIP8_PROGRAM_START_OFFSET;
        /* Reset the SP */
        chip8.R.SP = CHIP8_STACK_ORIGIN;

        /* copy the font table into the Chip-8's memory */
        void *font_dst = (void *)(&chip8.ram[FONT_START_OFFSET]);
        void *font_src = &(FONT_TABLE[0]);
        memcpy(font_dst, font_src, FONT_TABLE_LEN);

        /* seed the RNG */
        srand(time(0));

        /* Fill the jump table with our instructions */
        ins_table[SYS_addr] = ins_SYS_addr;
        ins_table[CLS] = ins_CLS;
        ins_table[RET] = ins_RET;
        ins_table[JP_addr] = ins_JP_addr;
        ins_table[CALL_addr] = ins_CALL_addr;
        ins_table[SE_Vx_byte] = ins_SE_Vx_byte;
        ins_table[SNE_Vx_byte] = ins_SNE_Vx_byte;
        ins_table[SE_Vx_Vy] = ins_SE_Vx_Vy;
        ins_table[LD_Vx_byte] = ins_LD_Vx_byte;
        ins_table[ADD_Vx_byte] = ins_ADD_Vx_byte;
        ins_table[LD_Vx_Vy] = ins_LD_Vx_Vy;
        ins_table[OR_Vx_Vy] = ins_OR_Vx_Vy;
        ins_table[AND_Vx_Vy] = ins_AND_Vx_Vy;
        ins_table[XOR_Vx_Vy] = ins_XOR_Vx_Vy;
        ins_table[ADD_Vx_Vy] = ins_ADD_Vx_Vy;
        ins_table[SUB_Vx_Vy] = ins_SUB_Vx_Vy;
        ins_table[SHR_Vx] = ins_SHR_Vx;
        ins_table[SUBN_Vx_Vy] = ins_SUBN_Vx_Vy;
        ins_table[SHL_Vx] = ins_SHL_Vx;
        ins_table[SNE_Vx_Vy] = ins_SNE_Vx_Vy;
        ins_table[LD_I_addr] = ins_LD_I_addr;
        ins_table[JP_V0_addr] = ins_JP_V0_addr;
        ins_table[RND_Vx_byte] = ins_RND_Vx_byte;
        ins_table[DRW_Vx_Vy_nibble] = ins_DRW_Vx_Vy_nib;
        ins_table[SKP_Vx] = ins_SKP_Vx;
        ins_table[SKNP_Vx] = ins_SKNP_Vx;
        ins_table[LD_Vx_DT] = ins_LD_Vx_DT;
        ins_table[LD_Vx_K] = ins_LD_Vx_K;
        ins_table[LD_DT_Vx] = ins_LD_DT_Vx;
        ins_table[LD_ST_Vx] = ins_LD_ST_Vx;
        ins_table[ADD_I_Vx] = ins_ADD_I_Vx;
        ins_table[LD_F_Vx] = ins_LD_F_Vx;
        ins_table[LD_B_Vx] = ins_LD_B_Vx;
        ins_table[LD_memI_Vx] = ins_LD_memI_Vx;
        ins_table[LD_Vx_memI] = ins_LD_Vx_memI;

        chip8.disp = SDL_SetVideoMode(CHIP8_DISPLAY_WIDTH * 10,
                                      CHIP8_DISPLAY_HEIGHT * 10,
                                      0, 0);

        return;
}

void chip8emu_load_rom(uint8_t *rom, unsigned short rom_size) {
        void *rom_dst = &(chip8.ram[CHIP8_PROGRAM_START_OFFSET]);

        memcpy (rom_dst, rom, rom_size);

        return;
}

/* 
 * Execution loop.
 * TODO: 
 *      1. update timers after each iteration
 *      2. Move out execution loop to a separate function
 */
void chip8emu_begin_emulate() {

        unsigned int done = 0;
        SDL_Event event;
        while (!done) {

                while (SDL_PollEvent(&event)) {
                        switch (event.type) {
                        case SDL_QUIT:
                                done = 1;
                                break;
                        }
                }

                if (done) {
#if DEBUG
                        debug_print_registers();
#endif
                        continue;
                }      
                /* FETCH */
                uint16_t opcode = (uint16_t)(chip8.ram[chip8.R.PC] << 8);
                opcode |= chip8.ram[chip8.R.PC + 1];
                chip8.R.PC += 2;

                /* DECODE */
                struct _instruction ins = parse_opcode(opcode);

                /* EXECUTE */
                (*ins_table[ins.ins])(ins);
        }
}

void chip8emu_begin_emulate_dummy() {
        unsigned int n_iters = 256;
        while (n_iters--) {
                /* FETCH */
                printf("PC is %u\n", chip8.R.PC);
                uint16_t opcode = (uint16_t)(chip8.ram[chip8.R.PC] << 8);
                opcode |= chip8.ram[chip8.R.PC + 1];
                chip8.R.PC += 2;
                printf("Fetched opcode %x\n", opcode);

                /* DECODE */
                struct _instruction ins = parse_opcode(opcode);
                printf("Executing instruction: %u\n", ins.ins);
                /* EXECUTE */
                (*ins_table[ins.ins])(ins);
        }
}

void update_display() {
        /* needs to be implemented. */
}

void debug_print_registers() {
        uint8_t x;
        for (x = 0; x <= 0xF; ++x) {
                printf("V%x: %u\n", x, chip8.R.gen[x]);
        }
        printf("I: %u\n", chip8.R.I);
        printf("DT: %u\n", chip8.R.DT);
        printf("ST: %u\n", chip8.R.ST);
        printf("PC: %u\n", chip8.R.PC);
        printf("SP: %u\n", chip8.R.SP);
}

struct _instruction parse_opcode(uint16_t opcode) {
        struct _instruction ins;
        ins.x = nibble(opcode, 2);
        ins.y = nibble(opcode, 1);
        ins.n = nibble(opcode, 0);
        ins.kk = low_byte(opcode);
        ins.addr = get_addr(opcode);
        ins.ins = identify_ins(opcode);

        return ins;
}

/* This instruction is ignored. */
void ins_SYS_addr(struct _instruction ins) {
        return;
}

void ins_CLS(struct _instruction ins) {
        uint8_t r;
        for (r = 0; r < CHIP8_DISPLAY_HEIGHT; ++r) {
                uint8_t c;
                for (c = 0; c < CHIP8_DISPLAY_WIDTH; ++c) {
                        chip8.display_buffer[r][c] = 0;
                }
        }
}

void ins_RET(struct _instruction ins) {
        chip8.R.PC = chip8.stack[chip8.R.SP];
        chip8.R.SP = chip8.R.SP - 1;
}

void ins_JP_addr(struct _instruction ins) {
#if DEBUG
        printf("Jumping to %u\n", ins.addr);
#endif
        chip8.R.PC = ins.addr;
}

void ins_CALL_addr(struct _instruction ins) {
        chip8.R.SP++;
        chip8.stack[chip8.R.SP] = chip8.R.PC;
        chip8.R.PC = ins.addr;
}

void ins_SE_Vx_byte(struct _instruction ins) {
        if (chip8.R.gen[ins.x] == ins.kk) {
                chip8.R.PC += 2;
        }
}

void ins_SNE_Vx_byte(struct _instruction ins) {
        if (chip8.R.gen[ins.x] != ins.kk) {
                chip8.R.PC += 2;
        }
}

void ins_SE_Vx_Vy(struct _instruction ins) {
        if (chip8.R.gen[ins.x] == chip8.R.gen[ins.y]) {
                chip8.R.PC += 2;
        }
}

void ins_LD_Vx_byte(struct _instruction ins) {
        chip8.R.gen[ins.x] = ins.kk;
}

void ins_ADD_Vx_byte(struct _instruction ins) {
        chip8.R.gen[ins.x] += ins.kk;
}

void ins_LD_Vx_Vy(struct _instruction ins) {
        chip8.R.gen[ins.x] = chip8.R.gen[ins.y];
}

void ins_OR_Vx_Vy(struct _instruction ins) {
        chip8.R.gen[ins.x] |= chip8.R.gen[ins.y]; 
}

void ins_AND_Vx_Vy(struct _instruction ins) {
        chip8.R.gen[ins.x] &= chip8.R.gen[ins.y];
}

void ins_XOR_Vx_Vy(struct _instruction ins) {
        chip8.R.gen[ins.x] ^= chip8.R.gen[ins.y];
}

void ins_ADD_Vx_Vy(struct _instruction ins) {
        uint16_t res = chip8.R.gen[ins.x] + chip8.R.gen[ins.y];
        if (res > 255) {
                chip8.R.gen[0xF] = 1;
        } else {
                chip8.R.gen[0xF] = 0;
        }
        chip8.R.gen[ins.x] = res & 0xFF;
}

void ins_SUB_Vx_Vy(struct _instruction ins) {
        if (chip8.R.gen[ins.x] > chip8.R.gen[ins.y]) {
                chip8.R.gen[0xF] = 1;
        } else {
                chip8.R.gen[0xF] = 0;
        }
        chip8.R.gen[ins.x] -= chip8.R.gen[ins.y];
}

void ins_SHR_Vx(struct _instruction ins) {
        if (chip8.R.gen[ins.x] & 0x1) {
                chip8.R.gen[0xF] = 1;
        } else {
                chip8.R.gen[0xF] = 0;
        }
        chip8.R.gen[ins.x] >>= 1;
}

void ins_SUBN_Vx_Vy(struct _instruction ins) {
        if (chip8.R.gen[ins.y] > chip8.R.gen[ins.x]) {
                chip8.R.gen[0xF] = 1;
        } else {
                chip8.R.gen[0xF] = 0;
        }
        chip8.R.gen[ins.x] = chip8.R.gen[ins.y] - chip8.R.gen[ins.x];
}

void ins_SHL_Vx(struct _instruction ins) {
        if (chip8.R.gen[ins.x] & 0x80) {
                chip8.R.gen[0xF] = 1;
        } else {
                chip8.R.gen[0xF] = 0;
        }
        chip8.R.gen[ins.x] <<= 1;
}

void ins_SNE_Vx_Vy(struct _instruction ins) {
        if (chip8.R.gen[ins.x] != chip8.R.gen[ins.y]) {
                chip8.R.PC += 2;
        }
}

void ins_LD_I_addr(struct _instruction ins) {
        chip8.R.I = ins.addr;
}


void ins_JP_V0_addr(struct _instruction ins) {
        chip8.R.PC = chip8.R.gen[0x0] + ins.addr;
}

void ins_RND_Vx_byte(struct _instruction ins) {
        /*
         * NOTE: This method using % doesn't give a proper even distribution
         * over the range, but is okay for now for our needs.
         */
        uint8_t random = (uint8_t)(rand() % 256);
        chip8.R.gen[ins.x] = random & ins.kk;
}

void ins_DRW_Vx_Vy_nib(struct _instruction ins) {
        uint8_t x = chip8.R.gen[ins.x];
        uint8_t y = chip8.R.gen[ins.y];

        /* clear the flag register.  It will be modified when necessary when
         * the pixels are being set.
         */
        chip8.R.gen[0xF] = 0;
        uint8_t r;
        for (r = 0; r < ins.n; ++r) {
                uint8_t spb = chip8.ram[chip8.R.I + r];
                uint8_t screen_y = (y + r) % CHIP8_DISPLAY_HEIGHT;

                uint8_t c;
                for (c = 0; c < 8; ++c) {
                        uint8_t screen_x = (x + c) % CHIP8_DISPLAY_WIDTH;

                        uint8_t on = spb & (0x80 >> c);
                        if (chip8.display_buffer[screen_y][screen_x] && 
                                                                        !on) {
                                chip8.R.gen[0xF] = 1;
                        }

                }       
        }
        update_display();
}


void ins_SKP_Vx(struct _instruction ins) {
        if (chip8.keyboard[chip8.R.gen[ins.x]] == 1) {
                chip8.R.PC += 2;
        }
}

void ins_SKNP_Vx(struct _instruction ins) {
        if (chip8.keyboard[chip8.R.gen[ins.x]] == 0) {
                chip8.R.PC += 2;
        }
}

void ins_LD_Vx_DT(struct _instruction ins) {
        chip8.R.gen[ins.x] = chip8.R.DT;
}

void ins_LD_Vx_K(struct _instruction ins) {
        /* needs to be implemented. */
}

void ins_LD_DT_Vx(struct _instruction ins) {
        chip8.R.DT = chip8.R.gen[ins.x];
}

void ins_LD_ST_Vx(struct _instruction ins) {
        chip8.R.ST = chip8.R.gen[ins.x];
}

void ins_ADD_I_Vx(struct _instruction ins) {
        chip8.R.I += chip8.R.gen[ins.x];
}

void ins_LD_F_Vx(struct _instruction ins) {
        chip8.R.I = FONT_START_OFFSET + (chip8.R.gen[ins.x] * FONT_LEN);
}

void ins_LD_B_Vx(struct _instruction ins) {
        uint8_t val = chip8.R.gen[ins.x];
        uint8_t o = val % 10;
        val /= 10;
        uint8_t t = val % 10;
        val /= 10;
        uint8_t h = val % 10;
        uint16_t I = chip8.R.I;

        chip8.ram[I] = h;
        chip8.ram[I + 1] = t;
        chip8.ram[I + 2] = o;
}


void ins_LD_memI_Vx(struct _instruction ins) {
        uint8_t x;
        for (x = 0; x <= ins.x; ++x) {
                chip8.ram[chip8.R.I + x] = chip8.R.gen[x];
        }
}

void ins_LD_Vx_memI(struct _instruction ins) {
        uint8_t x;
        for (x = 0; x <= ins.x; ++x) {
                chip8.R.gen[x] = chip8.ram[chip8.R.I + x];
        }
}