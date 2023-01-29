#include "gui.hpp"
#include "../shared/emulator_core.hpp"

#include <GLFW/glfw3.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

namespace GUI {

	void init(GLFWwindow* window)
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
        ImGui::StyleColorsDark();
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 450");
    }

    void shutdown()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    void beginFrame()
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void renderFrame()
    {
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }
    }

    void MemoryView::drawWindow(const char* title, const std::shared_ptr<EmulatorCore>& core, size_t memoryIndex, size_t base_display_addr)
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

    void DisassemblyView::drawWindow(const std::shared_ptr<EmulatorCore>& core)
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
    
    void StateView::drawWindow(const std::shared_ptr<EmulatorCore>& core, bool& paused, double dt)
    {
        if (ImGui::Begin("State", &open, ImGuiWindowFlags_NoScrollbar))
        {
            auto& state = core->getState();
            for (auto& entry : state)
            {
                ImGui::Text(entry.label);
                ImGui::SameLine();
                ImGui::Text("%0*X", entry.width, entry.value);
                if (entry.sameLine) ImGui::SameLine();
                if (entry.separator) ImGui::Separator();
            }

            ImGui::Separator();

            ImGui::BeginDisabled(!paused);
            if (ImGui::Button("Run")) paused = false;
            ImGui::EndDisabled();
            ImGui::SameLine();

            ImGui::BeginDisabled(paused);
            if (ImGui::Button("Pause")) paused = true;
            ImGui::EndDisabled();
            ImGui::SameLine();

            ImGui::BeginDisabled(!paused);
            if (ImGui::Button("Step")) core->update(dt);
            ImGui::EndDisabled();
            ImGui::SameLine();

            if (ImGui::Button("Reset")) core->reset();
        }
        ImGui::End();
    }
}
