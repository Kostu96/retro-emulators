set(EMULATORS_COMMON_LIB_NAME emulators_common_lib)
set(EMULATORS_COMMON_HEADERS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(EMULATORS_COMMON_SOURCES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/source)

set(EMULATORS_COMMON_HEADERS
    ${EMULATORS_COMMON_HEADERS_DIR}/emu_common/gui.hpp
)
set(EMULATORS_COMMON_SOURCES
    ${EMULATORS_COMMON_SOURCES_DIR}/gui.cpp
)

add_library(${EMULATORS_COMMON_LIB_NAME} STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
    ${EMULATORS_COMMON_HEADERS}
    ${EMULATORS_COMMON_SOURCES}
    ${CMAKE_SOURCE_DIR}/third_party/tinyfiledialogs/tinyfiledialogs.c
    ${CMAKE_SOURCE_DIR}/third_party/tinyfiledialogs/tinyfiledialogs.h
)
set_source_files_properties(${CMAKE_SOURCE_DIR}/third_party/tinyfiledialogs/tinyfiledialogs.c PROPERTIES LANGUAGE CXX)
source_group(TREE ${EMULATORS_COMMON_HEADERS_DIR}/emu_common PREFIX source FILES ${EMULATORS_COMMON_HEADERS})
source_group(TREE ${EMULATORS_COMMON_SOURCES_DIR} PREFIX source FILES ${EMULATORS_COMMON_SOURCES})

set_target_warnings(${EMULATORS_COMMON_LIB_NAME})
target_compile_options(${EMULATORS_COMMON_LIB_NAME} PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/MP /fp:fast /external:anglebrackets /external:W1>
)

target_include_directories(${EMULATORS_COMMON_LIB_NAME} PUBLIC
    ${EMULATORS_COMMON_HEADERS_DIR}
    ${CMAKE_SOURCE_DIR}/third_party/tinyfiledialogs
)

target_link_libraries(${EMULATORS_COMMON_LIB_NAME} PUBLIC
    cpp-common-lib
    glfw
    glw
    imgui
)

set_target_properties(${EMULATORS_COMMON_LIB_NAME}
    PROPERTIES
    FOLDER emulators
)
