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
 
#include <stdio.h>

#include "chip8emu.h"
#include "utility.h"

#define ROM_SIZE 4096

int main(int argc, char** argv) {

        if (argc > 1) {
                uint8_t rom[ROM_SIZE];
                int size = read_rom(argv[1], rom, ROM_SIZE);
                if (size != -1) {
                        chip8emu_init();
                        chip8emu_load_rom(rom, size);
                        chip8emu_begin_emulate();
                }
        }
        return 0;
}

