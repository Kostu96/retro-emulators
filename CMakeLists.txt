cmake_minimum_required(VERSION 3.20)

project(emulators LANGUAGES CXX)
include(scripts/cmake/base_configuration.cmake)
include(scripts/cmake/compiler_warnings.cmake)

add_subdirectory(third_party)
add_subdirectory(cores)

set(APP_TARGET_NAME application)

set(APP_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
    ${CMAKE_CURRENT_SOURCE_DIR}/dll_loader.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/emulator_core.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/shader_sources.inl
    ${CMAKE_CURRENT_SOURCE_DIR}/types.hpp
)

add_executable(${APP_TARGET_NAME} ${APP_SOURCES})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX source FILES ${APP_SOURCES})

set_target_warnings(${APP_TARGET_NAME})

target_compile_options(${APP_TARGET_NAME} PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/MP /fp:fast /external:anglebrackets /external:W1>
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -pedantic>
)

set_target_properties(${APP_TARGET_NAME} PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(${APP_TARGET_NAME} PRIVATE
    glfw
    glw
)
