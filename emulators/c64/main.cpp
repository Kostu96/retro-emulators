#include "c64.hpp"
#include "emu_common/application.hpp"

#include <glad/gl.h>
#include <glw/glw.hpp>
#include <GLFW/glfw3.h>

#include <iostream>
#include <thread>
#include <memory>

constexpr u16 FRAME_WIDTH = 320;
constexpr u16 FRAME_HEIGHT = 200;
constexpr u16 SCALE = 2;
constexpr u16 WINDOW_WIDTH = FRAME_WIDTH * SCALE;
constexpr u16 WINDOW_HEIGHT = FRAME_HEIGHT * SCALE;

int main()
{
    glfwSetErrorCallback(EmuCommon::glfwErrorCallback);
    if (!glfwInit()) {
        std::cerr << "GLFW init failed!\n";
        std::terminate();
    }

    glfwWindowHint(GLFW_RESIZABLE, 0);
    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Commodore 64 emulaator by Kostu96", nullptr, nullptr);
    if (!window) {
        std::cerr << "GLFW window creation failed!\n";
        std::terminate();
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    glw::init(glfwGetProcAddress);
    glw::Renderer::init();
    glClearColor(0.f, 136.f / 255.f, 1.f, 1.f);

    std::unique_ptr<C64> c64 = std::make_unique<C64>();
    std::thread emuThread{
        [&]() {
            while (!glfwWindowShouldClose(window)) {
                c64->clock();
            }
        }
    };

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);
        glw::Renderer::beginFrame();
        glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
        //auto pixels = gameboy.getPPU().getScreenPixels();
        //screenTexture.setData(pixels.data(), pixels.size() * sizeof(u32));
        //screenTexture.bind(0);
        //glw::Renderer::renderTexture(-1.f, 1.f, 1.f, -1.f, 0.f, 0.f, 1.f, 1.f);
        //glw::Renderer::endFrame();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    emuThread.join();

    glw::Renderer::shutdown();
    glfwTerminate();
    return 0;
}
