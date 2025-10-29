set(GAMEBOY_TESTS_TARGET_NAME ${GAMEBOY_TARGET_NAME}_tests)
set(GAMEBOY_TESTS_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
    ${CMAKE_CURRENT_SOURCE_DIR}/cpu_arithmetic_tests.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/cpu_branch_tests.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/cpu_logic_tests.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/cpu_misc_tests.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/cpu_tests_fixture.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/cpu_transfer_tests.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/rom_tests.cpp
)

add_executable(${GAMEBOY_TESTS_TARGET_NAME} ${GAMEBOY_TESTS_SOURCES})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX source FILES ${GAMEBOY_TESTS_SOURCES})

target_compile_definitions(${GAMEBOY_TESTS_TARGET_NAME} PRIVATE
    GAMEBOY_TESTS
)

target_link_libraries(${GAMEBOY_TESTS_TARGET_NAME} PRIVATE
    ${GAMEBOY_LIB_TARGET_NAME}
    GTest::gtest
    GTest::gtest_main
)

set_target_properties(${GAMEBOY_TESTS_TARGET_NAME} PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY $<TARGET_FILE_DIR:${GAMEBOY_TESTS_TARGET_NAME}>
    FOLDER ${GAMEBOY_FOLDER_NAME}
)

add_custom_command(
    TARGET ${GAMEBOY_TESTS_TARGET_NAME}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/third_party/gb-test-roms/cpu_instrs/individual $<TARGET_FILE_DIR:${GAMEBOY_TESTS_TARGET_NAME}>/test_files/gameboy/blargg
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/third_party/mooneye-test-roms $<TARGET_FILE_DIR:${GAMEBOY_TESTS_TARGET_NAME}>/test_files/gameboy/mooneye
)
