#include "video.hpp"
#include "shared/source/devices/cpu8080/cpu8080.hpp"

void Video::reset()
{
    m_counter = 0;
}

void Video::clock()
{
    m_counter++;

    if (m_counter == 16666) { // TODO(Kostu): do math from 2MHz
        size_t index = 0;
        for (const u8* ptr = m_VRAM; ptr < m_VRAM + 0xE00; ptr++) {
            u8 byte = *ptr;
            while (byte) {
                m_screenPixels[index++] = (byte & 1) ? 0xFFFFFFFF : 0xFF000000;
                byte >>= 1;
            }
        }
        m_cpuRef.interrupt(0x08);
    }
    else if (m_counter == 33333) {
        size_t index = 28672;
        for (const u8* ptr = m_VRAM + 0xE00; ptr < m_VRAM + 0x1C00; ptr++) {
            u8 byte = *ptr;
            while (byte) {
                m_screenPixels[index++] = (byte & 1) ? 0xFFFFFFFF : 0xFF000000;
                byte >>= 1;
            }
        }
        m_cpuRef.interrupt(0x10);
        m_counter = 0;
    }
}
