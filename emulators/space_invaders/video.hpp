#pragma once
#include "utils/types.hpp"

#include <span>

constexpr u16 SCREEN_WIDTH = 224;
constexpr u16 SCREEN_HEIGHT = 256;

class CPU8080;

class Video
{
public:
    Video(CPU8080& cpu, const u8* VRAM) : m_cpuRef{ cpu }, m_VRAM{ VRAM } {};

    void reset();
    void clock();

    std::span<const u32> getScreenPixels() const { return { m_screenPixels, SCREEN_WIDTH * SCREEN_HEIGHT }; }

    Video(const Video&) = delete;
    Video& operator=(const Video&) = delete;
private:
    CPU8080& m_cpuRef;
    const u8* m_VRAM;
    u32 m_screenPixels[SCREEN_WIDTH * SCREEN_HEIGHT];
    u32 m_counter = 0;
};
