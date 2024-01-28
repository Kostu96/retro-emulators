set(PET_TARGET_NAME pet)
set(PET_LIB_TARGET_NAME ${PET_TARGET_NAME}_lib)
set(PET_APP_TARGET_NAME ${PET_TARGET_NAME}_app)
set(PET_FOLDER_NAME emulators/commodore)

set(PET_LIB_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
    ${CMAKE_CURRENT_SOURCE_DIR}/pet.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/pet.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/pia6520.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/pia6520.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/via6522.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/via6522.hpp
)

add_library(${PET_LIB_TARGET_NAME} STATIC ${PET_LIB_SOURCES})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX source FILES ${PET_LIB_SOURCES})

set_target_warnings(${PET_LIB_TARGET_NAME})
target_compile_options(${PET_LIB_TARGET_NAME} PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/MP /fp:fast /external:anglebrackets /external:W1>
)

target_link_libraries(${PET_LIB_TARGET_NAME} PUBLIC
    cpp-common-lib
    cpu6502_lib
)

set_target_properties(${PET_LIB_TARGET_NAME} PROPERTIES
    FOLDER ${PET_FOLDER_NAME}
)

set(PET_APP_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

add_executable(${PET_APP_TARGET_NAME} ${PET_APP_SOURCES})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX source FILES ${PET_APP_SOURCES})

set_target_warnings(${PET_APP_TARGET_NAME})
target_compile_options(${PET_APP_TARGET_NAME} PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/MP /fp:fast /external:anglebrackets /external:W1>
)

target_link_libraries(${PET_APP_TARGET_NAME} PRIVATE
    ${PET_LIB_TARGET_NAME}
    emu_common_lib
)

set_target_properties(${PET_APP_TARGET_NAME} PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY $<TARGET_FILE_DIR:${PET_APP_TARGET_NAME}>
    FOLDER ${PET_FOLDER_NAME}
)
