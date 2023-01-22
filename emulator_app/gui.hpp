#pragma once
#include <cstdio>
#include <cstdint>

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

    void drawWindow(const char* title, const std::shared_ptr<EmulatorCore>& core, size_t memoryIndex, size_t base_display_addr = 0x0000)
    {
        int cols = core->getMemoryColsNumbers()[memoryIndex];
        Sizes s;
        calcSizes(s, core->getMemorySizes()[memoryIndex], cols, base_display_addr);
        ImGui::SetNextWindowSize(ImVec2(s.WindowWidth, s.WindowWidth * 0.60f), ImGuiCond_Once);
        ImGui::SetNextWindowSizeConstraints(ImVec2(0.0f, 0.0f), ImVec2(s.WindowWidth, FLT_MAX));

        if (ImGui::Begin(title, &open, ImGuiWindowFlags_NoScrollbar))
        {
            if (cols < 1)
                cols = 1;

            size_t mem_size = core->getMemorySizes()[memoryIndex];
            ImGuiStyle& style = ImGui::GetStyle();

            // We begin into our scrolling region with the 'ImGuiWindowFlags_NoMove' in order to prevent click from moving the window.
            // This is used as a facility since our main click detection code doesn't assign an ActiveId so the click would normally be caught as a window-move.
            const float height_separator = style.ItemSpacing.y;
            float footer_height = optFooterExtraHeight + height_separator + ImGui::GetFrameHeightWithSpacing() * 1;
            ImGui::BeginChild("##scrolling", ImVec2(0, -footer_height), false, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoNav);
            ImDrawList* draw_list = ImGui::GetWindowDrawList();

            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));

            // We are not really using the clipper API correctly here, because we rely on visible_start_addr/visible_end_addr for our scrolling function.
            const int line_total_count = (int)((mem_size + cols - 1) / cols);
            ImGuiListClipper clipper;
            clipper.Begin(line_total_count, s.LineHeight);

            // Draw vertical separator
            ImVec2 window_pos = ImGui::GetWindowPos();
            draw_list->AddLine(ImVec2(window_pos.x + s.PosAsciiStart - s.GlyphWidth, window_pos.y), ImVec2(window_pos.x + s.PosAsciiStart - s.GlyphWidth, window_pos.y + 9999), ImGui::GetColorU32(ImGuiCol_Border));

            const ImU32 color_text = ImGui::GetColorU32(ImGuiCol_Text);
            const ImU32 color_disabled = ImGui::GetColorU32(ImGuiCol_TextDisabled);

            while (clipper.Step())
                for (int line_i = clipper.DisplayStart; line_i < clipper.DisplayEnd; line_i++) // display only visible lines
                {
                    size_t addr = line_i * cols;
                    ImGui::Text("%0*IX: ", s.AddrDigitsCount, base_display_addr + addr);

                    // Draw Hexadecimal
                    for (int n = 0; n < cols && addr < mem_size; n++, addr++)
                    {
                        float byte_pos_x = s.PosHexStart + s.HexCellWidth * n;
                        if (optMidColsCount > 0)
                            byte_pos_x += (float)(n / optMidColsCount) * s.SpacingBetweenMidCols;
                        ImGui::SameLine(byte_pos_x);

                        ImU8 b = core->getByteAt((u16)addr, memoryIndex);
                        if (b == 0)
                            ImGui::TextDisabled("00 ");
                        else
                            ImGui::Text("%02X ", b);
                    }

                    // Draw ASCII values
                    ImGui::SameLine(s.PosAsciiStart);
                    ImVec2 pos = ImGui::GetCursorScreenPos();
                    addr = line_i * cols;
                    ImGui::PushID(line_i);
                    ImGui::InvisibleButton("ascii", ImVec2(s.PosAsciiEnd - s.PosAsciiStart, s.LineHeight));
                    ImGui::PopID();
                    for (int n = 0; n < cols && addr < mem_size; n++, addr++)
                    {
                        unsigned char c = core->getByteAt((u16)addr, memoryIndex);
                        char display_c = (c < 32 || c >= 128) ? '.' : c;
                        draw_list->AddText(pos, (display_c == c) ? color_text : color_disabled, &display_c, &display_c + 1);
                        pos.x += s.GlyphWidth;
                    }
                }
            ImGui::PopStyleVar(2);
            ImGui::EndChild();

            // Notify the main window of our ideal child content size (FIXME: we are missing an API to get the contents size from the child)
            ImGui::SetCursorPosX(s.WindowWidth);

            ImGui::Separator();
            drawOptionsLine(s, mem_size, cols, base_display_addr);
        }
        ImGui::End();
    }

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
    bool open;

    DisassemblyView() :
        open{ true }
    {}

    size_t findLine(u16 addr, size_t max_addr, const std::vector<DisassemblyLine>& disassembly)
    {
        float p = (float)addr / (float)max_addr;
        size_t line = (size_t)(disassembly.size() * p);
        while (disassembly[line].address > addr) line--;
        return line;
    }

    void drawWindow(const std::shared_ptr<EmulatorCore>& core)
    {
        auto& disassembly = core->getDisassembly();
        if (!disassembly.empty())
        {
            if (ImGui::Begin("Disassembly", &open, ImGuiWindowFlags_NoScrollbar))
            {
                ImGui::BeginChild("##scrolling", ImVec2(0, 0), false, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoNav);

                ImGuiListClipper clipper;
                clipper.Begin((int)disassembly.size(), ImGui::GetTextLineHeight());

                while (clipper.Step())
                    for (int line_i = clipper.DisplayStart; line_i < clipper.DisplayEnd; line_i++)
                    {
                        auto& line = disassembly[line_i];
                        if (line.address == core->getPC())
                            ImGui::TextColored(ImVec4{ 1.f, 0.1f, 0.1f, 1.f }, line.buffer);
                        else
                            ImGui::Text(line.buffer);
                    }

                ImGui::SetScrollFromPosY(
                    ImGui::GetCursorStartPos().y + findLine(core->getPC(), core->getMemorySizes()[0], disassembly) * ImGui::GetTextLineHeight()
                );
                ImGui::EndChild();
            }
            ImGui::End();
        }
    }
};
