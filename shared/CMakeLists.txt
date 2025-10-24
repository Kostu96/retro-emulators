set(SHARED_LIB_TARGET_NAME shared_lib)
set(SHARED_LIB_TESTS_TARGET_NAME ${SHARED_LIB_TARGET_NAME}_tests)

set(SHARED_LIB_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/source/imgui/debug_view.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/source/imgui/debug_view.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/source/imgui/disassembly_view.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/source/imgui/disassembly_view.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/source/imgui/imgui_helper.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/source/imgui/imgui_helper.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/source/imgui/memory_view.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/source/imgui/memory_view.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/source/application.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/source/application.hpp
)

set(SHARED_LIB_3RD_PARTY
    ${CMAKE_SOURCE_DIR}/third_party/tinyfiledialogs/tinyfiledialogs.c
    ${CMAKE_SOURCE_DIR}/third_party/tinyfiledialogs/tinyfiledialogs.h
)

add_library(${SHARED_LIB_TARGET_NAME} STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
    ${SHARED_LIB_SOURCES}
    ${SHARED_LIB_3RD_PARTY}
)

set_source_files_properties(${CMAKE_SOURCE_DIR}/third_party/tinyfiledialogs/tinyfiledialogs.c PROPERTIES LANGUAGE CXX)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/source PREFIX source FILES ${SHARED_LIB_SOURCES})
source_group(TREE ${CMAKE_SOURCE_DIR}/third_party PREFIX third_party FILES ${SHARED_LIB_3RD_PARTY})

set_target_warnings(${SHARED_LIB_TARGET_NAME})
target_compile_options(${SHARED_LIB_TARGET_NAME} PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/MP /fp:fast /external:anglebrackets /external:W1>
)

target_include_directories(${SHARED_LIB_TARGET_NAME} PUBLIC
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/third_party/tinyfiledialogs
)

target_link_libraries(${SHARED_LIB_TARGET_NAME} PUBLIC
    glfw
    glw
    imgui
)
