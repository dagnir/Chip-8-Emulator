#ifndef __GUARD_KEYBOARD_H__
#define __GUARD_KEYBOARD_H__
#include <SDL/SDL.h>

#define UNKNOWN_KEY -1
#define NUM_KEYS 16


struct _kb {
        uint8_t kb_buff[NUM_KEYS];
};

enum {
        Key_0 = 0,
        Key_1,
        Key_2,
        Key_3,
        Key_4,
        Key_5,
        Key_6,
        Key_7,
        Key_8,
        Key_9,
        Key_A,
        Key_B,
        Key_C,
        Key_D,
        Key_E,
        Key_F
};

void keyboard_update(struct _kb *kb, SDL_KeyboardEvent *event);
int translate_key(unsigned int key);


#endif /* __GUARD_KEYBOARD_H__ */