set(SHARED_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/memory_map.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/type_aliases.hpp
)

set(ASM_COMMON_LIB_TARGET_NAME asm_common_lib)

set(ASM_COMMON_LIB_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
    ${CMAKE_CURRENT_SOURCE_DIR}/asm_common.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/asm_common.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/type_aliases.hpp
)

add_library(${ASM_COMMON_LIB_TARGET_NAME} STATIC ${ASM_COMMON_LIB_SOURCES})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX source FILES ${ASM_COMMON_LIB_SOURCES})

target_include_directories(${ASM_COMMON_LIB_TARGET_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(${ASM_COMMON_LIB_TARGET_NAME} PROPERTIES
    FOLDER shared
)

add_subdirectory(cpu4040)
add_subdirectory(cpu6502)
add_subdirectory(cpu8008)
add_subdirectory(cpu8080)
