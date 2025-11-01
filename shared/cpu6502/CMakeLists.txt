set(CPU6502_LIB_TARGET_NAME cpu6502)

add_device_library(
    TARGET_NAME ${CPU6502_LIB_TARGET_NAME}
    SOURCES
        CMakeLists.txt
        src/cpu6502.cpp
        src/include/cpu6502/cpu6502.hpp
    INCLUDE_DIRS
        src/include
    LIBRARIES
        utils
)

set(CPU6502_TEST_TARGET_NAME cpu6502_test)

add_executable(${CPU6502_TEST_TARGET_NAME}
    ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
    ${CMAKE_CURRENT_SOURCE_DIR}/test/instructions_tests.cpp
)

set_target_warnings(${CPU6502_TEST_TARGET_NAME})

target_link_libraries(${CPU6502_TEST_TARGET_NAME} PRIVATE
    ${CPU6502_LIB_TARGET_NAME}
    GTest::gtest
    GTest::gtest_main
)

add_custom_command(
    TARGET ${CPU6502_TEST_TARGET_NAME}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/third_party/6502_test_roms/. ${CMAKE_CURRENT_BINARY_DIR}/test_files/6502
)

gtest_discover_tests(${CPU6502_TEST_TARGET_NAME})
