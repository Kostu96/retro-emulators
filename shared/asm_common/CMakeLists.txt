set(ASM_COMMON_LIB_TARGET_NAME asm_common_lib)

set(ASM_COMMON_LIB_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
    ${CMAKE_CURRENT_SOURCE_DIR}/asm_common.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/asm_common.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/trie.hpp
)

add_library(${ASM_COMMON_LIB_TARGET_NAME} STATIC
    ${ASM_COMMON_LIB_SOURCES}
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX source FILES ${ASM_COMMON_LIB_SOURCES})

target_include_directories(${ASM_COMMON_LIB_TARGET_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(${ASM_COMMON_LIB_TARGET_NAME} PUBLIC
    cpp-common-lib
)

set_target_properties(${ASM_COMMON_LIB_TARGET_NAME} PROPERTIES
    FOLDER shared
)
