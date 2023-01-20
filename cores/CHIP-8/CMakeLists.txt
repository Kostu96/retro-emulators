set(CHIP8_TARGET_NAME chip8)

set(CHIP8_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
    ${CMAKE_CURRENT_SOURCE_DIR}/chip8_core.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/chip8_core.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/chip8_instruction.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/disasm_chip8.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/disasm_chip8.hpp
)

add_library(${CHIP8_TARGET_NAME} SHARED ${CHIP8_SOURCES})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX source FILES ${CHIP8_SOURCES})

set_target_properties(${CHIP8_TARGET_NAME}
    PROPERTIES
    FOLDER cores
)
