#pragma once
#include "utils/types.hpp"

class IO
{
public:
    IO() = default;

    IO(const IO&) = delete;
    IO& operator=(const IO&) = delete;

    u8 read8(u8 port) const;
    void write8(u8 port, u8 data);
private:
    u8 m_shiftAmount : 3;
    u16 m_shiftRegister;
};
