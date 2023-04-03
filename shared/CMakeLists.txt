set(SHARED_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/address_range.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/disassembly_line.hpp
)

add_subdirectory(asm_common)
add_subdirectory(cpu_x80)
add_subdirectory(cpu40xx)
add_subdirectory(cpu6502)
add_subdirectory(cpu8008)
add_subdirectory(cpu8080)
