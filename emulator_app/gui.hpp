#pragma once
#include "../shared/type_aliases.hpp"

#include <imgui.h>

#include <cstdio>
#include <cstdint>
#include <memory>
#include <vector>

class EmulatorCore;
struct GLFWwindow;

namespace GUI {

    void init(GLFWwindow* window);
    void shutdown();
    void beginFrame();
    void renderFrame();

    struct MemoryView
    {
        bool open;                  // = true // set to false when DrawWindow() was closed. ignore if not using DrawWindow().
        int optMidColsCount;        // = 8    // set to 0 to disable extra spacing between every mid-cols.
        float optFooterExtraHeight; // = 0    // space to reserve at the bottom of the widget to add custom widgets

        char   addrInputBuf[32];
        size_t gotoAddr;

        MemoryView()
        {
            open = true;
            optMidColsCount = 8;
            optFooterExtraHeight = 0.0f;

            memset(addrInputBuf, 0, sizeof(addrInputBuf));
            gotoAddr = (size_t)-1;
        }

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

        void calcSizes(Sizes& s, size_t mem_size, int cols, size_t base_display_addr)
        {
            ImGuiStyle& style = ImGui::GetStyle();
            for (size_t n = base_display_addr + mem_size - 1; n > 0; n >>= 4)
                s.AddrDigitsCount++;
            s.LineHeight = ImGui::GetTextLineHeight();
            s.GlyphWidth = ImGui::CalcTextSize("F").x + 1;                  // We assume the font is mono-space
            s.HexCellWidth = (float)(int)(s.GlyphWidth * 2.5f);             // "FF " we include trailing space in the width to easily catch clicks everywhere
            s.SpacingBetweenMidCols = (float)(int)(s.HexCellWidth * 0.25f); // Every OptMidColsCount columns we add a bit of extra spacing
            s.PosHexStart = (s.AddrDigitsCount + 2) * s.GlyphWidth;
            s.PosHexEnd = s.PosHexStart + (s.HexCellWidth * cols);
            s.PosAsciiStart = s.PosAsciiEnd = s.PosHexEnd;
            s.PosAsciiStart = s.PosHexEnd + s.GlyphWidth * 1;
            if (optMidColsCount > 0)
                s.PosAsciiStart += (float)((cols + optMidColsCount - 1) / optMidColsCount) * s.SpacingBetweenMidCols;
            s.PosAsciiEnd = s.PosAsciiStart + cols * s.GlyphWidth;
            s.WindowWidth = s.PosAsciiEnd + style.ScrollbarSize + style.WindowPadding.x * 2 + s.GlyphWidth;
        }

        void drawWindow(const char* title, const std::shared_ptr<EmulatorCore>& core, size_t memoryIndex, size_t base_display_addr = 0x0000);

        void drawOptionsLine(const Sizes& s, size_t mem_size, int cols, size_t base_display_addr)
        {
            ImGuiStyle& style = ImGui::GetStyle();
            ImGui::Text("Go to address (%0*IX-%0*IX):", s.AddrDigitsCount, base_display_addr, s.AddrDigitsCount, base_display_addr + mem_size - 1);
            ImGui::SameLine();
            ImGui::SetNextItemWidth((s.AddrDigitsCount + 1) * s.GlyphWidth + style.FramePadding.x * 2.0f);
            if (ImGui::InputText("##addr", addrInputBuf, IM_ARRAYSIZE(addrInputBuf), ImGuiInputTextFlags_CharsHexadecimal | ImGuiInputTextFlags_EnterReturnsTrue))
            {
                size_t goto_addr;
                if (sscanf_s(addrInputBuf, "%IX", &goto_addr) == 1)
                {
                    gotoAddr = goto_addr - base_display_addr;
                }
            }

            if (gotoAddr != (size_t)-1)
            {
                if (gotoAddr < mem_size)
                {
                    ImGui::BeginChild("##scrolling");
                    ImGui::SetScrollFromPosY(ImGui::GetCursorStartPos().y + (gotoAddr / cols) * ImGui::GetTextLineHeight());
                    ImGui::EndChild();
                }
                gotoAddr = (size_t)-1;
            }
        }
    };

    struct DisassemblyView
    {
        bool open = true;

        size_t findLine(u16 addr, size_t max_addr, const std::vector<DisassemblyLine>& disassembly)
        {
            float p = (float)addr / (float)max_addr;
            size_t line = (size_t)(disassembly.size() * p);
            while (disassembly[line].address > addr) line--;
            return line;
        }

        void drawWindow(const std::shared_ptr<EmulatorCore>& core);
    };

    struct StateView
    {
        bool open = true;

        void drawWindow(const std::shared_ptr<EmulatorCore>& core, bool& paused, double dt);
    };

}
