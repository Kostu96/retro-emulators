set(APPLE2_TARGET_NAME apple2)

set(APPLE2_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
    ${CMAKE_CURRENT_SOURCE_DIR}/apple2.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/apple2.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

add_executable(${APPLE2_TARGET_NAME}
    ${APPLE2_SOURCES}
)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX source FILES ${APPLE2_SOURCES})

set_target_warnings(${APPLE2_TARGET_NAME})
target_compile_options(${APPLE2_TARGET_NAME} PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/MP /fp:fast /external:anglebrackets /external:W1>
)

target_link_libraries(${APPLE2_TARGET_NAME} PRIVATE
    emu_common_lib
    cpu6502_lib
)

set_target_properties(${APPLE2_TARGET_NAME} PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY $<TARGET_FILE_DIR:${APPLE2_TARGET_NAME}>
    FOLDER emulators
)
