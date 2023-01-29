set(CPU40XX_TESTS_TARGET_NAME cpu40xx_tests)

set(CPU40XX_TESTS_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
    ${CMAKE_CURRENT_SOURCE_DIR}/cpu40xx_instructions_tests.cpp
)

add_executable(${CPU40XX_TESTS_TARGET_NAME} ${CPU40XX_TESTS_SOURCES})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX source FILES ${CPU40XX_TESTS_SOURCES})

target_link_libraries(${CPU40XX_TESTS_TARGET_NAME} PRIVATE
    cpu40xx_lib
    gtest
    gtest_main
)

set_target_properties(${CPU40XX_TESTS_TARGET_NAME} PROPERTIES
    FOLDER shared/40xx
)
