#pragma once
#include "types.hpp"

template<typename T>
struct AddressRange
{
    T start;
    T end;

    bool contains(T address, T& offset) const
    {
        offset = address - start;
        return (address >= start) && (address <= end);
    }
};

using AddressRange16 = AddressRange<u16>;
using AddressRange32 = AddressRange<u32>;
