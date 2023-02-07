#include "ppu.hpp"

#include <glw/glw.hpp>

#include <cassert>
#include <cstring>

static u32 s_colors[4]{
    0xFFDDDDDD,
    0xFF999999,
    0xFF444444,
    0xFF000000,
};

static u8 s_bgColorMap[4]{
    0, 1, 2, 3
};

PPU::PPU() :
    m_VRAM{ new u8[VRAM_SIZE] },
    m_tileDataFBO{ new glw::Framebuffer{
        glw::Framebuffer::Properties{ TILE_DATA_FRAME_WIDTH + 15 + 2, TILE_DATA_FRAME_HEIGHT + 23 + 2, 1, {
                glw::TextureSpecification{
                    glw::TextureFormat::RGBA8,
                    glw::TextureFilter::Nearest,
                    glw::TextureFilter::Nearest,
                    glw::TextureWrapMode::Clamp
                }
            } } } },
    m_tileMap0FBO{ new glw::Framebuffer{
        glw::Framebuffer::Properties{ TILEMAP_FRAME_SIZE, TILEMAP_FRAME_SIZE, 1, {
                glw::TextureSpecification{
                    glw::TextureFormat::RGBA8,
                    glw::TextureFilter::Nearest,
                    glw::TextureFilter::Nearest,
                    glw::TextureWrapMode::Repeat
                }
            } } } },
    m_tileMap1FBO{ new glw::Framebuffer{
        glw::Framebuffer::Properties{ TILEMAP_FRAME_SIZE, TILEMAP_FRAME_SIZE, 1, {
                glw::TextureSpecification{
                    glw::TextureFormat::RGBA8,
                    glw::TextureFilter::Nearest,
                    glw::TextureFilter::Nearest,
                    glw::TextureWrapMode::Repeat
                }
            } } } }
{}

PPU::~PPU()
{
    delete[] m_VRAM;

    delete m_tileDataFBO;
    delete m_tileMap0FBO;
    delete m_tileMap1FBO;
}

void PPU::reset()
{
    m_SCY = 0;
    m_SCX = 0;
    m_LY = 0;

    m_isTileDataDirty = true;
}

void PPU::clock()
{
    static u16 lineTicks = 0;

    switch (m_LCDStatus.Mode)
    {
    case 0: // H Blank
        if (lineTicks >= 456)
        {
            lineTicks = 0;
            m_LCDStatus.LYCEqLY = (++m_LY == m_LYC);
            m_LCDStatus.Mode = (m_LY == 144) ? 1 : 2;
        }
        break;
    case 1: // V Blank
        if (lineTicks >= 456)
        {
            lineTicks = 0;
            m_LCDStatus.LYCEqLY = (++m_LY == m_LYC);

            if (m_LY >= 153)
            {
                m_LCDStatus.Mode = 2;
                m_LY = 0;
            }
        }
        break;
    case 2: // OAM Search
        if (lineTicks >= 80)
            m_LCDStatus.Mode = 3;
        break;
    case 3: // Data Transfer
        if (lineTicks >= 80 + 168)
            m_LCDStatus.Mode = 0;
        break;
    }

    lineTicks++;

    if (m_isTileDataDirty) { m_isTileDataDirty = false; redrawTileData(); }

    if (m_LY == 144)
    {
        glw::Renderer::beginFrame(m_tileMap0FBO);
        redrawTileMap(0x1800);
        glw::Renderer::renderLine(m_SCX, m_SCY, m_SCX, m_SCY + 144, 0xFF002255);
        glw::Renderer::renderLine(m_SCX + 160, m_SCY, m_SCX + 160, m_SCY + 144, 0xFF002255);
        glw::Renderer::renderLine(m_SCX, m_SCY, m_SCX + 160, m_SCY, 0xFF002255);
        glw::Renderer::renderLine(m_SCX, m_SCY + 144, m_SCX + 160, m_SCY + 144, 0xFF002255);
        glw::Renderer::endFrame();

        glw::Renderer::beginFrame(m_tileMap1FBO);
        redrawTileMap(0x1C00);
        glw::Renderer::endFrame();
    }
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
        m_LCDStatus.byte &= 0x0F;
        m_LCDStatus.byte |= data & 0xF0;
        return;
    case 0x2: m_SCY = data; return;
    case 0x3: m_SCX = data; return;

    case 0x5: m_LYC = data; return;

    case 0x7:
        m_BGpaletteData = data;
        s_bgColorMap[0] = (m_BGpaletteData >> 0) & 0b11;
        s_bgColorMap[1] = (m_BGpaletteData >> 2) & 0b11;
        s_bgColorMap[2] = (m_BGpaletteData >> 4) & 0b11;
        s_bgColorMap[3] = (m_BGpaletteData >> 6) & 0b11;
        return;
    case 0x8: m_OBJpalette0Data = data; return;
    case 0x9: m_OBJpalette1Data = data; return;
    case 0xA: m_WY = data; return;
    case 0xB: m_WX = data; return;
    }

    assert(false && "Unhandled");
}

u8 PPU::loadVRAM8(u16 address) const
{
    return m_VRAM[address];
}

void PPU::storeVRAM8(u16 address, u8 data)
{
    m_VRAM[address] = data;

    m_isTileDataDirty = address < 0x1800;
}

void PPU::clearVRAM()
{
    std::memset(m_VRAM, 0, VRAM_SIZE);

    m_isTileDataDirty = true;
}

void PPU::redrawTileData()
{
    glw::Renderer::beginFrame(m_tileDataFBO);
    u16 address = 0;
    for (u16 y = 0; y < 24; y++) {
        for (u16 x = 0; x < 16; x++) {
            for (u8 tileY = 0; tileY < 16; tileY += 2) {
                u8 b1 = m_VRAM[address + tileY];
                u8 b2 = m_VRAM[address + tileY + 1];
                for (s8 bit = 7; bit >= 0; bit--) {
                    u8 color = (((b2 >> bit) & 1) << 1) | ((b1 >> bit) & 1);
                    glw::Renderer::renderPoint(x * 8 + 7 - bit + (x + 1), y * 8 + tileY / 2 + (y + 1), s_colors[s_bgColorMap[color]]);
                }
            }

            address += 16;
        }
    }
    glw::Renderer::endFrame();
}

void PPU::redrawTileMap(u16 address)
{
    m_tileDataFBO->getAttachments()[0].bind(0);
    for (u16 y = 0; y < 32; y++) {
        for (u16 x = 0; x < 32; x++) {
            u16 index = m_VRAM[address];
            //if (m_LCDControl.WinBGTileData == 0 && index < 128) index += 256;
            u16 ypos = index / 16;
            u16 xpos = index % 16;
            float left = (float)(x * 8) / (256.f * 0.5f) - 1.f;
            float right = (float)(x * 8 + 8) / (256.f * 0.5f) - 1.f;
            float top = -((float)(y * 8) / (256.f * 0.5f) - 1.f);
            float bottom = -((float)(y * 8 + 8) / (256.f * 0.5f) - 1.f);
            float u0 = (float)(xpos * 8 + (xpos + 1)) / (TILE_DATA_FRAME_WIDTH + 15.f + 2.f);
            float v0 = 1.f - (float)(ypos * 8 + (ypos + 1)) / (TILE_DATA_FRAME_HEIGHT + 23.f + 2.f);
            float u1 = (float)(xpos * 8 + 8 + (xpos + 1)) / (TILE_DATA_FRAME_WIDTH + 15.f + 2.f);
            float v1 = 1.f - (float)(ypos * 8 + 8 + (ypos + 1)) / (TILE_DATA_FRAME_HEIGHT + 23.f + 2.f);
            glw::Renderer::renderTexture(left, top, right, bottom, u0, v0, u1, v1);
            address++;
        }
    }
}
