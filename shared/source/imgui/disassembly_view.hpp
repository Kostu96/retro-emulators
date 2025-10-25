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

        void updateWindow(unsigned int pc);

        DisassemblyView(const Disassembly& disasm, unsigned int addrWidth) :
            disassembly{ disasm },
            addressWidth{ addrWidth } {}
    };

} // namespace imgui
