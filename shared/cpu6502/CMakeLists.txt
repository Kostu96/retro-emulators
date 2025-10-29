set(CPU6502_LIB_TARGET_NAME cpu6502)

add_library(${CPU6502_LIB_TARGET_NAME} STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
    ${CMAKE_CURRENT_SOURCE_DIR}/src/cpu6502.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/include/cpu6502/cpu6502.hpp
)

target_include_directories(${CPU6502_LIB_TARGET_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/include
)

target_link_libraries(${CPU6502_LIB_TARGET_NAME} PUBLIC
    utils
)

set(CPU6502_TEST_TARGET_NAME cpu6502_test)

add_executable(${CPU6502_TEST_TARGET_NAME}
    ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
    ${CMAKE_CURRENT_SOURCE_DIR}/test/instructions_tests.cpp
)

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
