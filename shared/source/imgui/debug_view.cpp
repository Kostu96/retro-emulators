#include "shared/source/imgui/debug_view.hpp"

#include <imgui.h>

namespace imgui {

	void DebugView::updateWindow()
	{
        if (!open) return;

        if (ImGui::Begin("Debug Control&Status", &open, ImGuiWindowFlags_NoScrollbar))
        {
            if (isPaused) {
                if (ImGui::Button("Start")) isPaused = false;
            }
            else {
                if (ImGui::Button("Pause")) isPaused = true;
            }
            ImGui::SameLine();
            if (ImGui::Button("Step") && stepCallback) stepCallback();
            ImGui::SeparatorText("Status");
            if (cpuStatusCallback) cpuStatusCallback();
        }
        ImGui::End();
	}

} // namespace imgui
