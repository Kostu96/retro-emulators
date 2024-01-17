set(PET_TARGET_NAME pet)

set(PET_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
    ${CMAKE_CURRENT_SOURCE_DIR}/pet.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/pet.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

add_executable(${PET_TARGET_NAME} ${PET_SOURCES})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX source FILES ${PET_SOURCES})

set_target_warnings(${PET_TARGET_NAME})
target_compile_options(${PET_TARGET_NAME} PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/MP /fp:fast /external:anglebrackets /external:W1>
)

target_link_libraries(${PET_TARGET_NAME} PRIVATE
    emu_common_lib
    cpu6502_lib
)

set_target_properties(${PET_TARGET_NAME} PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY $<TARGET_FILE_DIR:${PET_TARGET_NAME}>
    FOLDER emulators
)
