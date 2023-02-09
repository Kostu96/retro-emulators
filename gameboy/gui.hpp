#pragma once

struct GLFWwindow;

class Gameboy;

namespace GUI {

    void init(GLFWwindow* window);
    void shutdown();
    void update(const Gameboy& gb);

} // namespace GUI
