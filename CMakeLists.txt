cmake_minimum_required(VERSION 3.22)

project(emulators LANGUAGES CXX)
include(scripts/cmake/base_configuration.cmake)
include(scripts/cmake/compiler_warnings.cmake)

add_subdirectory(third_party)
add_subdirectory(shared)
add_subdirectory(cores)
add_subdirectory(assembler_app)
add_subdirectory(emulator_app)

add_subdirectory(altair8800)
add_subdirectory(gameboy)
