#pragma once
#include "shared/source/types.hpp"

#include <span>

constexpr u16 SCREEN_WIDTH = 256;
constexpr u16 SCREEN_HEIGHT = 224;

class CPU8080;

class Video
{
public:
    Video(CPU8080& cpu) : m_cpuRef{ cpu } {};

    void reset();
    void clock();

    std::span<const u32> getScreenPixels() const { return { m_screenPixels, SCREEN_WIDTH * SCREEN_HEIGHT }; }

    Video(const Video&) = delete;
    Video& operator=(const Video&) = delete;
private:
    CPU8080& m_cpuRef;
    u32 m_screenPixels[SCREEN_WIDTH * SCREEN_HEIGHT];
    u32 m_counter = 0;
};
