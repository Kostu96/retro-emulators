#pragma once
#include <functional>

namespace imgui {

	struct MemoryView
	{
		bool open = false;
		unsigned int cols = 32; // number of columns to display.
		
		using Read8MemoryCallback = std::function<unsigned char(unsigned int)>;
		Read8MemoryCallback read8 = nullptr;

		int OptMidColsCount = 16; // set to 0 to disable extra spacing between every mid-cols.

        struct Sizes
        {
            int     AddrDigitsCount;
            float   LineHeight;
            float   GlyphWidth;
            float   HexCellWidth;
            float   SpacingBetweenMidCols;
            float   PosHexStart;
            float   PosHexEnd;
            float   PosAsciiStart;
            float   PosAsciiEnd;
            float   WindowWidth;

            Sizes() { memset(this, 0, sizeof(*this)); }
        };

		void calcSizes(Sizes& s, size_t mem_size, size_t base_display_addr);
		void updateWindow();
	};

} // namespace imgui
