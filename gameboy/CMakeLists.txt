set(GAMEBOY_TARGET_NAME gameboy)

set(GAMEBOY_LIB_TARGET_NAME ${GAMEBOY_TARGET_NAME}_lib)
set(GAMEBOY_LIB_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
    ${CMAKE_CURRENT_SOURCE_DIR}/bit_fifo.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/bit_fifo.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/cartridge.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/cartridge.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/gameboy.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/gameboy.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ppu.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ppu.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/timer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/timer.hpp
)

add_library(${GAMEBOY_LIB_TARGET_NAME} STATIC ${GAMEBOY_LIB_SOURCES})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX source FILES ${GAMEBOY_LIB_SOURCES})

set_target_warnings(${GAMEBOY_LIB_TARGET_NAME})
target_compile_options(${GAMEBOY_LIB_TARGET_NAME} PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/MP /fp:fast /external:anglebrackets /external:W1>
)

target_link_libraries(${GAMEBOY_LIB_TARGET_NAME} PUBLIC
    cpp-common-lib
    cpu8080_lib
)

set_target_properties(${GAMEBOY_LIB_TARGET_NAME} PROPERTIES
    FOLDER gameboy
)

set(GAMEBOY_APP_TARGET_NAME ${GAMEBOY_TARGET_NAME}_app)
set(GAMEBOY_APP_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
    ${CMAKE_CURRENT_SOURCE_DIR}/gui.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/gui.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

add_executable(${GAMEBOY_APP_TARGET_NAME}
    ${GAMEBOY_APP_SOURCES}
    ${CMAKE_SOURCE_DIR}/third_party/tinyfiledialogs/tinyfiledialogs.c
    ${CMAKE_SOURCE_DIR}/third_party/tinyfiledialogs/tinyfiledialogs.h
)
set_source_files_properties(${CMAKE_SOURCE_DIR}/third_party/tinyfiledialogs/tinyfiledialogs.c PROPERTIES LANGUAGE CXX)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX source FILES ${GAMEBOY_APP_SOURCES})

set_target_warnings(${GAMEBOY_APP_TARGET_NAME})
target_compile_options(${GAMEBOY_APP_TARGET_NAME} PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/MP /fp:fast /external:anglebrackets /external:W1>
)

target_include_directories(${GAMEBOY_APP_TARGET_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/third_party/tinyfiledialogs
)

target_link_libraries(${GAMEBOY_APP_TARGET_NAME} PRIVATE
    ${GAMEBOY_LIB_TARGET_NAME}
    glfw
    glw
    imgui
)

set_target_properties(${GAMEBOY_APP_TARGET_NAME} PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY $<TARGET_FILE_DIR:${GAMEBOY_APP_TARGET_NAME}>
    FOLDER gameboy
)

add_subdirectory(tests)
