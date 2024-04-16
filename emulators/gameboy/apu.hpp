#pragma once
#include "shared/source/types.hpp"

class APU
{
public:
    APU() = default;

    void reset()
    {
        m_buffer[0x00] = 0x80;
        m_buffer[0x01] = 0xBF;
        m_buffer[0x02] = 0xF3;
        m_buffer[0x03] = 0xFF;
        m_buffer[0x04] = 0xBF;
        m_buffer[0x05] = 0xFF;
        m_buffer[0x06] = 0x3F;
        m_buffer[0x07] = 0x00;
        m_buffer[0x08] = 0xFF;
        m_buffer[0x09] = 0xBF;
        m_buffer[0x0A] = 0x7F;
        m_buffer[0x0B] = 0xFF;
        m_buffer[0x0C] = 0x9F;
        m_buffer[0x0D] = 0xFF;
        m_buffer[0x0E] = 0xBF;
        m_buffer[0x0F] = 0xFF;
        m_buffer[0x10] = 0xFF;
        m_buffer[0x11] = 0x00;
        m_buffer[0x12] = 0x00;
        m_buffer[0x13] = 0xBF;
        m_buffer[0x14] = 0x77;
        m_buffer[0x15] = 0xF3;
        m_buffer[0x16] = 0xF1;
    }

    u8 load8(u16 address)
    {
        return m_buffer[address];
    }
private:
    u8 m_buffer[0x17];
};
