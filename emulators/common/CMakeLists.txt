set(EMULATORS_COMMON_LIB_NAME emulators_common_lib)
set(EMULATORS_COMMON_HEADERS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(EMULATORS_COMMON_SOURCES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/source)

set(EMULATORS_COMMON_HEADERS
    ${EMULATORS_COMMON_HEADERS_DIR}/emu_common/SDL/font.hpp
    ${EMULATORS_COMMON_HEADERS_DIR}/emu_common/SDL/rect_shape.hpp
    ${EMULATORS_COMMON_HEADERS_DIR}/emu_common/SDL/renderable.hpp
    ${EMULATORS_COMMON_HEADERS_DIR}/emu_common/SDL/sprite.hpp
    ${EMULATORS_COMMON_HEADERS_DIR}/emu_common/SDL/text.hpp
    ${EMULATORS_COMMON_HEADERS_DIR}/emu_common/SDL/texture.hpp
    ${EMULATORS_COMMON_HEADERS_DIR}/emu_common/SDL/transformable.hpp
    ${EMULATORS_COMMON_HEADERS_DIR}/emu_common/application.hpp
    ${EMULATORS_COMMON_HEADERS_DIR}/emu_common/gui.hpp
    ${EMULATORS_COMMON_HEADERS_DIR}/emu_common/rect.hpp
    ${EMULATORS_COMMON_HEADERS_DIR}/emu_common/vec2.hpp
)
set(EMULATORS_COMMON_SOURCES
    ${EMULATORS_COMMON_SOURCES_DIR}/SDL/font.cpp
    ${EMULATORS_COMMON_SOURCES_DIR}/SDL/rect_shape.cpp
    ${EMULATORS_COMMON_SOURCES_DIR}/SDL/sprite.cpp
    ${EMULATORS_COMMON_SOURCES_DIR}/SDL/text.cpp
    ${EMULATORS_COMMON_SOURCES_DIR}/SDL/texture.cpp
    ${EMULATORS_COMMON_SOURCES_DIR}/SDL/transformable.cpp
    ${EMULATORS_COMMON_SOURCES_DIR}/application.cpp
    ${EMULATORS_COMMON_SOURCES_DIR}/gui.cpp
)
set(EMULATORS_COMMON_3RD_PARTY
    ${CMAKE_SOURCE_DIR}/third_party/tinyfiledialogs/tinyfiledialogs.c
    ${CMAKE_SOURCE_DIR}/third_party/tinyfiledialogs/tinyfiledialogs.h
)

add_library(${EMULATORS_COMMON_LIB_NAME} STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
    ${EMULATORS_COMMON_HEADERS}
    ${EMULATORS_COMMON_SOURCES}
    ${EMULATORS_COMMON_3RD_PARTY}
)
set_source_files_properties(${CMAKE_SOURCE_DIR}/third_party/tinyfiledialogs/tinyfiledialogs.c PROPERTIES LANGUAGE CXX)
source_group(TREE ${EMULATORS_COMMON_HEADERS_DIR}/emu_common PREFIX source FILES ${EMULATORS_COMMON_HEADERS})
source_group(TREE ${EMULATORS_COMMON_SOURCES_DIR} PREFIX source FILES ${EMULATORS_COMMON_SOURCES})
source_group(TREE ${CMAKE_SOURCE_DIR}/third_party PREFIX third_party FILES ${EMULATORS_COMMON_3RD_PARTY})

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
    SDL2-static
    SDL2main
    SDL2_image
    SDL2_ttf
    glfw
    glw
    imgui
)

set_target_properties(${EMULATORS_COMMON_LIB_NAME}
    PROPERTIES
    FOLDER emulators
)
