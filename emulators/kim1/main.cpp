#include "kim1.hpp"
#include "emu_common/application.hpp"

#include <glad/gl.h>
#include <glw/glw.hpp>
#include <GLFW/glfw3.h>

#include <iostream>
#include <thread>
#include <memory>

constexpr u16 BORDER_SIZE = 16;
constexpr u16 IMGUI_MENU_BAR_HEIGHT = 18;
constexpr u16 WINDOW_WIDTH = 6 * 32 + 3 * BORDER_SIZE;
constexpr u16 WINDOW_HEIGHT = 8 * 32 + 2 * BORDER_SIZE;

int main()
{
    std::unique_ptr<KIM1> kim1 = std::make_unique<KIM1>();

    glfwSetErrorCallback(EmuCommon::glfwErrorCallback);
    if (!glfwInit()) {
        std::cerr << "GLFW init failed!\n";
        std::terminate();
    }

    glfwWindowHint(GLFW_RESIZABLE, 0);
    GLFWwindow* window =
        glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT + IMGUI_MENU_BAR_HEIGHT, "KIM-1 Emulator by Kostu96", nullptr, nullptr);
    if (!window) {
        std::cerr << "GLFW window creation failed!\n";
        std::terminate();
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    glw::init(glfwGetProcAddress);
    glw::Renderer::init();
    glClearColor(0.85f, 0.85f, 0.85f, 1.f);

    std::thread emuThread{
        [&]() {
            while (!glfwWindowShouldClose(window)) {
                std::this_thread::sleep_for(std::chrono::nanoseconds{ 32 }); // TODO: temp
                kim1->clock();
            }
        }
    };

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    emuThread.join();

    glw::Renderer::shutdown();
    glfwTerminate();

    return 0;
}
