#pragma once

struct GLFWwindow;

class Gameboy;

namespace GUI {

    void init(GLFWwindow* window);
    void shutdown();
    void update(Gameboy& gb);

} // namespace GUI
