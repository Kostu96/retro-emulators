set(PET_TARGET_NAME pet)

set(PET_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/pet.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/pet.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/pia6520.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/pia6520.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/via6522.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/via6522.hpp
)

add_executable(${PET_TARGET_NAME}
    ${PET_SOURCES}
    ${CMAKE_SOURCE_DIR}/shared/file_io.cpp
)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX source FILES ${PET_SOURCES})

set_target_warnings(${PET_TARGET_NAME})
target_compile_options(${PET_TARGET_NAME} PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/MP /fp:fast /external:anglebrackets /external:W1>
)

target_link_libraries(${PET_TARGET_NAME} PUBLIC
    cpu6502_lib
    emu_common_lib
)

set_target_properties(${PET_TARGET_NAME} PROPERTIES
    FOLDER emulators/commodore
)
