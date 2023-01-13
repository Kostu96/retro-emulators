set(APP_TARGET_NAME application)

set(APP_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
    ${CMAKE_CURRENT_SOURCE_DIR}/dll_loader.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui_memory_view.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/shader_sources.inl
)

add_executable(${APP_TARGET_NAME} ${APP_SOURCES})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX source FILES ${APP_SOURCES})

set_target_warnings(${APP_TARGET_NAME})
target_compile_options(${APP_TARGET_NAME} PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/MP /fp:fast /external:anglebrackets /external:W1>
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -pedantic>
)

target_link_libraries(${APP_TARGET_NAME} PRIVATE
    glfw
    glw
    imgui
)

set_target_properties(${APP_TARGET_NAME} PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY $<TARGET_FILE_DIR:${APP_TARGET_NAME}>
)
