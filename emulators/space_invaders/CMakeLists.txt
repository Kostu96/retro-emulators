set(INVADERS_TARGET_NAME space_invaders)

set(INVADERS_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
    ${CMAKE_CURRENT_SOURCE_DIR}/invaders.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/invaders.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

add_executable(${INVADERS_TARGET_NAME}
    ${INVADERS_SOURCES}
)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX source FILES ${INVADERS_SOURCES})

target_link_libraries(${INVADERS_TARGET_NAME} PRIVATE
    shared_lib
)

set_target_properties(${INVADERS_TARGET_NAME} PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY $<TARGET_FILE_DIR:${INVADERS_TARGET_NAME}>
    FOLDER emulators
)
