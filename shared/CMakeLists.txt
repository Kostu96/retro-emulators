set(SHARED_LIB_TARGET_NAME shared_lib)
set(SHARED_LIB_TESTS_TARGET_NAME ${SHARED_LIB_TARGET_NAME}_tests)

set(SHARED_LIB_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/source/asm/asm_common.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/source/asm/asm_common.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/source/asm/trie.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/source/devices/cpu40xx/asm40xx.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/source/devices/cpu40xx/asm40xx.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/source/devices/cpu40xx/cpu40xx.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/source/devices/cpu40xx/cpu40xx.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/source/devices/cpu40xx/parser40xx.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/source/devices/cpu40xx/parser40xx.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/source/devices/cpu40xx/scanner40xx.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/source/devices/cpu40xx/scanner40xx.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/source/devices/cpu6502/cpu6502.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/source/devices/cpu6502/cpu6502.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/source/devices/cpu8008/asm8008.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/source/devices/cpu8008/asm8008.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/source/devices/cpu8008/cpu8008.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/source/devices/cpu8008/cpu8008.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/source/devices/cpu8008/scanner8008.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/source/devices/cpu8008/scanner8008.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/source/devices/cpu8080/cpu8080.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/source/devices/cpu8080/cpu8080.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/source/devices/cpu8080/disasm8080.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/source/devices/cpu8080/disasm8080.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/source/address_range.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/source/disassembly_line.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/source/file_io.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/source/file_io.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/source/imgui_helper.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/source/imgui_helper.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/source/types.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/source/vec2.hpp
)

set(SHARED_LIB_3RD_PARTY
    ${CMAKE_SOURCE_DIR}/third_party/tinyfiledialogs/tinyfiledialogs.c
    ${CMAKE_SOURCE_DIR}/third_party/tinyfiledialogs/tinyfiledialogs.h
)

add_library(${SHARED_LIB_TARGET_NAME} STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
    ${SHARED_LIB_SOURCES}
    ${SHARED_LIB_3RD_PARTY}
)

set_source_files_properties(${CMAKE_SOURCE_DIR}/third_party/tinyfiledialogs/tinyfiledialogs.c PROPERTIES LANGUAGE CXX)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/source PREFIX source FILES ${SHARED_LIB_SOURCES})
source_group(TREE ${CMAKE_SOURCE_DIR}/third_party PREFIX third_party FILES ${SHARED_LIB_3RD_PARTY})

set_target_warnings(${SHARED_LIB_TARGET_NAME})
target_compile_options(${SHARED_LIB_TARGET_NAME} PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/MP /fp:fast /external:anglebrackets /external:W1>
)

target_include_directories(${SHARED_LIB_TARGET_NAME} PUBLIC
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/third_party/tinyfiledialogs
)

target_link_libraries(${SHARED_LIB_TARGET_NAME} PUBLIC
    glfw
    glw
    imgui
)

set(SHARED_LIB_TESTS_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/tests/devices/cpu40xx/cpu40xx_instructions_tests.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/tests/devices/cpu6502/cpu6502_tests.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/tests/devices/cpu8080/cpu8080_tests.cpp
)

add_executable(${SHARED_LIB_TESTS_TARGET_NAME}
    ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
    ${SHARED_LIB_TESTS_SOURCES}
)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/tests PREFIX tests FILES ${SHARED_LIB_TESTS_SOURCES})

target_link_libraries(${SHARED_LIB_TESTS_TARGET_NAME} PRIVATE
    ${SHARED_LIB_TARGET_NAME}
    gtest
    gtest_main
)
