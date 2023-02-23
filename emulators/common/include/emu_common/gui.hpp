#pragma once
#include <vector>

struct DisassemblyLine;
struct GLFWwindow;

namespace EmuCommon::GUI {

    void init(GLFWwindow* window);
    void shutdown();
    void beginFrame();
    void endFrame();

    struct DisassemblyView
    {
        bool open = true;

        void updateWindow(const std::vector<DisassemblyLine>& disassembly);
    };

} // namespace EmuCommon::GUI
