#pragma once
#include "shared/source/types.hpp"

constexpr u16 SCREEN_WIDTH = 256;
constexpr u16 SCREEN_HEIGHT = 224;

class Video
{
public:
    Video() = default;

    Video(Video&) = delete;
    Video& operator=(Video&) = delete;
private:
    u32 m_screenPixels[SCREEN_WIDTH * SCREEN_HEIGHT];
};
