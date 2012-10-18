cmake_minimum_required(VERSION 2.8)

project(CHIP8)

#set(CMAKE_C_FLAGS "-ansi -Wall")

add_subdirectory(disassembler)
add_subdirectory(utility)
add_subdirectory(chip8emu)