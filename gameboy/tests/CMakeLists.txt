set(GAMEBOY_TESTS_TARGET_NAME ${GAMEBOY_TARGET_NAME}_tests)
set(GAMEBOY_TESTS_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
    ${CMAKE_CURRENT_SOURCE_DIR}/rom_tests.cpp
)

add_executable(${GAMEBOY_TESTS_TARGET_NAME} ${GAMEBOY_TESTS_SOURCES})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX source FILES ${GAMEBOY_TESTS_SOURCES})

target_compile_definitions(${GAMEBOY_TESTS_TARGET_NAME} PRIVATE
    GAMEBOY_TESTS
)

target_link_libraries(${GAMEBOY_TESTS_TARGET_NAME} PRIVATE
    ${GAMEBOY_LIB_TARGET_NAME}
    gtest
    gtest_main
)

set_target_properties(${GAMEBOY_TESTS_TARGET_NAME} PROPERTIES
    FOLDER gameboy
)

add_custom_command(
    TARGET ${GAMEBOY_TESTS_TARGET_NAME}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/third_party/gb-test-roms/cpu_instrs/individual $<TARGET_FILE_DIR:${GAMEBOY_TESTS_TARGET_NAME}>/test_files/gameboy/blargg
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/third_party/mooneye-test-roms/boot_div-dmgABCmgb.gb $<TARGET_FILE_DIR:${GAMEBOY_TESTS_TARGET_NAME}>/test_files/gameboy/mooneye/boot_div-dmgABCmgb.gb
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/third_party/mooneye-test-roms/boot_regs-dmgABC.gb $<TARGET_FILE_DIR:${GAMEBOY_TESTS_TARGET_NAME}>/test_files/gameboy/mooneye/boot_regs-dmgABC.gb
)
