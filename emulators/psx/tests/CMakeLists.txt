set(PSX_TESTS_TARGET_NAME ${PSX_TARGET_NAME}_tests)
set(PSX_TESTS_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
    ${CMAKE_CURRENT_SOURCE_DIR}/cpu_alu_instructions_tests.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/cpu_branch_instructions_tests.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/cpu_instructions_tests_fixture.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/cpu_transfer_instructions_tests.cpp
)

add_executable(${PSX_TESTS_TARGET_NAME} ${PSX_TESTS_SOURCES})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX source FILES ${PSX_TESTS_SOURCES})

target_compile_definitions(${PSX_TESTS_TARGET_NAME} PRIVATE
    PSX_TESTS
)

target_link_libraries(${PSX_TESTS_TARGET_NAME} PRIVATE
    ${PSX_LIB_TARGET_NAME}
    gtest
    gtest_main
)

set_target_properties(${PSX_TESTS_TARGET_NAME} PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY $<TARGET_FILE_DIR:${PSX_TESTS_TARGET_NAME}>
    FOLDER ${PSX_FOLDER_NAME}
)
