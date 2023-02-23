set(EMU_APP_TARGET_NAME emulator_app)

set(EMU_APP_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
    ${CMAKE_CURRENT_SOURCE_DIR}/dll_loader.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/gui.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/gui.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/renderer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/renderer.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/shader_sources.inl
)

add_executable(${EMU_APP_TARGET_NAME} ${EMU_APP_SOURCES})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX source FILES ${EMU_APP_SOURCES})

set_target_warnings(${EMU_APP_TARGET_NAME})
target_compile_options(${EMU_APP_TARGET_NAME} PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/MP /fp:fast /external:anglebrackets /external:W1>
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -pedantic>
)

target_link_libraries(${EMU_APP_TARGET_NAME} PRIVATE
    glfw
    glw
    imgui
)

set_target_properties(${EMU_APP_TARGET_NAME} PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY $<TARGET_FILE_DIR:${EMU_APP_TARGET_NAME}>
)
