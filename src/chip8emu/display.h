#ifndef __GUARD_DISPLAY_H__
#define __GUARD_DISPLAY_H__
#include <stdint.h>

#define CHIP8_DISPLAY_WIDTH 64 /* pixels */
#define CHIP8_DISPLAY_HEIGHT 32 /* pixels */

/* forward declaration. */
struct SDL_Surface;

struct _display {
        uint8_t buff[CHIP8_DISPLAY_HEIGHT][CHIP8_DISPLAY_WIDTH];
        SDL_Surface *disp;
};

/*
 * Returns 1 if as a result of drawing at (x,y) caused the existing pixel at
 * already at (x,y) to be erased.
 */
unsigned int display_draw_pix(struct _display *disp, unsigned short x, 
                                        unsigned short y, uint8_t state);

void display_clear(struct _display *disp);
void display_update(struct _display *disp);
void draw_pixel(struct _display *disp, short x, short y, unsigned int color);
#endif /* __GUARD_DISPLAY_H__ */