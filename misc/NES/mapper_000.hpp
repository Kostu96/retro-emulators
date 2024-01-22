#pragma once
#include "mapper.hpp"

class Mapper000 :
    public Mapper
{
public:
    Mapper000(u8 numPRGChunks, u8 numCHRChunks) :
        Mapper{ numPRGChunks, numCHRChunks } {}

    u16 map(u16 address) const override;
private:

};
