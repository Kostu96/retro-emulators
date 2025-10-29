set(VIC20_TARGET_NAME vic20)

set(VIC20_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/vic20.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/vic20.hpp
)

add_executable(${VIC20_TARGET_NAME}
    ${VIC20_SOURCES}
)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX source FILES ${VIC20_SOURCES})

set_target_warnings(${VIC20_TARGET_NAME})
target_compile_options(${VIC20_TARGET_NAME} PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/MP /fp:fast /external:anglebrackets /external:W1>
)

target_link_libraries(${VIC20_TARGET_NAME} PRIVATE
    shared_lib
    utils
    cpu6502
)

set_target_properties(${VIC20_TARGET_NAME} PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY $<TARGET_FILE_DIR:${VIC20_TARGET_NAME}>
    FOLDER emulators/commodore
)
