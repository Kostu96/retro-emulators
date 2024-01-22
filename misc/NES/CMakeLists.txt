set(NES_TARGET_NAME nes)

set(NES_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
    ${CMAKE_CURRENT_SOURCE_DIR}/cartridge.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/cartridge.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/mapper.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/mapper_000.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/mapper_000.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ppu2C02.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ppu2C02.hpp
)

add_executable(${NES_TARGET_NAME} ${NES_SOURCES})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX source FILES ${NES_SOURCES})

set_target_warnings(${NES_TARGET_NAME})

target_compile_options(${NES_TARGET_NAME} PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/MP /fp:fast /external:anglebrackets /external:W1>
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -pedantic>
)

target_link_libraries(${NES_TARGET_NAME} PRIVATE
    cpu6502
    glfw
    glw
)

set_target_properties(${NES_TARGET_NAME} PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

#add_custom_command(
#    TARGET ${NES_TARGET_NAME}
#    POST_BUILD
#    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/assets $<TARGET_FILE_DIR:${NES_TARGET_NAME}>/assets
#)
