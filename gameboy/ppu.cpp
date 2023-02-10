#include "ppu.hpp"

#include <glw/texture.hpp>

#include <cassert>
#include <cstring>
#include <iomanip>
#include <iostream>

static u32 s_colors[4]{
    0xFFDDEEDD,
    0xFF889988,
    0xFF445544,
    0xFF001100,
};

static u8 s_bgColorMap[4]{
    0, 1, 2, 3
};

PPU::PPU(u8& interruptFlagsRef) :
    m_VRAM{ new u8[VRAM_SIZE] },
    m_screenPixels{ new u32[LCD_WIDTH * LCD_HEIGHT] },
    m_tileDataPixels{ new u32[TILE_DATA_WIDTH * TILE_DATA_HEIGHT] },
    m_interruptFlagsRef{ interruptFlagsRef }
{}

PPU::~PPU()
{
    delete[] m_VRAM;

    delete[] m_screenPixels;

    delete[] m_tileDataPixels;
}

void PPU::reset()
{
    m_SCY = 0;
    m_SCX = 0;
    m_LY = 1;
    m_LCDStatus.byte = 0;
    m_LCDStatus.Mode = 2;

    m_fetcherMode = 0;
    m_fetcherTileX = 0;
    m_fetcherTileY = 0;
    m_pixelFIFOEmpty = true;
    m_pixelFIFONeedFetch = true;

    m_colorFIFO.clear();
    m_pixelFIFOPaletteL = 0;
    m_pixelFIFOPaletteH = 0;

    m_currentPixelX = 0;
}

void PPU::clock()
{
    constexpr u16 TICKS_PER_LINE = 114;
    constexpr u16 LINES_PER_FRAME = 154;
    static u16 ticks = 0;
    ticks++;

    auto checkForLYC = [&]() {
        if (m_LY++ == m_LYC) {
            m_LCDStatus.LYCEqLY = 1;
            if (m_LCDStatus.STATSource & 0b1000)
                m_interruptFlagsRef |= 2;
        }
    };

    switch ((Mode)m_LCDStatus.Mode)
    {
    case Mode::HBlank:
        if (ticks >= TICKS_PER_LINE)
        {
            ticks = 0;
            m_LCDStatus.Mode = (u8)((m_LY >= LCD_HEIGHT) ? Mode::VBlank : Mode::OAMSearch);

            checkForLYC();
        }
        break;
    case Mode::VBlank:
        if (m_LY == LCD_HEIGHT + 1 && ticks == 1) {
            m_interruptFlagsRef |= 1;
        }
        if (ticks >= TICKS_PER_LINE)
        {
            ticks = 0;
            if (m_LY >= LINES_PER_FRAME) {
                m_LCDStatus.Mode = (u8)Mode::OAMSearch;
                m_LY = 0;
            }

            checkForLYC();
        }
        break;
    case Mode::OAMSearch:
        if (ticks >= 20)
            m_LCDStatus.Mode = (u8)Mode::PixelTransfer;
        break;
    case Mode::PixelTransfer:
        if (!m_pixelFIFONeedFetch) {
            u8 pixelsPerCycle = 4;
            while (pixelsPerCycle--) {
                u8 paletteL = m_pixelFIFOPaletteL >> 15;
                //u8 paletteH = m_pixelFIFOPaletteH >> 15;
                m_pixelFIFOPaletteL <<= 1;
                m_pixelFIFOPaletteH <<= 1;
                u8 color = m_colorFIFO.pop();
                //u8 palette = (paletteH << 1) | paletteL;
                m_screenPixels[(m_LY - 1) * LCD_WIDTH + m_currentPixelX++] = paletteL ? s_colors[s_bgColorMap[color]] : s_colors[s_bgColorMap[0]];
            }
        }

        static u8 fetchedColorL = 0;
        static u8 fetchedPaletteL = 0;
        switch (m_fetcherMode)
        {
        case 0: {
            u16 tileIndex = ((m_SCY + m_LY - 1) / 8) * 32 + m_SCX / 8 + m_fetcherTileX++;
            u16 tileAddressBase = m_LCDControl.BGTileMap ? 0x1C00 : 0x1800;
            u8 tile = m_VRAM[tileAddressBase + tileIndex];
            m_tileDataAddress = (m_LCDControl.WinBGTileData ? tile : 0x100 + (s8)tile) * 16;
            fetchedColorL = m_VRAM[m_tileDataAddress + ((m_SCY + m_LY - 1) % 8) * 2];
            fetchedPaletteL = m_LCDControl.WinBGEnable ? 0xFF : 0x00; // temp cause only one palette
            m_fetcherMode = 1;
        } break;
        case 1: {
            m_colorFIFO.push(fetchedColorL, m_VRAM[m_tileDataAddress + 1 + ((m_SCY + m_LY - 1) % 8) * 2]);
            m_pixelFIFOPaletteL |= fetchedPaletteL << (m_pixelFIFOEmpty ? 8 : 0);
            m_pixelFIFOPaletteH |= 0; // temp cause only one palette

            if (!m_pixelFIFOEmpty) m_pixelFIFONeedFetch = false;
            if (m_pixelFIFOEmpty) m_pixelFIFOEmpty = false;
            
            m_fetcherMode = 0;
        } break;
        }

        if (m_currentPixelX >= LCD_WIDTH) {
            m_fetcherMode = 0;
            m_fetcherTileX = 0;
            m_currentPixelX = 0;
            m_pixelFIFOEmpty = true;
            m_pixelFIFONeedFetch = true;

            m_colorFIFO.clear();

            m_pixelFIFOPaletteL = 0;
            m_pixelFIFOPaletteH = 0;

            m_LCDStatus.Mode = 0;
        }
        break;
    }
}

void PPU::clearVRAM()
{
    std::memset(m_VRAM, 0, VRAM_SIZE);

    // debug:
    redrawTileData();
}

u8 PPU::loadVRAM8(u16 address) const
{
    return m_VRAM[address];
}

void PPU::storeVRAM8(u16 address, u8 data)
{
    m_VRAM[address] = data;

    // debug:
    if (address < 0x1800) {
        u16 x = (address / 16u) % 16;
        u16 y = (address / 16u) / 16;
        redrawTileData((u8)x, (u8)y, 1, 1);
    }
}

u8 PPU::loadOAM8(u16 address) const
{
    return m_OAM.bytes[address];
}

void PPU::storeOAM8(u16 address, u8 data)
{
    m_OAM.bytes[address] = data;
}

u8 PPU::load8(u16 address) const
{
    switch (address)
    {
    case 0x0: return m_LCDControl.byte;
    case 0x1: return m_LCDStatus.byte;
    case 0x2: return m_SCY;
    case 0x3: return m_SCX;
    case 0x4: return m_LY;
    case 0x5: return m_LYC;

    case 0x7: return m_BGpaletteData;
    case 0x8: return m_OBJpalette0Data;
    case 0x9: return m_OBJpalette1Data;
    case 0xA: return m_WY;
    case 0xB: return m_WX;
    }

    assert(false && "Unhandled");
    return 0;
}

void PPU::store8(u16 address, u8 data)
{
    switch (address)
    {
    case 0x0: m_LCDControl.byte = data; return;
    case 0x1:
        m_LCDStatus.byte &= 0x07;
        m_LCDStatus.byte |= data & 0xF8;
        return;
    case 0x2: m_SCY = data; return;
    case 0x3: m_SCX = data; return;

    case 0x5: m_LYC = data; return;
    case 0x6: return; // TODO: OAM transfer
    case 0x7:
        m_BGpaletteData = data;
        s_bgColorMap[0] = (m_BGpaletteData >> 0) & 0b11;
        s_bgColorMap[1] = (m_BGpaletteData >> 2) & 0b11;
        s_bgColorMap[2] = (m_BGpaletteData >> 4) & 0b11;
        s_bgColorMap[3] = (m_BGpaletteData >> 6) & 0b11;

        // debug:
        redrawTileData();
        return;
    case 0x8: m_OBJpalette0Data = data; return;
    case 0x9: m_OBJpalette1Data = data; return;
    case 0xA: m_WY = data; return;
    case 0xB: m_WX = data; return;
    }

    std::cerr << "Unexpected write to PPU - " << std::hex << std::uppercase << std::setw(4) << std::setfill('0') << address;
    std::cerr << ':' << std::hex << std::setw(2) << (u16)data << '\n';
}

void PPU::redrawTileData(u8 xOffset, u8 yOffset, u8 width, u8 height)
{
    assert(xOffset + width <= 16);
    assert(yOffset + height <= 24);

    for (u8 y = yOffset; y < height + yOffset; y++)
        for (u8 x = xOffset; x < width + xOffset; x++)
        {
            u16 address = y * 256 + x * 16;
            for (u8 tileY = 0; tileY < 16; tileY += 2) {
                u8 b1 = m_VRAM[address + tileY];
                u8 b2 = m_VRAM[address + tileY + 1];
                for (s8 bit = 7; bit >= 0; bit--) {
                    u8 color = (((b2 >> bit) & 1) << 1) | ((b1 >> bit) & 1);
                    u16 x_coord = x * 8 + 7 - bit;
                    u16 y_coord = y * 8 + tileY / 2;
                    u16 line_width = 16 * 8;
                    m_tileDataPixels[y_coord * line_width + x_coord] = s_colors[s_bgColorMap[color]];
                }
            }
        }
}
