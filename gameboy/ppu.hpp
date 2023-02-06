#pragma once
#include "../shared/type_aliases.hpp"

#include <ccl/non_copyable.h>

namespace glw {
	class Framebuffer;
}

class PPU :
	public ccl::NonCopyable
{
public:
	PPU();
	~PPU();

	void reset();
	void clock();

	u8 load8(u16 address) const;
	void store8(u16 address, u8 data);
	u8 loadVRAM8(u16 address) const;
	void storeVRAM8(u16 address, u8 data);

	void clearVRAM();

	const glw::Framebuffer* getTileDataFBO() const { return m_tileDataFBO; }
	const glw::Framebuffer* getTileMap0FBO() const { return m_tileMap0FBO; }
	const glw::Framebuffer* getTileMap1FBO() const { return m_tileMap1FBO; }
private:
	static constexpr u16 VRAM_SIZE = 0x2000;

	u8* m_VRAM;

	union {
		struct {
			u8 LCDEnable     : 1; // 7
			u8 WinTileMap    : 1; // 6
			u8 WinEnable     : 1; // 5
			u8 WinBGTileData : 1; // 4
			u8 BGTileMap     : 1; // 3
			u8 OBJSize       : 1; // 2
			u8 OBJEnable     : 1; // 1
			u8 WinBGEnable   : 1; // 0
		};
		u8 byte;
	} m_LCDControl;

	union {
		struct {
			u8 asdf;
		};
		u8 byte;
	} m_LCDStatus;

	u8 m_SCY, m_SCX;
	u8 m_LY, m_LYC;

	u8 m_BGpaletteData;
	u8 m_OBJpalette0Data;
	u8 m_OBJpalette1Data;

	u8 m_WY, m_WX;

	// debug:
	static constexpr u16 TILE_DATA_FRAME_WIDTH = 16 * 8;
	static constexpr u16 TILE_DATA_FRAME_HEIGHT = 24 * 8;
	static constexpr u16 TILEMAP_FRAME_SIZE = 256;

	void redrawTileData();
	void redrawTileMap(u16 address);

	glw::Framebuffer* m_tileDataFBO;
	bool m_isTileDataDirty;
	glw::Framebuffer* m_tileMap0FBO;
	glw::Framebuffer* m_tileMap1FBO;
};
