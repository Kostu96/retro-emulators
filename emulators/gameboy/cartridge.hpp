#pragma once
#include "shared/source/types.hpp"

class Cartridge
{
public:
    ~Cartridge();

    u8 load8(u16 address) const;
    void store8(u16 address, u8 data);
    u8 load8ExtRAM(u16 address) const;
    void store8ExtRAM(u16 address, u8 data);

    bool loadFromFile(const char* filename, bool quiet = false);

    Cartridge() = default;
    Cartridge(const Cartridge&) = delete;
    Cartridge& operator=(const Cartridge&) = delete;
private:
    struct Header {
        u8 entryPoint[4];      // 0x100 - 0x103
        u8 nintendoLogo[0x30]; // 0x104 - 0x133
        char title[15];        // 0x134 - 0x142
        u8 CGBFlag;            // 0x143
        u16 newLicenseeCode;   // 0x144
        u8 SGBFlag;            // 0x146
        u8 cartridgeTypeCode;  // 0x147
        u8 ROMSizeCode;        // 0x148
        u8 RAMSizeCode;        // 0x149
        u8 destinationCode;    // 0x14A
        u8 oldLicenseeCode;    // 0x14B
        u8 version;            // 0x14C
        u8 headerChecksum;     // 0x14D
        u8 globalChecksum[2];  // 0x14E - 0x14F
    };
    static_assert(sizeof(Header) == 0x50);

    Header* m_header = nullptr;
    u8* m_data = nullptr;
    size_t m_size = 0;
    u8 m_MBC1RAMEnable = 0;
    u8 m_MBC1ROMBank = 0;
    u8 m_MBC1RAMBank = 0;
    u8* m_RAM = nullptr;
};
