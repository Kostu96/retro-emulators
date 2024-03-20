cmake_minimum_required(VERSION 3.22)

project(emulators LANGUAGES CXX)
include(cmake/base_configuration.cmake)
include(cmake/compiler_warnings.cmake)

add_subdirectory(third_party)

add_subdirectory(shared)
#add_subdirectory(shared/emu_common)

#add_subdirectory(emulators/altair8800)
#add_subdirectory(emulators/apple2)
add_subdirectory(emulators/c64)
add_subdirectory(emulators/chip8)
add_subdirectory(emulators/gameboy)
add_subdirectory(emulators/kim1)
add_subdirectory(emulators/nes)
add_subdirectory(emulators/pet)
add_subdirectory(emulators/psx)
add_subdirectory(emulators/space_invaders)
add_subdirectory(emulators/vic20)

add_subdirectory(assembler_app)
