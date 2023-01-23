set(ASM_APP_TARGET_NAME assembler_app)

set(ASM_APP_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

add_executable(${ASM_APP_TARGET_NAME} ${ASM_APP_SOURCES})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX source FILES ${ASM_APP_SOURCES})

set_target_warnings(${ASM_APP_TARGET_NAME})
target_compile_options(${ASM_APP_TARGET_NAME} PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/MP /fp:fast /external:anglebrackets /external:W1>
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -pedantic>
)

target_link_libraries(${ASM_APP_TARGET_NAME} PRIVATE
    asm40xx_lib
)

set_target_properties(${ASM_APP_TARGET_NAME} PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY $<TARGET_FILE_DIR:${ASM_APP_TARGET_NAME}>
)
