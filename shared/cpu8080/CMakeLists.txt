set(CPU8080_LIB_TARGET_NAME cpu8080)

add_device_library(
    TARGET_NAME ${CPU8080_LIB_TARGET_NAME}
    SOURCES
        CMakeLists.txt
        src/cpu8080.cpp
        src/disasm8080.cpp
        src/include/cpu8080/cpu8080.hpp
        src/include/cpu8080/disasm8080.hpp
    INCLUDE_DIRS
        src/include
    LIBRARIES
        utils
)

set(CPU8080_TEST_TARGET_NAME cpu8080_test)

add_executable(${CPU8080_TEST_TARGET_NAME}
    ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
    ${CMAKE_CURRENT_SOURCE_DIR}/test/arithmetic_tests.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/test/branch_tests.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/test/instructions_tests_fixture.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/test/misc_tests.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/test/transfer_tests.cpp
)

set_target_warnings(${CPU8080_TEST_TARGET_NAME})

target_link_libraries(${CPU8080_TEST_TARGET_NAME} PUBLIC
    ${CPU8080_LIB_TARGET_NAME}
    GTest::gtest
    GTest::gtest_main
)

gtest_discover_tests(${CPU8080_TEST_TARGET_NAME})
