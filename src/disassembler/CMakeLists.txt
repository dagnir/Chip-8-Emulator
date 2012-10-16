include_directories(${CHIP8_SOURCE_DIR}/utility)

link_directories(${CHIP8_BINARY_DIR}/utility)

add_library(chip8disasm chip8disasm.c)
target_link_libraries(chip8disasm utility)

add_executable(disassembler disassembler.c)
target_link_libraries(disassembler chip8disasm)