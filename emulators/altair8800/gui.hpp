#pragma once

struct GLFWwindow;

class Altair;

namespace GUI {

    void init(GLFWwindow* window);
    void shutdown();
    void update(Altair& altair);

} // namespace GUI
