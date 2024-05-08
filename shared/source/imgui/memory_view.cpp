#include "shared/source/imgui/memory_view.hpp"

#include <imgui.h>

namespace imgui {

    void MemoryView::calcSizes(Sizes& s, size_t mem_size, size_t base_display_addr)
    {
        ImGuiStyle& style = ImGui::GetStyle();
        s.AddrDigitsCount = 0;
        for (size_t n = base_display_addr + mem_size - 1; n > 0; n >>= 4)
            s.AddrDigitsCount++;
        s.LineHeight = ImGui::GetTextLineHeight();
        s.GlyphWidth = ImGui::CalcTextSize("F").x + 1;                  // We assume the font is mono-space
        s.HexCellWidth = (float)(int)(s.GlyphWidth * 2.5f);             // "FF " we include trailing space in the width to easily catch clicks everywhere
        s.SpacingBetweenMidCols = (float)(int)(s.HexCellWidth * 0.25f); // Every OptMidColsCount columns we add a bit of extra spacing
        s.PosHexStart = (s.AddrDigitsCount + 2) * s.GlyphWidth;
        s.PosHexEnd = s.PosHexStart + (s.HexCellWidth * cols);
        s.PosAsciiStart = s.PosAsciiEnd = s.PosHexEnd;
        /*if (OptShowAscii)
        {
            s.PosAsciiStart = s.PosHexEnd + s.GlyphWidth * 1;
            if (OptMidColsCount > 0)
                s.PosAsciiStart += (float)((Cols + OptMidColsCount - 1) / OptMidColsCount) * s.SpacingBetweenMidCols;
            s.PosAsciiEnd = s.PosAsciiStart + Cols * s.GlyphWidth;
        }*/
        s.WindowWidth = s.PosAsciiEnd + style.ScrollbarSize + style.WindowPadding.x * 2 + s.GlyphWidth;
    }

	void MemoryView::updateWindow()
	{
        static const size_t mem_size = 0x1000000; // temp
        static const size_t base_display_addr = 0; // temp?

        if (!open) return;

        if (ImGui::Begin("Memory", &open))
        {
            Sizes s;
            calcSizes(s, mem_size, base_display_addr);

            ImGui::BeginChild("##scrolling", ImVec2(0, 0), false, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoNav);

            const int line_total_count = (int)((mem_size + cols - 1) / cols);
            ImGuiListClipper clipper;
            clipper.Begin(line_total_count, ImGui::GetTextLineHeight());

            const char* format_address = "%0*IX: ";
            const char* format_byte_space = "%02X ";

            while (clipper.Step())
                for (int line_i = clipper.DisplayStart; line_i < clipper.DisplayEnd; line_i++) // display only visible lines
                {
                    size_t addr = (size_t)(line_i * cols);
                    ImGui::Text(format_address, s.AddrDigitsCount, base_display_addr + addr);

                    // Draw Hexadecimal
                    for (unsigned int n = 0; n < cols && addr < mem_size; n++, addr++)
                    {
                        float byte_pos_x = s.PosHexStart + s.HexCellWidth * n;
                        if (OptMidColsCount > 0)
                            byte_pos_x += (float)(n / OptMidColsCount) * s.SpacingBetweenMidCols;
                        ImGui::SameLine(byte_pos_x);

                        //// Draw highlight
                        //bool is_highlight_from_user_range = (addr >= HighlightMin && addr < HighlightMax);
                        //bool is_highlight_from_user_func = (HighlightFn && HighlightFn(mem_data, addr));
                        //bool is_highlight_from_preview = (addr >= DataPreviewAddr && addr < DataPreviewAddr + preview_data_type_size);
                        //if (is_highlight_from_user_range || is_highlight_from_user_func || is_highlight_from_preview)
                        //{
                        //    ImVec2 pos = ImGui::GetCursorScreenPos();
                        //    float highlight_width = s.GlyphWidth * 2;
                        //    bool is_next_byte_highlighted = (addr + 1 < mem_size) && ((HighlightMax != (size_t)-1 && addr + 1 < HighlightMax) || (HighlightFn && HighlightFn(mem_data, addr + 1)));
                        //    if (is_next_byte_highlighted || (n + 1 == Cols))
                        //    {
                        //        highlight_width = s.HexCellWidth;
                        //        if (OptMidColsCount > 0 && n > 0 && (n + 1) < Cols && ((n + 1) % OptMidColsCount) == 0)
                        //            highlight_width += s.SpacingBetweenMidCols;
                        //    }
                        //    draw_list->AddRectFilled(pos, ImVec2(pos.x + highlight_width, pos.y + s.LineHeight), HighlightColor);
                        //}

                        //if (DataEditingAddr == addr)
                        //{
                        //    // Display text input on current byte
                        //    bool data_write = false;
                        //    ImGui::PushID((void*)addr);
                        //    if (DataEditingTakeFocus)
                        //    {
                        //        ImGui::SetKeyboardFocusHere(0);
                        //        sprintf(AddrInputBuf, format_data, s.AddrDigitsCount, base_display_addr + addr);
                        //        sprintf(DataInputBuf, format_byte, ReadFn ? ReadFn(mem_data, addr) : mem_data[addr]);
                        //    }
                        //    struct UserData
                        //    {
                        //        // FIXME: We should have a way to retrieve the text edit cursor position more easily in the API, this is rather tedious. This is such a ugly mess we may be better off not using InputText() at all here.
                        //        static int Callback(ImGuiInputTextCallbackData* data)
                        //        {
                        //            UserData* user_data = (UserData*)data->UserData;
                        //            if (!data->HasSelection())
                        //                user_data->CursorPos = data->CursorPos;
                        //            if (data->SelectionStart == 0 && data->SelectionEnd == data->BufTextLen)
                        //            {
                        //                // When not editing a byte, always refresh its InputText content pulled from underlying memory data
                        //                // (this is a bit tricky, since InputText technically "owns" the master copy of the buffer we edit it in there)
                        //                data->DeleteChars(0, data->BufTextLen);
                        //                data->InsertChars(0, user_data->CurrentBufOverwrite);
                        //                data->SelectionStart = 0;
                        //                data->SelectionEnd = 2;
                        //                data->CursorPos = 0;
                        //            }
                        //            return 0;
                        //        }
                        //        char   CurrentBufOverwrite[3];  // Input
                        //        int    CursorPos;               // Output
                        //    };
                        //    UserData user_data;
                        //    user_data.CursorPos = -1;
                        //    sprintf(user_data.CurrentBufOverwrite, format_byte, ReadFn ? ReadFn(mem_data, addr) : mem_data[addr]);
                        //    ImGuiInputTextFlags flags = ImGuiInputTextFlags_CharsHexadecimal | ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_NoHorizontalScroll | ImGuiInputTextFlags_CallbackAlways;
                        //    flags |= ImGuiInputTextFlags_AlwaysOverwrite; // was ImGuiInputTextFlags_AlwaysInsertMode
                        //    ImGui::SetNextItemWidth(s.GlyphWidth * 2);
                        //    if (ImGui::InputText("##data", DataInputBuf, IM_ARRAYSIZE(DataInputBuf), flags, UserData::Callback, &user_data))
                        //        data_write = data_next = true;
                        //    else if (!DataEditingTakeFocus && !ImGui::IsItemActive())
                        //        DataEditingAddr = data_editing_addr_next = (size_t)-1;
                        //    DataEditingTakeFocus = false;
                        //    if (user_data.CursorPos >= 2)
                        //        data_write = data_next = true;
                        //    if (data_editing_addr_next != (size_t)-1)
                        //        data_write = data_next = false;
                        //    unsigned int data_input_value = 0;
                        //    if (data_write && sscanf(DataInputBuf, "%X", &data_input_value) == 1)
                        //    {
                        //        if (WriteFn)
                        //            WriteFn(mem_data, addr, (ImU8)data_input_value);
                        //        else
                        //            mem_data[addr] = (ImU8)data_input_value;
                        //    }
                        //    ImGui::PopID();
                        //}
                        //else
                        //{
                            // NB: The trailing space is not visible but ensure there's no gap that the mouse cannot click on.
                        ImU8 b = read8(static_cast<unsigned int>(addr));// ReadFn ? ReadFn(mem_data, addr) : mem_data[addr];

                            /*if (OptShowHexII)
                            {
                                if ((b >= 32 && b < 128))
                                    ImGui::Text(".%c ", b);
                                else if (b == 0xFF && OptGreyOutZeroes)
                                    ImGui::TextDisabled("## ");
                                else if (b == 0x00)
                                    ImGui::Text("   ");
                                else
                                    ImGui::Text(format_byte_space, b);
                            }
                            else
                            {*/
                                if (b == 0 /*&& OptGreyOutZeroes*/)
                                    ImGui::TextDisabled("00 ");
                                else
                                    ImGui::Text(format_byte_space, b);
                            //}
                            /*if (!ReadOnly && ImGui::IsItemHovered() && ImGui::IsMouseClicked(0))
                            {
                                DataEditingTakeFocus = true;
                                data_editing_addr_next = addr;
                            }*/
                        //}
                    }

                    //if (OptShowAscii)
                    //{
                    //    // Draw ASCII values
                    //    ImGui::SameLine(s.PosAsciiStart);
                    //    ImVec2 pos = ImGui::GetCursorScreenPos();
                    //    addr = line_i * Cols;
                    //    ImGui::PushID(line_i);
                    //    if (ImGui::InvisibleButton("ascii", ImVec2(s.PosAsciiEnd - s.PosAsciiStart, s.LineHeight)))
                    //    {
                    //        DataEditingAddr = DataPreviewAddr = addr + (size_t)((ImGui::GetIO().MousePos.x - pos.x) / s.GlyphWidth);
                    //        DataEditingTakeFocus = true;
                    //    }
                    //    ImGui::PopID();
                    //    for (int n = 0; n < Cols && addr < mem_size; n++, addr++)
                    //    {
                    //        if (addr == DataEditingAddr)
                    //        {
                    //            draw_list->AddRectFilled(pos, ImVec2(pos.x + s.GlyphWidth, pos.y + s.LineHeight), ImGui::GetColorU32(ImGuiCol_FrameBg));
                    //            draw_list->AddRectFilled(pos, ImVec2(pos.x + s.GlyphWidth, pos.y + s.LineHeight), ImGui::GetColorU32(ImGuiCol_TextSelectedBg));
                    //        }
                    //        unsigned char c = ReadFn ? ReadFn(mem_data, addr) : mem_data[addr];
                    //        char display_c = (c < 32 || c >= 128) ? '.' : c;
                    //        draw_list->AddText(pos, (display_c == c) ? color_text : color_disabled, &display_c, &display_c + 1);
                    //        pos.x += s.GlyphWidth;
                    //    }
                    //}
                }

            ImGui::EndChild();
        }
        ImGui::End();
	}

} // namespace imgui
