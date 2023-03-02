set(EMULATORS_COMMON_LIB_NAME emulators_common_lib)
set(EMULATORS_COMMON_HEADERS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(EMULATORS_COMMON_SOURCES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/source)

set(EMULATORS_COMMON_HEADERS
    ${EMULATORS_COMMON_HEADERS_DIR}/emu_common/graphics/color.hpp
    ${EMULATORS_COMMON_HEADERS_DIR}/emu_common/graphics/font.hpp
    ${EMULATORS_COMMON_HEADERS_DIR}/emu_common/graphics/rect.hpp
    ${EMULATORS_COMMON_HEADERS_DIR}/emu_common/graphics/rect_shape.hpp
    ${EMULATORS_COMMON_HEADERS_DIR}/emu_common/graphics/renderable.hpp
    ${EMULATORS_COMMON_HEADERS_DIR}/emu_common/graphics/sprite.hpp
    ${EMULATORS_COMMON_HEADERS_DIR}/emu_common/graphics/text.hpp
    ${EMULATORS_COMMON_HEADERS_DIR}/emu_common/graphics/texture.hpp
    ${EMULATORS_COMMON_HEADERS_DIR}/emu_common/graphics/transformable.hpp
    ${EMULATORS_COMMON_HEADERS_DIR}/emu_common/gui/button.hpp
    ${EMULATORS_COMMON_HEADERS_DIR}/emu_common/gui/label.hpp
    ${EMULATORS_COMMON_HEADERS_DIR}/emu_common/gui/switch.hpp
    ${EMULATORS_COMMON_HEADERS_DIR}/emu_common/gui/widget.hpp
    ${EMULATORS_COMMON_HEADERS_DIR}/emu_common/application.hpp
    ${EMULATORS_COMMON_HEADERS_DIR}/emu_common/imgui_helper.hpp
    ${EMULATORS_COMMON_HEADERS_DIR}/emu_common/vec2.hpp
)
set(EMULATORS_COMMON_SOURCES
    ${EMULATORS_COMMON_SOURCES_DIR}/graphics/color.cpp
    ${EMULATORS_COMMON_SOURCES_DIR}/graphics/font.cpp
    ${EMULATORS_COMMON_SOURCES_DIR}/graphics/rect_shape.cpp
    ${EMULATORS_COMMON_SOURCES_DIR}/graphics/sprite.cpp
    ${EMULATORS_COMMON_SOURCES_DIR}/graphics/text.cpp
    ${EMULATORS_COMMON_SOURCES_DIR}/graphics/texture.cpp
    ${EMULATORS_COMMON_SOURCES_DIR}/graphics/transformable.cpp
    ${EMULATORS_COMMON_SOURCES_DIR}/gui/button.cpp
    ${EMULATORS_COMMON_SOURCES_DIR}/gui/label.cpp
    ${EMULATORS_COMMON_SOURCES_DIR}/gui/switch.cpp
    ${EMULATORS_COMMON_SOURCES_DIR}/application.cpp
    ${EMULATORS_COMMON_SOURCES_DIR}/imgui_helper.cpp
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
