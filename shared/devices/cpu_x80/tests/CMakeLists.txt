set(CPUX80_TESTS_TARGET_NAME cpu_x80_tests)

set(CPUX80_TESTS_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
    ${CMAKE_CURRENT_SOURCE_DIR}/cpu_x80_gameboy_mode_arithmetic_tests.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/cpu_x80_gameboy_mode_fixture.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/cpu_x80_gameboy_mode_jumps_tests.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/cpu_x80_gameboy_mode_logic_tests.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/cpu_x80_gameboy_mode_misc_tests.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/cpu_x80_gameboy_mode_transfer_tests.cpp
)

add_executable(${CPUX80_TESTS_TARGET_NAME} ${CPUX80_TESTS_SOURCES})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX source FILES ${CPUX80_TESTS_SOURCES})

target_compile_definitions(${CPUX80_TESTS_TARGET_NAME} PRIVATE
    CPUx80_TESTS
)

target_link_libraries(${CPUX80_TESTS_TARGET_NAME} PRIVATE
    cpu_x80_lib
    gtest
    gtest_main
)

set_target_properties(${CPUX80_TESTS_TARGET_NAME} PROPERTIES
    FOLDER shared/x80
)
