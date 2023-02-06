#include "ppu.hpp"

#include <glad/gl.h>

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
        glw::Framebuffer::Properties{ TILE_DATA_FRAME_WIDTH, TILE_DATA_FRAME_HEIGHT, 1, {
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
    m_LY = 0x90; // TODO: temp

    m_isTileDataDirty = true;
    m_isTileMap0Dirty = true;
    m_isTileMap1Dirty = true;
}

void PPU::clock()
{
    if (m_isTileDataDirty) { m_isTileDataDirty = false; redrawTileData(); }
    if (m_isTileMap0Dirty) { m_isTileMap0Dirty = false; redrawTileMap(m_tileMap0FBO, 0x1800); }
    if (m_isTileMap1Dirty) { m_isTileMap1Dirty = false; redrawTileMap(m_tileMap1FBO, 0x1C00); }
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
    m_isTileMap0Dirty = address >= 0x1800 && address < 0x1C00;
    m_isTileMap1Dirty = address >= 0x1C00 && address < 0x2000;
}

void PPU::clearVRAM()
{
    std::memset(m_VRAM, 0, VRAM_SIZE);

    m_isTileDataDirty = true;
    m_isTileMap0Dirty = true;
    m_isTileMap1Dirty = true;
}

void PPU::redrawTileData()
{
    m_tileDataFBO->bind();
    glViewport(0, 0, TILE_DATA_FRAME_WIDTH, TILE_DATA_FRAME_HEIGHT);
    glw::Renderer::beginFrame();
    u16 address = 0;
    for (u16 y = 0; y < 24; y++) {
        for (u16 x = 0; x < 16; x++) {
            for (u8 tileY = 0; tileY < 16; tileY += 2) {
                u8 b1 = m_VRAM[address + tileY];
                u8 b2 = m_VRAM[address + tileY + 1];
                for (s8 bit = 7; bit >= 0; bit--) {
                    u8 color = (((b2 >> bit) & 1) << 1) | ((b1 >> bit) & 1);
                    glw::Renderer::renderPoint(x * 8 + 7 - bit, y * 8 + tileY / 2, s_colors[s_bgColorMap[color]]);
                }
            }

            address += 16;
        }
    }
    glw::Renderer::endFrame();
    m_tileDataFBO->unbind();
}

void PPU::redrawTileMap(glw::Framebuffer* tileMapFBO, u16 address)
{
    tileMapFBO->bind();
    glViewport(0, 0, TILEMAP_FRAME_SIZE, TILEMAP_FRAME_SIZE);
    glw::Renderer::beginFrame();
    m_tileDataFBO->getAttachments()[0].bind(0);
    for (u16 y = 0; y < 32; y++) {
        for (u16 x = 0; x < 32; x++) {
            u16 index = m_VRAM[address];
            if (m_LCDControl.WinBGTileData == 0 && index < 128) index += 256;
            u16 ypos = index / 16;
            u16 xpos = index % 16;
            float left = (float)(x * 8) / (256.f * 0.5f) - 1.f;
            float right = (float)(x * 8 + 8) / (256.f * 0.5f) - 1.f;
            float top = -((float)(y * 8) / (256.f * 0.5f) - 1.f);
            float bottom = -((float)(y * 8 + 8) / (256.f * 0.5f) - 1.f);
            float u0 = (float)(xpos * 8) / 128.f;
            float v0 = 1.f - (float)(ypos * 8) / 192.f;
            float u1 = (float)(xpos * 8 + 8) / 128.f;
            float v1 = 1.f - (float)(ypos * 8 + 8) / 192.f;
            glw::Renderer::renderTexture(left, top, right, bottom, u0, v0, u1, v1);
            address++;
        }
    }
    glw::Renderer::endFrame();
    tileMapFBO->unbind();
}
