cmake_minimum_required(VERSION 3.22)

project(emulators LANGUAGES CXX)
include(scripts/cmake/base_configuration.cmake)
include(scripts/cmake/compiler_warnings.cmake)

set(SFML_STATIC_LIBRARIES TRUE)
set(SFML_DIR "E:/Programming/SFML_2.6.x/lib/cmake/SFML") # temp
find_package(SFML 2 COMPONENTS graphics)

add_subdirectory(third_party)
add_subdirectory(shared)
add_subdirectory(assembler_app)

add_subdirectory(emulators/common)
add_subdirectory(emulators/altair8800)
add_subdirectory(emulators/chip8)
add_subdirectory(emulators/gameboy)
