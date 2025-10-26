set(CPU40XX_LIB_TARGET_NAME cpu40xx)

add_library(${CPU40XX_LIB_TARGET_NAME} STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
    ${CMAKE_CURRENT_SOURCE_DIR}/src/asm40xx.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/asm40xx.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/cpu40xx.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/parser40xx.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/parser40xx.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/scanner40xx.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/scanner40xx.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/include/cpu40xx/cpu40xx.hpp
)

set_target_warnings(${CPU40XX_LIB_TARGET_NAME})

target_include_directories(${CPU40XX_LIB_TARGET_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/include
)

target_link_libraries(${CPU40XX_LIB_TARGET_NAME} PUBLIC
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
