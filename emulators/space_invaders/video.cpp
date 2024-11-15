#include "video.hpp"
#include "shared/source/devices/cpu8080/cpu8080.hpp"

void Video::reset()
{
    m_counter = 0;
}

void Video::clock()
{
    m_counter++;

    constexpr size_t COUNTS_PER_FRAME = 2000000 / 60;
    constexpr size_t COUNTS_PER_HALFFRAME = COUNTS_PER_FRAME / 2;

    auto drawHalf = [this](bool second) {
        for (size_t x = 0; x < SCREEN_WIDTH; x++) {
            for (size_t y = second ? SCREEN_HEIGHT / 16 : 0; y < SCREEN_HEIGHT / (second ? 8 : 16); y++) {
                size_t ptrOffset = x * 32 + (32 - y - 1);
                u8 byte = m_VRAM[ptrOffset];
                for (size_t i = 0; i < 8; i++) {
                    size_t index = (y * 8 + (8 - i) - 1) * SCREEN_WIDTH + x;
                    m_screenPixels[index] = ((byte >> i) & 1) ? 0xFFFFFFFF : 0xFF000000;
                }
            }
        }
    };

    if (m_counter == COUNTS_PER_HALFFRAME) { // TODO(Kostu): do math from 2MHz
        drawHalf(false);
        m_cpuRef.interrupt(0x08);
    }
    else if (m_counter == COUNTS_PER_FRAME) {
        drawHalf(true);
        m_cpuRef.interrupt(0x10);
        m_counter = 0;
    }
}
