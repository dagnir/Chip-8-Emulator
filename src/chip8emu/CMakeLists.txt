include_directories(${CHIP8_SOURCE_DIR}/utility)
include_directories(${CHIP8_SOURCE_DIR}/disassembler)
#SDL dependency
include_directories(${CHIP8_SOURCE_DIR}/../dep/SDL/include)

link_directories(${CHIP8_BINARY_DIR}/utility)
link_directories(${CHIP8_BINARY_DIR}/disassembler)
#SDL dependency
link_directories(${CHIP8_BINARY_DIR}/../dep/SDL/lib)


add_executable(chip8emulator chip8emulator.c chip8emu.c)

target_link_libraries(chip8emulator utility)
target_link_libraries(chip8emulator chip8disasm)
target_link_libraries(chip8emulator mingw32)
target_link_libraries(chip8emulator SDLmain)
target_link_libraries(chip8emulator SDL)
