cmake_minimum_required(VERSION 3.22)
project(emulators LANGUAGES CXX)

include(cmake/base_configuration.cmake)
include(cmake/compiler_warnings.cmake)

enable_testing()
find_package(GTest REQUIRED)
include(GoogleTest)

add_subdirectory(shared/utils)

add_subdirectory(shared/cpu40xx)
add_subdirectory(shared/cpu6502)
add_subdirectory(shared/cpu8008)
add_subdirectory(shared/cpu8080)

set_target_properties(
    cpu40xx
    cpu40xx_test
    cpu6502
    cpu6502_test
    cpu8008
    cpu8080
    cpu8080_test
    PROPERTIES
    FOLDER devices
)

add_subdirectory(third_party)

#add_subdirectory(shared)
#add_subdirectory(shared/emu_common)

#add_subdirectory(emulators/altair8800)
#add_subdirectory(emulators/apple2)
#add_subdirectory(emulators/c64)
#add_subdirectory(emulators/chip8)
#add_subdirectory(emulators/gameboy)
#add_subdirectory(emulators/kim1)
#add_subdirectory(emulators/nes)
#add_subdirectory(emulators/pet)
#add_subdirectory(emulators/psx)
#add_subdirectory(emulators/space_invaders)
#add_subdirectory(emulators/vic20)

#add_subdirectory(assembler_app)
