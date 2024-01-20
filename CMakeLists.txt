cmake_minimum_required(VERSION 3.22)

project(emulators LANGUAGES CXX)
include(scripts/cmake/base_configuration.cmake)
include(scripts/cmake/compiler_warnings.cmake)

add_subdirectory(third_party)

add_subdirectory(shared/asm_common)
add_subdirectory(shared/cpu_common)
add_subdirectory(shared/cpu_x80)
add_subdirectory(shared/cpu40xx)
add_subdirectory(shared/cpu6502)
add_subdirectory(shared/cpu8008)
add_subdirectory(shared/cpu8080)

add_subdirectory(emulators/emu_common)
add_subdirectory(emulators/altair8800)
add_subdirectory(emulators/apple2)
add_subdirectory(emulators/c64)
add_subdirectory(emulators/chip8)
add_subdirectory(emulators/gameboy)
add_subdirectory(emulators/nes)
add_subdirectory(emulators/pet)
add_subdirectory(emulators/space_invaders)

add_subdirectory(assembler_app)
