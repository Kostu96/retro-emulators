#include "chip8_core.hpp"

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
