#pragma once
#include <cstdint>

using u8 = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using s8 = int8_t;
using s16 = int16_t;
using f32 = float;

union Instruction
{
    struct
    {
        u8 n1 : 4;
        u8 n2 : 4;
        u8 n3 : 4;
        u8 n4 : 4;
    } nibbles;
    u16 word;
};
