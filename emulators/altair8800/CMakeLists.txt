set(ALTAIR_TARGET_NAME altair8800)
set(ALTAIR_FOLDER_NAME emulators/altair)

set(ALTAIR_LIB_TARGET_NAME ${ALTAIR_TARGET_NAME}_lib)
set(ALTAIR_LIB_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
    ${CMAKE_CURRENT_SOURCE_DIR}/altair.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/altair.hpp
)

add_library(${ALTAIR_LIB_TARGET_NAME} STATIC ${ALTAIR_LIB_SOURCES})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX source FILES ${ALTAIR_LIB_SOURCES})

set_target_warnings(${ALTAIR_LIB_TARGET_NAME})
target_compile_options(${ALTAIR_LIB_TARGET_NAME} PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/MP /fp:fast /external:anglebrackets /external:W1>
)

target_link_libraries(${ALTAIR_LIB_TARGET_NAME} PUBLIC
    cpp-common-lib
    cpu_x80_lib
)

set_target_properties(${ALTAIR_LIB_TARGET_NAME} PROPERTIES
    FOLDER ${ALTAIR_FOLDER_NAME}
)

set(ALTAIR_APP_TARGET_NAME ${ALTAIR_TARGET_NAME}_app)
set(ALTAIR_APP_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
    ${CMAKE_CURRENT_SOURCE_DIR}/gui.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/gui.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

add_executable(${ALTAIR_APP_TARGET_NAME} ${ALTAIR_APP_SOURCES})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX source FILES ${ALTAIR_APP_SOURCES})

set_target_warnings(${ALTAIR_APP_TARGET_NAME})
target_compile_options(${ALTAIR_APP_TARGET_NAME} PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/MP /fp:fast /external:anglebrackets /external:W1>
)

target_link_libraries(${ALTAIR_APP_TARGET_NAME} PRIVATE
    ${ALTAIR_LIB_TARGET_NAME}
    emulators_common_lib
)

set_target_properties(${ALTAIR_APP_TARGET_NAME} PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY $<TARGET_FILE_DIR:${ALTAIR_APP_TARGET_NAME}>
    FOLDER ${ALTAIR_FOLDER_NAME}
)
