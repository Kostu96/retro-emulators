#include "shared/source/imgui/disassembly_view.hpp"
#include "shared/source/disassembly_line.hpp"

#include <imgui.h>

namespace imgui {

    void DisassemblyView::updateWindow(uint32_t pc)
    {
        if (!open) return;

        if (ImGui::Begin("Disassembly", &open, ImGuiWindowFlags_NoScrollbar))
        {
            ImGui::BeginChild("##scrolling", ImVec2(0, 0), false, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoNav);

            ImGuiListClipper clipper;
            clipper.Begin((int)disassembly.size(), ImGui::GetTextLineHeight());

            while (clipper.Step())
                for (int line_i = clipper.DisplayStart; line_i < clipper.DisplayEnd; line_i++)
                {
                    auto& line = disassembly[line_i];
                    if (line_i > 0) {
                        auto& prev_line = disassembly[line_i - 1];
                        if (line.address - prev_line.address > 4) ImGui::Separator();
                    }
                    constexpr const char* fmt = "%08X:  %s";
                    (pc == line.address) ?
                    ImGui::TextColored({ 1.f, 0.f, 0.f, 1.f }, fmt, line.address, line.buffer) :
                    ImGui::Text(fmt, line.address, line.buffer);
                }

            ImGui::EndChild();
        }
        ImGui::End();
    }

} // namespace imgui
