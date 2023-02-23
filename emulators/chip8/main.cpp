#include "chip8.hpp"

#include "emu_common/gui.hpp"

#include <glad/gl.h>
#include <glw/glw.hpp>
#include <GLFW/glfw3.h>

#include <iostream>
#include <thread>

constexpr u16 FRAME_WIDTH = 64;
constexpr u16 FRAME_HEIGHT = 32;
constexpr u16 SCALE = 8;
constexpr u16 BORDER_SIZE = 10;
constexpr u16 IMGUI_MENU_BAR_HEIGHT = 0;
constexpr u16 WINDOW_WIDTH = FRAME_WIDTH * SCALE + 2 * BORDER_SIZE;
constexpr u16 WINDOW_HEIGHT = FRAME_HEIGHT * SCALE + 2 * BORDER_SIZE;

static void glfwErrorCallback(int error, const char* description)
{
    std::cerr << "GLFW error " << error << ": " << description << '\n';
}

int main()
{
    glfwSetErrorCallback(glfwErrorCallback);
    if (!glfwInit()) {
        std::cerr << "GLFW init failed!\n";
        std::terminate();
    }

    glfwWindowHint(GLFW_RESIZABLE, 0);
    GLFWwindow* window =
        glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT + IMGUI_MENU_BAR_HEIGHT, "CHIP-8 Interpreter by Kostu96", nullptr, nullptr);
    if (!window) {
        std::cerr << "GLFW window creation failed!\n";
        std::terminate();
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    glw::init(glfwGetProcAddress);
    glw::Renderer::init();
    glClearColor(0.8f, 0.8f, 0.7f, 1.f);

    EmuCommon::GUI::init(window);

    CHIP8 chip8;
    chip8.loadProgram("C:/Users/kmisiak/myplace/retro-extras/programs/chip8/Fishie.ch8");

    EmuCommon::GUI::DisassemblyView disasmView;

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        EmuCommon::GUI::beginFrame();
        disasmView.updateWindow(chip8.getDisassembly());
        EmuCommon::GUI::endFrame();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    EmuCommon::GUI::shutdown();
    glw::Renderer::shutdown();
    glfwTerminate();
	return 0;
}
