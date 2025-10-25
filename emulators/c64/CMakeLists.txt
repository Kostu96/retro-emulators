set(C64_TARGET_NAME c64)

set(C64_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
    ${CMAKE_CURRENT_SOURCE_DIR}/c64.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/c64.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/cia.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/cia.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sid.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sid.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/vic_ii.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/vic_ii.hpp
)

add_executable(${C64_TARGET_NAME}
    ${C64_SOURCES}
)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX source FILES ${C64_SOURCES})

set_target_warnings(${C64_TARGET_NAME})
target_compile_options(${C64_TARGET_NAME} PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/MP /fp:fast /external:anglebrackets /external:W1>
)

target_link_libraries(${C64_TARGET_NAME} PRIVATE
    shared_lib
    utils
    cpu6502
)

set_target_properties(${C64_TARGET_NAME} PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY $<TARGET_FILE_DIR:${C64_TARGET_NAME}>
    FOLDER emulators/commodore
)
