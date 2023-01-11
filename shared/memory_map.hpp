#pragma once
#include "type_aliases.hpp"

#include <concepts>
#include <functional>

template <typename T>
concept ConstMapable = requires(T object, u16 address) {
    { object.read(address) } -> std::same_as<u8>;
};
template <typename T>
concept Mapable = ConstMapable<T> && requires(T object, u16 address, u8 data) {
    { object.write(address, data) } -> std::same_as<void>;
};

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

struct ReadMapEntry {
    using ReadFunc = std::function<u8(u16)>;

    AddressRange range;
    ReadFunc read;

    ReadMapEntry(AddressRange inRange, ReadFunc inRead) :
        range{ inRange }, read{ inRead } {}
};

struct WriteMapEntry {
    using WriteFunc = std::function<void(u16, u8)>;

    AddressRange range;
    WriteFunc write;

    WriteMapEntry(AddressRange inRange, WriteFunc inWrite) :
        range{ inRange }, write{ inWrite } {}
};
