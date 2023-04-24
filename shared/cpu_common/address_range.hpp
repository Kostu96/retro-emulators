#pragma once
#include <ccl/types.hpp>

struct AddressRange
{
    u16 start;
    u16 end;

    bool contains(u16 address, u16& offset) const
    {
        offset = address - start;
        return (address >= start) && (address <= end);
    }
};
