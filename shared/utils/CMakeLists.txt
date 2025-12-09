set(UTILS_LIB_TARGET_NAME utils)

add_library(${UTILS_LIB_TARGET_NAME} STATIC
    CMakeLists.txt
    src/asm_common.cpp
    src/file_io.cpp
    src/root_path.cpp
    src/vec2.hpp
    src/include/utils/address_range.hpp
    src/include/utils/asm_common.hpp
    src/include/utils/common.hpp
    src/include/utils/disassembly_line.hpp
    src/include/utils/exception.hpp
    src/include/utils/file_io.hpp
    src/include/utils/log.hpp
    src/include/utils/root_path.hpp
    src/include/utils/trie.hpp
    src/include/utils/types.hpp
    src/include/utils/warnings.hpp
)

set_target_warnings(${UTILS_LIB_TARGET_NAME})

target_compile_features(${UTILS_LIB_TARGET_NAME} PUBLIC cxx_std_23)

target_include_directories(${UTILS_LIB_TARGET_NAME} PUBLIC
    src/include
)
