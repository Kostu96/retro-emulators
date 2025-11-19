set(UTILS_LIB_TARGET_NAME utils)

add_library(${UTILS_LIB_TARGET_NAME} STATIC
    CMakeLists.txt
    src/asm_common.cpp
    src/file_io.cpp
    src/vec2.hpp
    src/include/utils/address_range.hpp
    src/include/utils/asm_common.hpp
    src/include/utils/disassembly_line.hpp
    src/include/utils/file_io.hpp
    src/include/utils/trie.hpp
    src/include/utils/types.hpp
    src/include/utils/warnings.hpp
)

target_include_directories(${UTILS_LIB_TARGET_NAME} PUBLIC
    src/include
)
