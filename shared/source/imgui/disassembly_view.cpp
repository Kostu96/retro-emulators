#include "shared/source/imgui/disassembly_view.hpp"
#include "shared/source/disassembly_line.hpp"

#include <imgui.h>

namespace imgui {

    void DisassemblyView::updateWindow(const std::vector<DisassemblyLine>& disassembly)
    {
        if (disassembly.empty()) return;

        if (ImGui::Begin("Disassembly", &open, ImGuiWindowFlags_NoScrollbar))
        {
            ImGui::BeginChild("##scrolling", ImVec2(0, 0), false, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoNav);

            ImGuiListClipper clipper;
            clipper.Begin((int)disassembly.size(), ImGui::GetTextLineHeight());

            while (clipper.Step())
                for (int line_i = clipper.DisplayStart; line_i < clipper.DisplayEnd; line_i++)
                {
                    auto& line = disassembly[line_i];
                    ImGui::Text(line.buffer);
                }

            ImGui::EndChild();
        }
        ImGui::End();
    }

} // namespace imgui
