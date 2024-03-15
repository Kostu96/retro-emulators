set(PSX_TARGET_NAME psx)
set(PSX_LIB_TARGET_NAME ${PSX_TARGET_NAME}_lib)
set(PSX_APP_TARGET_NAME ${PSX_TARGET_NAME}_app)
set(PSX_FOLDER_NAME emulators/psx)

set(PSX_LIB_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
    ${CMAKE_CURRENT_SOURCE_DIR}/cpu.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/cpu.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/psx.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/psx.hpp
)

add_library(${PSX_LIB_TARGET_NAME} STATIC ${PSX_LIB_SOURCES})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX source FILES ${PSX_LIB_SOURCES})

set_target_warnings(${PSX_LIB_TARGET_NAME})
target_compile_options(${PSX_LIB_TARGET_NAME} PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/MP /fp:fast /external:anglebrackets /external:W1>
)

set_target_properties(${PSX_LIB_TARGET_NAME} PROPERTIES
    FOLDER ${PSX_FOLDER_NAME}
)

set(PSX_APP_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

add_executable(${PSX_APP_TARGET_NAME} ${PSX_APP_SOURCES})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX source FILES ${PSX_APP_SOURCES})

set_target_warnings(${PSX_APP_TARGET_NAME})
target_compile_options(${PSX_APP_TARGET_NAME} PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/MP /fp:fast /external:anglebrackets /external:W1>
)

target_link_libraries(${PSX_APP_TARGET_NAME} PRIVATE
    ${PSX_LIB_TARGET_NAME}
    emu_common_lib
)

set_target_properties(${PSX_APP_TARGET_NAME} PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY $<TARGET_FILE_DIR:${PSX_APP_TARGET_NAME}>
    FOLDER ${PSX_FOLDER_NAME}
)
