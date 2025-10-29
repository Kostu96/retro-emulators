set(CHIP8_TARGET_NAME chip8)

set(CHIP8_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
    ${CMAKE_CURRENT_SOURCE_DIR}/chip8.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/chip8.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/chip8_instruction.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/disasm_chip8.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/disasm_chip8.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

add_executable(${CHIP8_TARGET_NAME}
    ${CHIP8_SOURCES}
)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX source FILES ${CHIP8_SOURCES})

set_target_warnings(${CHIP8_TARGET_NAME})
target_compile_options(${CHIP8_TARGET_NAME} PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/MP /fp:fast /external:anglebrackets /external:W1>
)

target_link_libraries(${CHIP8_TARGET_NAME} PRIVATE
    shared_lib
    utils
)

set_target_properties(${CHIP8_TARGET_NAME}
    PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY $<TARGET_FILE_DIR:${CHIP8_TARGET_NAME}>
    FOLDER emulators
)
