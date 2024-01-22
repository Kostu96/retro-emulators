#pragma once
#include "type_aliases.hpp"
#include "mapper.hpp"

#include <memory>
#include <vector>

class Cartridge
{
public:
    void load(const char* filename);

    u8 read(u16 offset);
    void write(u16 offset, u8 data);

    Cartridge() = default;
private:
    std::vector<u8> m_programROM;
    std::vector<u8> m_characterROM;
    std::unique_ptr<Mapper> m_mapper;
};
