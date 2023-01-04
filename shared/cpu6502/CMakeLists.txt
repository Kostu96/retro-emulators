set(CPU6502_TARGET_NAME cpu6502)

add_library(${CPU6502_TARGET_NAME} STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/cpu6502.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/cpu6502.hpp
)

target_include_directories(${CPU6502_TARGET_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(${CPU6502_TARGET_NAME} PROPERTIES
    FOLDER tests
)

### --- ### --- ### --- ### --- ###

add_executable(test6502_functional
    ${CMAKE_CURRENT_SOURCE_DIR}/test_functional.cpp
)

target_link_libraries(test6502_functional PRIVATE
    cpu6502
)

add_executable(test6502_interrupt
    ${CMAKE_CURRENT_SOURCE_DIR}/test_interrupt.cpp
)

target_link_libraries(test6502_interrupt PRIVATE
    cpu6502
)

set_target_properties(
    test6502_functional
    test6502_interrupt
    PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    FOLDER tests
)
