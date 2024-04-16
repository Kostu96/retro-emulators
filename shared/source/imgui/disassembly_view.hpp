#pragma once
#include <vector>

struct DisassemblyLine;

namespace imgui {

    struct DisassemblyView
    {
        bool open = true;

        void updateWindow(const std::vector<DisassemblyLine>& disassembly);
    };

} // namespace imgui
