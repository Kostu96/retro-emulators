set(CPU40XX_LIB_TARGET_NAME cpu40xx)

add_device_library(
    TARGET_NAME ${CPU40XX_LIB_TARGET_NAME}
    SOURCES
        CMakeLists.txt
        src/asm40xx.cpp
        src/asm40xx.hpp
        src/cpu40xx.cpp
        src/parser40xx.cpp
        src/parser40xx.hpp
        src/scanner40xx.cpp
        src/scanner40xx.hpp
        src/include/cpu40xx/cpu40xx.hpp
    INCLUDE_DIRS
        src/include
    LIBRARIES
        utils
)

set(CPU40XX_TEST_TARGET_NAME cpu40xx_test)

add_executable(${CPU40XX_TEST_TARGET_NAME}
    ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
    ${CMAKE_CURRENT_SOURCE_DIR}/test/instructions_tests.cpp
)

set_target_warnings(${CPU40XX_TEST_TARGET_NAME})

target_link_libraries(${CPU40XX_TEST_TARGET_NAME} PRIVATE
    ${CPU40XX_LIB_TARGET_NAME}
    GTest::gtest
    GTest::gtest_main
)

gtest_discover_tests(${CPU40XX_TEST_TARGET_NAME})
