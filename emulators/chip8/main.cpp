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

    glw::Texture screenTexture{
        glw::Texture::Properties{
                glw::TextureSpecification{
                    glw::TextureFormat::RGBA8,
                    glw::TextureFilter::Nearest,
                    glw::TextureFilter::Nearest,
                    glw::TextureWrapMode::Clamp
                },
                FRAME_WIDTH, FRAME_HEIGHT
        }
    };

    CHIP8 chip8;
    //chip8.loadProgram("C:/Users/kmisiak/myplace/retro-extras/programs/chip8/Fishie.ch8");
    chip8.loadProgram("C:/Users/Konstanty/Desktop/retro-extras/programs/chip8/Chip8 Picture.ch8");

    EmuCommon::GUI::DisassemblyView disasmView;

    std::thread emuThread{
        [&]() {
            while (!glfwWindowShouldClose(window)) {
                std::this_thread::sleep_for(std::chrono::nanoseconds{ 32 }); // TODO: temp
                chip8.update();
            }
        }
    };

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        glw::Renderer::beginFrame();
        glViewport(BORDER_SIZE, BORDER_SIZE, WINDOW_WIDTH - 2 * BORDER_SIZE, WINDOW_HEIGHT - 2 * BORDER_SIZE);
        auto pixels = chip8.getScreenPixels();
        screenTexture.setData(pixels.data(), pixels.size() * sizeof(u32));
        screenTexture.bind(0);
        glw::Renderer::renderTexture(-1.f, 1.f, 1.f, -1.f, 0.f, 0.f, 1.f, 1.f);
        glw::Renderer::endFrame();

        EmuCommon::GUI::beginFrame();
        disasmView.updateWindow(chip8.getDisassembly());
        EmuCommon::GUI::endFrame();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    emuThread.join();

    EmuCommon::GUI::shutdown();
    glw::Renderer::shutdown();
    glfwTerminate();
	return 0;
}
