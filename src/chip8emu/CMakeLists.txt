include_directories(${CHIP8_SOURCE_DIR}/utility)
include_directories(${CHIP8_SOURCE_DIR}/disassembler)

link_directories(${CHIP8_BINARY_DIR}/utility)
link_directories(${CHIP8_BINARY_DIR}/disassembler)

add_executable(chip8emulator chip8emulator.c chip8emu.c)

target_link_libraries(chip8emulator utility)
target_link_libraries(chip8emulator chip8disasm)