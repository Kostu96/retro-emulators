set(CPU8080_TESTS_TARGET_NAME cpu8080_tests)

set(CPU8080_TESTS_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
    ${CMAKE_CURRENT_SOURCE_DIR}/cpu8080_gameboy_mode_arithmetic_tests.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/cpu8080_gameboy_mode_fixture.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/cpu8080_gameboy_mode_jumps_tests.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/cpu8080_gameboy_mode_logic_tests.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/cpu8080_gameboy_mode_misc_tests.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/cpu8080_gameboy_mode_transfer_tests.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/cpu8080_mode_i8080_tests.cpp
)

add_executable(${CPU8080_TESTS_TARGET_NAME} ${CPU8080_TESTS_SOURCES})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX source FILES ${CPU8080_TESTS_SOURCES})

target_compile_definitions(${CPU8080_TESTS_TARGET_NAME} PRIVATE
    CPU8080_TESTS
)

target_link_libraries(${CPU8080_TESTS_TARGET_NAME} PRIVATE
    cpu8080_lib
    gtest
    gtest_main
)

set_target_properties(${CPU8080_TESTS_TARGET_NAME} PROPERTIES
    FOLDER shared/8080
)
