set(CPU6502_LIB_TARGET_NAME cpu6502_lib)

set(CPU6502_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
    ${CMAKE_CURRENT_SOURCE_DIR}/cpu6502.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/cpu6502.hpp
)

add_library(${CPU6502_LIB_TARGET_NAME} STATIC ${SHARED_SOURCES} ${CPU6502_SOURCES})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX source FILES ${CPU6502_SOURCES})

target_include_directories(${CPU6502_LIB_TARGET_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(${CPU6502_LIB_TARGET_NAME} PUBLIC
    cpp-common-lib
    cpu_common_lib
)

set_target_properties(${CPU6502_LIB_TARGET_NAME} PROPERTIES
    FOLDER shared/6502
)

add_subdirectory(tests)
