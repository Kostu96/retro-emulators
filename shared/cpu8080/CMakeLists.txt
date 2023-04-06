set(CPU8080_LIB_TARGET_NAME cpu8080_lib)

set(CPU8080_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
    ${CMAKE_CURRENT_SOURCE_DIR}/cpu8080.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/cpu8080.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/disasm8080.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/disasm8080.hpp
)

add_library(${CPU8080_LIB_TARGET_NAME} STATIC ${SHARED_SOURCES} ${CPU8080_SOURCES})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX source FILES ${CPU8080_SOURCES})

target_include_directories(${CPU8080_LIB_TARGET_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(${CPU8080_LIB_TARGET_NAME} PUBLIC
    asm_common_lib
    cpu_common_lib
)

set_target_properties(${CPU8080_LIB_TARGET_NAME} PROPERTIES
    FOLDER shared/i8080
)

add_subdirectory(tests)
