add_subdirectory(asm40xx)

set(CPU40XX_LIB_TARGET_NAME cpu40xx_lib)

set(CPU40XX_LIB_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
    ${CMAKE_CURRENT_SOURCE_DIR}/cpu40xx.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/cpu40xx.hpp
)

add_library(${CPU40XX_LIB_TARGET_NAME} STATIC ${SHARED_SOURCES} ${CPU40XX_LIB_SOURCES})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX source FILES ${CPU40XX_LIB_SOURCES})

target_include_directories(${CPU40XX_LIB_TARGET_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(${CPU40XX_LIB_TARGET_NAME} PUBLIC
    cpp-common-lib
)

set_target_properties(${CPU40XX_LIB_TARGET_NAME} PROPERTIES
    FOLDER shared/40xx
)

add_subdirectory(tests)
