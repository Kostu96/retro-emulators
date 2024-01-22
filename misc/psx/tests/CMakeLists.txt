set(SOURCE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/instruction_set_tests.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

add_executable(${TARGET_NAME_TESTS}
	${SOURCE_FILES}
)

target_link_libraries(${TARGET_NAME_TESTS}
    gtest
    ${TARGET_NAME_LIB}
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX tests FILES ${SOURCE_FILES})

set_target_properties(${TARGET_NAME_TESTS} PROPERTIES
    MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>"
    VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)
