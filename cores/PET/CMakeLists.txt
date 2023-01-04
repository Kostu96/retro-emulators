set(PET_TARGET_NAME pet)

set(PET_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
    ${CMAKE_CURRENT_SOURCE_DIR}/pet_core.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/pet_core.hpp
)

add_library(${PET_TARGET_NAME} SHARED ${PET_SOURCES})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX source FILES ${PET_SOURCES})

set_target_properties(${PET_TARGET_NAME}
    PROPERTIES
    FOLDER cores
)
