#include "shared/source/imgui/debug_view.hpp"

#include <imgui.h>

namespace imgui {

	void DebugView::updateWindow()
	{
        if (!open) return;

        if (ImGui::Begin("Debug Control&Status", &open, ImGuiWindowFlags_NoScrollbar))
        {
            ImGui::Button("Start"); ImGui::SameLine();
            ImGui::Button("Pause"); ImGui::SameLine();
            if (ImGui::Button("Step") && cpuStepCallback) cpuStepCallback();
            ImGui::SeparatorText("Status");
            if (cpuStatusCallback) cpuStatusCallback();
        }
        ImGui::End();
	}

} // namespace imgui
