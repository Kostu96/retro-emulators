#pragma once
#include "../shared/type_aliases.hpp"

#include <ccl/non_copyable.h>

namespace glw {
	class Texture;
}

class PPU :
	public ccl::NonCopyable
{
public:
	PPU();
	~PPU();

	void reset();
	void clock();

	void clearVRAM();
	u8 loadVRAM8(u16 address) const;
	void storeVRAM8(u16 address, u8 data);
	u8 loadOAM8(u16 address) const;
	void storeOAM8(u16 address, u8 data);
	u8 load8(u16 address) const;
	void store8(u16 address, u8 data);

	const glw::Texture* getScreenTexture() const { return m_screenTexture; }

	// debug:
	const glw::Texture* getTileDataTexture() const { return m_tileDataTexture; }
private:
	static constexpr u16 VRAM_SIZE = 0x2000;
	static constexpr u16 LCD_WIDTH = 160;
	static constexpr u16 LCD_HEIGHT = 144;

	struct OAMEntry {
		u8 YPosition;
		u8 XPosition;
		u8 TileIndex;
		union {
			struct {
				u8 cgb_only : 4; // 0-3
				u8 paletteNumber : 1; // 4
				u8 xFlip : 1; // 5
				u8 yFlip : 1; // 6
				u8 OBJPriority : 1; // 7
			};
			u8 byte;
		} Flags;
	};
	static_assert(sizeof(OAMEntry) == 4);

	u8* m_VRAM;
	union {
		OAMEntry entries[40];
		u8 bytes[160];
	} m_OAM;
	static_assert(sizeof(m_OAM) == 160);

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
			u8 unsued : 1;     // 7
			u8 STATSource : 3; // 3-6
			u8 LYCEqLY : 1;    // 2
			u8 Mode : 2;       // 0-1
		};
		u8 byte;
	} m_LCDStatus;

	u8 m_SCY, m_SCX;
	u8 m_LY, m_LYC;

	u8 m_BGpaletteData;
	u8 m_OBJpalette0Data;
	u8 m_OBJpalette1Data;

	u8 m_WY, m_WX;

	u8 m_fetcherMode;
	u8 m_fetcherTileX;
	u8 m_fetcherTileY;
	u16 m_tileDataAddress;
	bool m_pixelFIFOEmpty;
	bool m_pixelFIFONeedFetch;
	u16 m_pixelFIFOColorIndexL;
	u16 m_pixelFIFOColorIndexH;
	u16 m_pixelFIFOPaletteL;
	u16 m_pixelFIFOPaletteH;
	u8 m_currentPixelX;

	u32* m_screenPixels;
	glw::Texture* m_screenTexture;

	// debug:
	void redrawTileDataTexture(u8 xOffset = 0, u8 yOffset = 0, u8 width = 16, u8 height = 24);

	u32* m_tileDataPixels;
	glw::Texture* m_tileDataTexture;
};
