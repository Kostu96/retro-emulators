set(EMULATORS_COMMON_LIB_NAME emulators_common_lib)

set(EMULATORS_COMMON_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
    ${CMAKE_CURRENT_SOURCE_DIR}/gui.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/gui.hpp
)

add_library(${EMULATORS_COMMON_LIB_NAME} STATIC
    ${EMULATORS_COMMON_SOURCES}
    ${CMAKE_SOURCE_DIR}/third_party/tinyfiledialogs/tinyfiledialogs.c
    ${CMAKE_SOURCE_DIR}/third_party/tinyfiledialogs/tinyfiledialogs.h
)
set_source_files_properties(${CMAKE_SOURCE_DIR}/third_party/tinyfiledialogs/tinyfiledialogs.c PROPERTIES LANGUAGE CXX)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX source FILES ${EMULATORS_COMMON_SOURCES})

set_target_warnings(${EMULATORS_COMMON_LIB_NAME})
target_compile_options(${EMULATORS_COMMON_LIB_NAME} PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/MP /fp:fast /external:anglebrackets /external:W1>
)

target_include_directories(${EMULATORS_COMMON_LIB_NAME} PUBLIC
    ${CMAKE_SOURCE_DIR}/third_party/tinyfiledialogs
)

target_link_libraries(${EMULATORS_COMMON_LIB_NAME} PUBLIC
    glfw
    glw
    imgui
)

set_target_properties(${EMULATORS_COMMON_LIB_NAME}
    PROPERTIES
    FOLDER emulators
)
