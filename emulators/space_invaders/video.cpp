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
        m_cpuRef.interrupt(0x08);
    }
    else if (m_counter == 33333) {
        m_cpuRef.interrupt(0x10);
        m_counter = 0;
    }
}
