set(MCS8_TARGET_NAME mcs8)

set(MCS8_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
    ${CMAKE_CURRENT_SOURCE_DIR}/mcs8_core.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/mcs8_core.hpp
)

add_library(${MCS8_TARGET_NAME} SHARED ${MCS8_SOURCES})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX source FILES ${MCS8_SOURCES})

target_compile_options(${MCS8_TARGET_NAME} PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/MP /fp:fast /external:anglebrackets /external:W1>
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -pedantic>
)

target_link_libraries(${MCS8_TARGET_NAME} PRIVATE
    cpu8008
)

set_target_properties(${MCS8_TARGET_NAME}
    PROPERTIES
    FOLDER cores
)
