#pragma once
#include <ccl/types.hpp>

union Instruction
{
    struct
    {
        u8 n1 : 4;
        u8 n2 : 4;
        u8 n3 : 4;
        u8 n4 : 4;
    };
    struct
    {
        u8 h1;
        u8 h2;
    };
    u16 word;
};
