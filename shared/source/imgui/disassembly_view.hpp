#pragma once
#include <vector>

struct DisassemblyLine;

using Disassembly = std::vector<DisassemblyLine>;

namespace imgui {

    struct DisassemblyView
    {
        bool open = false;
        const Disassembly& disassembly;
        unsigned int addressWidth;

        void updateWindow(uint32_t pc);

        DisassemblyView(const Disassembly& disassembly, unsigned int addressWidth) :
            disassembly{ disassembly },
            addressWidth{ addressWidth } {}
    };

} // namespace imgui
