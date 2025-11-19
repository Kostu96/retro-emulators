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
