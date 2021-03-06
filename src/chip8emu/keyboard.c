#include <SDL/SDL_keysym.h>

#include "keyboard.h"

int translate_key(unsigned int key) {
        switch (key) {
        case SDLK_1:
                return Key_1;
        case SDLK_2:
                return Key_2;
        case SDLK_3:
                return Key_3;
        case SDLK_4:
                return Key_C;
        case SDLK_q:
                return Key_4;
        case SDLK_w:
                return Key_5;
        case SDLK_e:
                return Key_6;
        case SDLK_r:
                return Key_D;
        case SDLK_a:
                return Key_7;
        case SDLK_s:
                return Key_8;
        case SDLK_d:
                return Key_9;
        case SDLK_f:
                return Key_E;
        case SDLK_z:
                return Key_A;
        case SDLK_x:
                return Key_0;
        case SDLK_c:
                return Key_B;
        case SDLK_v:
                return Key_F;
        default:
                return UNKNOWN_KEY;
        }
}

void keyboard_update(struct _kb *kb, SDL_KeyboardEvent *event) {
        if (!kb || !event) {
                return;
        }

        int key = translate_key(event->keysym.sym);
        if (key != UNKNOWN_KEY) {
                printf ("event for key: %x\n", key);
                printf ("type is "); 
                switch (event->type) {
                case SDL_KEYDOWN:
                        printf("key down\n");
                        break;
                case SDL_KEYUP:
                        printf("key up\n");
                        break;
                }
                kb->kb_buff[key] = (event->type == SDL_KEYDOWN) ? 1 : 0;
                printf ("key is %x\n", kb->kb_buff[key]);
        }
}

int keyboard_wait_key() {
        int key;
        SDL_Event event;
        while (1) {
                if (SDL_PollEvent(&event)) {
                        if (event.type == SDL_KEYDOWN || 
                                                event.type == SDL_KEYUP) {
                                SDL_KeyboardEvent *kb_event = &event.key;
                                key = translate_key(kb_event->keysym.sym);
                                if (key != UNKNOWN_KEY)
                                        break;
                        } else if (event.type == SDL_QUIT) {
                                /* We got a quit event, put this event back
                                 * and it will be taken care of by the 
                                 * main emulation loop.  
                                 */
                                SDL_PeepEvents(&event, 1, SDL_ADDEVENT, 
                                                                SDL_ALLEVENTS);
                                break;
                        }
                } 
        }
        return key;
}