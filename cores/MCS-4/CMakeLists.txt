set(MCS4_TARGET_NAME mcs4)

set(MCS4_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
    ${CMAKE_CURRENT_SOURCE_DIR}/mcs4_core.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/mcs4_core.hpp
)

add_library(${MCS4_TARGET_NAME} SHARED ${MCS4_SOURCES})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX source FILES ${MCS4_SOURCES})

target_compile_options(${MCS4_TARGET_NAME} PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/MP /fp:fast /external:anglebrackets /external:W1>
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -pedantic>
)

target_link_libraries(${MCS4_TARGET_NAME} PRIVATE
    cpu40xx_lib
    asm40xx_lib
)

set_target_properties(${MCS4_TARGET_NAME}
    PROPERTIES
    FOLDER cores
)
