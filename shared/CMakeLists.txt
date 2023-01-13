set(SHARED_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/memory_map.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/type_aliases.hpp
)

add_subdirectory(cpu4040)
add_subdirectory(cpu6502)
add_subdirectory(cpu8080)
