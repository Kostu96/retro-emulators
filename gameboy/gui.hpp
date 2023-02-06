#pragma once

struct GLFWwindow;

class PPU;

namespace GUI {

    void init(GLFWwindow* window);
    void shutdown();
    void update(const PPU& ppu);

} // namespace GUI
