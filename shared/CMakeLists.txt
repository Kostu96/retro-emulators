set(SHARED_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/memory_map.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/type_aliases.hpp
)

add_subdirectory(asm_common)
add_subdirectory(cpu_x80)
add_subdirectory(cpu40xx)
add_subdirectory(cpu6502)
add_subdirectory(cpu8008)
