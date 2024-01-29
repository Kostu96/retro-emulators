set(KIM1_TARGET_NAME kim1)

set(KIM1_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/kim1.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/kim1.hpp
)

add_executable(${KIM1_TARGET_NAME} ${KIM1_SOURCES})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX source FILES ${KIM1_SOURCES})

set_target_warnings(${KIM1_TARGET_NAME})
target_compile_options(${KIM1_TARGET_NAME} PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/MP /fp:fast /external:anglebrackets /external:W1>
)

target_link_libraries(${KIM1_TARGET_NAME} PRIVATE
    emu_common_lib
    cpu6502_lib
)

set_target_properties(${KIM1_TARGET_NAME} PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY $<TARGET_FILE_DIR:${KIM1_TARGET_NAME}>
    FOLDER emulators/commodore
)

add_custom_command(
    TARGET ${KIM1_TARGET_NAME}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/rom $<TARGET_FILE_DIR:${KIM1_TARGET_NAME}>/rom/kim1
)
