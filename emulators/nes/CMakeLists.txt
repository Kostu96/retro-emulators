set(NES_TARGET_NAME nes)

set(NES_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
    ${CMAKE_CURRENT_SOURCE_DIR}/nes.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/nes.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

add_executable(${NES_TARGET_NAME} ${NES_SOURCES})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX source FILES ${NES_SOURCES})

set_target_warnings(${NES_TARGET_NAME})
target_compile_options(${NES_TARGET_NAME} PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/MP /fp:fast /external:anglebrackets /external:W1>
)

target_link_libraries(${NES_TARGET_NAME} PRIVATE
    emu_common_lib
    cpu6502_lib
)

set_target_properties(${NES_TARGET_NAME} PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY $<TARGET_FILE_DIR:${NES_TARGET_NAME}>
    FOLDER emulators
)
