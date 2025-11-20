set(CALC4004_APP_TARGET_NAME calc4004)

add_emulator_app(TARGET_NAME ${CALC4004_APP_TARGET_NAME}
    SOURCES
        CMakeLists.txt
        src/app.cpp
        src/emulator.cpp
        src/emulator.hpp
    LIBRARIES
        cpu40xx
        emu
)

add_custom_command(
    TARGET ${CALC4004_APP_TARGET_NAME}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/data/. ${CMAKE_CURRENT_BINARY_DIR}/data
)
