set(SOURCE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/bios.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/bios.h
    ${CMAKE_CURRENT_SOURCE_DIR}/cd_rom.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/cd_rom.h
    ${CMAKE_CURRENT_SOURCE_DIR}/cpu.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/cpu.h
    ${CMAKE_CURRENT_SOURCE_DIR}/dma.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/dma.h
    ${CMAKE_CURRENT_SOURCE_DIR}/gpu.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/gpu.h
    ${CMAKE_CURRENT_SOURCE_DIR}/instruction_set.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/instruction_set.h
    ${CMAKE_CURRENT_SOURCE_DIR}/memory_map.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/memory_map.h
    ${CMAKE_CURRENT_SOURCE_DIR}/ram.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ram.h
    ${CMAKE_CURRENT_SOURCE_DIR}/registers.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/registers.h
    ${CMAKE_CURRENT_SOURCE_DIR}/timers.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/timers.h
    ${CMAKE_CURRENT_SOURCE_DIR}/type_helpers.h
    ${CMAKE_CURRENT_SOURCE_DIR}/opengl/buffers.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/opengl/buffers.h
    ${CMAKE_CURRENT_SOURCE_DIR}/opengl/renderer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/opengl/renderer.h
    ${CMAKE_CURRENT_SOURCE_DIR}/opengl/shader.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/opengl/shader.h
)

add_library(${TARGET_NAME_LIB} STATIC
	${SOURCE_FILES}
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX source FILES ${SOURCE_FILES})

target_link_libraries(${TARGET_NAME_LIB}
	cpp-common-lib
	glad
	glfw
)

set_target_properties(${TARGET_NAME_LIB} PROPERTIES
    MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>"
)

if (MSVC)
    set_property(DIRECTORY ${CMAKE_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${TARGET_NAME})
endif()

add_executable(${TARGET_NAME}
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

target_link_libraries(${TARGET_NAME}
    ${TARGET_NAME_LIB}
)

set_target_properties(${TARGET_NAME} PROPERTIES
    MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>"
    VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)
