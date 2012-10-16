cmake_minimum_required(VERSION 2.8)

project(CHIP8)

set(CMAKE_C_FLAGS "-std=c99 -Wall -Wconversion")

add_subdirectory(disassembler)
add_subdirectory(utility)