set(GAMEBOY_TAGET_NAME gameboy)

set(GAMEBOY_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

add_executable(${GAMEBOY_TAGET_NAME} ${GAMEBOY_SOURCES})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX source FILES ${GAMEBOY_SOURCES})

set_target_warnings(${GAMEBOY_TAGET_NAME})
target_compile_options(${GAMEBOY_TAGET_NAME} PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/MP /fp:fast /external:anglebrackets /external:W1>
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -pedantic>
)

target_link_libraries(${GAMEBOY_TAGET_NAME} PRIVATE
    glfw
    glw
    imgui
    cpu8080_lib
)

set_target_properties(${GAMEBOY_TAGET_NAME} PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY $<TARGET_FILE_DIR:${GAMEBOY_TAGET_NAME}>
)
