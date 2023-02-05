#pragma once
#include "../shared/type_aliases.hpp"

#include <ccl/non_copyable.h>

class PPU :
	public ccl::NonCopyable
{
public:
	PPU() = default;

	void reset();
	void clock();

	u8 load8(u16 address);
	void store8(u16 address, u8 data);

	u8 getTileDataArea() const { return m_LCDControl.WinBGTileData; }
private:
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
};
