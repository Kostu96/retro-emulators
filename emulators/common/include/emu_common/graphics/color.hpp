#pragma once

namespace EmuCommon {

    struct Color
    {
        constexpr Color() = default;
        constexpr Color(unsigned char r_, unsigned char g_, unsigned char b_, unsigned char a_ = 255) :
            r{ r_ }, g{ g_ }, b{ b_ }, a{ a_ } {}

        unsigned char r = 255;
        unsigned char g = 255;
        unsigned char b = 255;
        unsigned char a = 255;

        static const Color WHITE;
        static const Color BLACK;
        static const Color RED;
        static const Color GREEN;
        static const Color BLUE;
    };

} // namespace EmuCommon
