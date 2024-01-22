#include "mapper_000.hpp"

u16 Mapper000::map(u16 address) const
{
    address -= (0x4000 - 0x0020);
    if (m_numPRGChunks == 1) address &= 0x3FFF;
    return address;
}
