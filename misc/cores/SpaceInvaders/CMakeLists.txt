set(INVADERS_TARGET_NAME invaders)

set(INVADERS_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
    ${CMAKE_CURRENT_SOURCE_DIR}/invaders_core.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/invaders_core.hpp
)

add_library(${INVADERS_TARGET_NAME} SHARED ${INVADERS_SOURCES})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX source FILES ${INVADERS_SOURCES})

target_link_libraries(${INVADERS_TARGET_NAME} PRIVATE
    cpu_x80_lib
)

set_target_properties(${INVADERS_TARGET_NAME}
    PROPERTIES
    FOLDER cores
)
