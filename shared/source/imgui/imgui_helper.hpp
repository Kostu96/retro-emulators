#pragma once

struct GLFWwindow;

namespace imgui {

    void init(GLFWwindow* window);
    void shutdown();
    void beginFrame();
    void endFrame();

} // namespace imgui
