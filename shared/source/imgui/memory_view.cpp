#include "shared/source/imgui/memory_view.hpp"

#include <imgui.h>

namespace imgui {

	void MemoryView::updateWindow()
	{
        if (!open) return;

        if (ImGui::Begin("Memory", &open))
        {
            
        }
        ImGui::End();
	}

} // namespace imgui
