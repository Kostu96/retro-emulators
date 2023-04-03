set(CPUX80_LIB_TARGET_NAME cpu_x80_lib)

set(CPUX80_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
    ${CMAKE_CURRENT_SOURCE_DIR}/cpu_x80.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/cpu_x80.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/cpu_x80_base.inl
    ${CMAKE_CURRENT_SOURCE_DIR}/cpu_x80_gameboy.cpp
)

add_library(${CPUX80_LIB_TARGET_NAME} STATIC ${SHARED_SOURCES} ${CPUX80_SOURCES})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX source FILES ${CPUX80_SOURCES})

target_include_directories(${CPUX80_LIB_TARGET_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(${CPUX80_LIB_TARGET_NAME} PUBLIC
    asm_common_lib
    cpp-common-lib
)

set_target_properties(${CPUX80_LIB_TARGET_NAME} PROPERTIES
    FOLDER shared/x80
)

add_subdirectory(tests)
