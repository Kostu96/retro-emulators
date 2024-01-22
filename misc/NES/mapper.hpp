#pragma once
#include "type_aliases.hpp"

class Mapper
{
public:
    Mapper(u8 numPRGChunks, u8 numCHRChunks) :
        m_numPRGChunks{ numPRGChunks },
        m_numCHRChunks{ numCHRChunks } {}

    virtual ~Mapper() = default;

    virtual u16 map(u16 address) const = 0;
protected:
    u8 m_numPRGChunks;
    u8 m_numCHRChunks;
};
