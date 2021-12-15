cmake_minimum_required(VERSION 3.21)
project(risc_lab)

set(CMAKE_CXX_STANDARD 17)

add_executable(risc_lab main.cpp elf_parsing.h instructions.h)
