#include "chip8_core.hpp"

#include <cstring>

extern "C"
{
    __declspec(dllexport) CHIP8Core* allocator()
    {
        return new CHIP8Core{};
    }

    __declspec(dllexport) void deleter(CHIP8Core* ptr)
    {
        delete ptr;
    }
}

void CHIP8Core::getWindowSettings(WindowSettings& settings)
{
    constexpr u16 CHIP8_WIDTH = 64;
    constexpr u16 CHIP8_HEIGHT = 32;
    constexpr u16 SCREEN_SIZE = CHIP8_WIDTH * CHIP8_HEIGHT;
    constexpr u16 ZOOM = 16;
    constexpr u16 WINDOW_WIDTH = CHIP8_WIDTH * ZOOM;
    constexpr u16 WINDOW_HEIGHT = CHIP8_HEIGHT * ZOOM;
    constexpr u16 BORDER = 20;

    settings.width = WINDOW_WIDTH + BORDER * 2;
    settings.height = WINDOW_HEIGHT + BORDER * 2;
    strcpy_s(settings.title, "chip8");
}
