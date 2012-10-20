#include <SDL/SDL.h>

#include "display.h"

unsigned int display_draw_pix(struct _display *disp, unsigned short x, 
                                        unsigned short y, uint8_t state) {
        if (!disp) {
                return;
        }
        unsigned int ret;
        uint8_t on = state ^ disp->buff[y][x];
        if (disp->buff[y][x] && !on) {
                ret = 1;
        } else {
                ret = 0;
        }
        disp->buff[y][x] = on;
}

void display_clear(struct _display *disp) {
        uint8_t r;
        for (r = 0; r < CHIP8_DISPLAY_HEIGHT; ++r) {
                uint8_t c;
                for (c = 0; c < CHIP8_DISPLAY_WIDTH; ++c) {
                        disp->buff[r][c] = 0;
                }
        }
}

void display_update(struct _display *disp) {
    uint8_t y;
    for (y = 0; y < CHIP8_DISPLAY_HEIGHT; ++y) {
        uint8_t x;
        for (x = 0; x < CHIP8_DISPLAY_WIDTH; ++x) {
            uint8_t on = disp->buff[y][x];
            draw_pixel(disp, x, y, (on == 1) ? 0xFFFFFF : 0x0);
        }
    }
    SDL_Flip(disp->disp);
}

void draw_pixel(struct _display *disp, short x, short y, unsigned int color) {
    SDL_Rect pix;
    pix.x = x * 10;
    pix.y = y * 10;
    pix.w = 10;
    pix.h = 10;
    SDL_FillRect(disp->disp, &pix, color);
}