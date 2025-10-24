set(UTILS_LIB_TARGET_NAME utils)

add_library(${UTILS_LIB_TARGET_NAME} STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
    ${CMAKE_CURRENT_SOURCE_DIR}/src/address_range.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/asm_common.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/file_io.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/vec2.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/include/utils/asm_common.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/include/utils/disassembly_line.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/include/utils/file_io.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/include/utils/trie.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/include/utils/types.hpp
)

target_include_directories(${UTILS_LIB_TARGET_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/include
)
