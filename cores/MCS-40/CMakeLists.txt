set(MCS40_TARGET_NAME mcs40)

set(MCS40_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
    ${CMAKE_CURRENT_SOURCE_DIR}/mcs40_core.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/mcs40_core.hpp
)

add_library(${MCS40_TARGET_NAME} SHARED ${MCS40_SOURCES})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX source FILES ${MCS40_SOURCES})

target_compile_options(${MCS40_TARGET_NAME} PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/MP /fp:fast /external:anglebrackets /external:W1>
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -pedantic>
)

target_link_libraries(${MCS40_TARGET_NAME} PRIVATE
    cpu4040
)

set_target_properties(${MCS40_TARGET_NAME}
    PROPERTIES
    FOLDER cores
)
