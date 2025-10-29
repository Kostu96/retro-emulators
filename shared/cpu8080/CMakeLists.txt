set(CPU8080_LIB_TARGET_NAME cpu8080)

add_library(${CPU8080_LIB_TARGET_NAME} STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
    ${CMAKE_CURRENT_SOURCE_DIR}/src/cpu8080.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/disasm8080.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/include/cpu8080/cpu8080.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/include/cpu8080/disasm8080.hpp
)

target_include_directories(${CPU8080_LIB_TARGET_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/include
)

target_link_libraries(${CPU8080_LIB_TARGET_NAME} PUBLIC
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

target_link_libraries(${CPU8080_TEST_TARGET_NAME} PUBLIC
    ${CPU8080_LIB_TARGET_NAME}
    GTest::gtest
    GTest::gtest_main
)

gtest_discover_tests(${CPU8080_TEST_TARGET_NAME})
