#pragma once
#include "bit_fifo.hpp"

#include <functional>
#include <span>

namespace glw {
	class Texture;
}

class PPU
{
public:
	static constexpr u16 LCD_WIDTH = 160;
	static constexpr u16 LCD_HEIGHT = 144;

	explicit PPU(u8& interruptFlagsRef);
	~PPU();

	using ReadMemoryCallback = std::function<u8(u16)>;
	void mapReadExternalMemoryCallback(ReadMemoryCallback callback) { loadExternal8 = callback; }

	void reset();
	void clock();

	void clearVRAM();
	u8 loadVRAM8(u16 address) const;
	void storeVRAM8(u16 address, u8 data);
	u8 loadOAM8(u16 address) const;
	void storeOAM8(u16 address, u8 data);
	u8 load8(u16 address) const;
	void store8(u16 address, u8 data);
	ReadMemoryCallback loadExternal8 = nullptr;

	std::span<const u32> getScreenPixels() const { return { m_screenPixels, LCD_WIDTH * LCD_HEIGHT }; }

	// debug:
	static constexpr u16 TILE_DATA_WIDTH = 16 * 8;
	static constexpr u16 TILE_DATA_HEIGHT = 24 * 8;
	std::span<u32> getTileDataPixels() const { return { m_tileDataPixels, TILE_DATA_WIDTH * TILE_DATA_HEIGHT }; }
	std::span<u8> getTileMap0() const { return { m_VRAM + 0x1800, 0x400 }; }
	std::span<u8> getTileMap1() const { return { m_VRAM + 0x1C00, 0x400 }; }
	bool getTileDataAddressingMode() const { return m_LCDControl.WinBGTileData; }

	PPU(const PPU&) = delete;
	PPU& operator=(const PPU&) = delete;
private:
	static constexpr u16 VRAM_SIZE = 0x2000;

	struct OAMEntry {
		u8 YPosition;
		u8 XPosition;
		u8 TileIndex;
		union {
			struct {
				u8 cgb_only      : 4; // 0-3
				u8 paletteNumber : 1; // 4
				u8 xFlip         : 1; // 5
				u8 yFlip         : 1; // 6
				u8 OBJPriority   : 1; // 7
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
			u8 WinBGEnable   : 1; // 0
			u8 OBJEnable     : 1; // 1
			u8 OBJSize       : 1; // 2
			u8 BGTileMap     : 1; // 3
			u8 WinBGTileData : 1; // 4
			u8 WinEnable     : 1; // 5
			u8 WinTileMap    : 1; // 6
			u8 LCDEnable     : 1; // 7
		};
		u8 byte;
	} m_LCDControl;

	enum class Mode {
		HBlank, VBlank, OAMSearch, PixelTransfer
	};

	union {
		struct {
			u8 Mode       : 2; // 0-1
			u8 LYCEqLY    : 1; // 2
			u8 STATSource : 4; // 3-6
			u8 Unsued     : 1; // 7
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
	BitFIFO m_colorFIFO;
	u16 m_pixelFIFOPaletteL;
	u16 m_pixelFIFOPaletteH;
	u8 m_currentPixelX;

	u32* m_screenPixels;
	u8& m_interruptFlagsRef;

	// DMA
	void handleDMA();

	bool m_DMARequested;
	bool m_DMAInProgress;
	u8 m_DMAAddress;

	// debug:
	void redrawTileData(u8 xOffset = 0, u8 yOffset = 0, u8 width = 16, u8 height = 24);
	u32* m_tileDataPixels;
};
