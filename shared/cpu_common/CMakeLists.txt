set(CPU_COMMON_LIB_TARGET_NAME cpu_common_lib)

set(CPU_COMMON_LIB_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
    ${CMAKE_CURRENT_SOURCE_DIR}/address_range.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/cpu_operations.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/cpu_operations.hpp
)

add_library(${CPU_COMMON_LIB_TARGET_NAME} STATIC
    ${CPU_COMMON_LIB_SOURCES}
)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX source FILES ${CPU_COMMON_LIB_SOURCES})

target_include_directories(${CPU_COMMON_LIB_TARGET_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(${CPU_COMMON_LIB_TARGET_NAME} PUBLIC
    cpp-common-lib
)

set_target_properties(${CPU_COMMON_LIB_TARGET_NAME} PROPERTIES
    FOLDER shared
)
