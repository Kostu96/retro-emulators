set(PET_TARGET_NAME pet)

set(PET_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
    ${CMAKE_CURRENT_SOURCE_DIR}/roms/basic1_C000.inl
    ${CMAKE_CURRENT_SOURCE_DIR}/roms/basic2_C000.inl
    ${CMAKE_CURRENT_SOURCE_DIR}/roms/basic4_B000.inl
    ${CMAKE_CURRENT_SOURCE_DIR}/roms/charset.inl
    ${CMAKE_CURRENT_SOURCE_DIR}/roms/editor1_E000.inl
    ${CMAKE_CURRENT_SOURCE_DIR}/roms/editor2_E000.inl
    ${CMAKE_CURRENT_SOURCE_DIR}/roms/editor4_E000.inl
    ${CMAKE_CURRENT_SOURCE_DIR}/roms/kernal1_F000.inl
    ${CMAKE_CURRENT_SOURCE_DIR}/roms/kernal2_F000.inl
    ${CMAKE_CURRENT_SOURCE_DIR}/roms/kernal4_F000.inl
    ${CMAKE_CURRENT_SOURCE_DIR}/io.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/io.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/keyboard.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/keyboard.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/pet_core.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/pet_core.hpp
)

add_library(${PET_TARGET_NAME} SHARED ${PET_SOURCES})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX source FILES ${PET_SOURCES})

target_link_libraries(${PET_TARGET_NAME} PRIVATE
    cpu6502
)

set_target_properties(${PET_TARGET_NAME}
    PROPERTIES
    FOLDER cores
)
