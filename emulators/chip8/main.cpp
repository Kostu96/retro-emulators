#include "chip8.hpp"

#include "shared/source/imgui/imgui_helper.hpp"
#include "shared/source/imgui/disassembly_view.hpp"

#include <glad/gl.h>
#include <glw/glw.hpp>
#include <GLFW/glfw3.h>

#include <iostream>
#include <thread>

constexpr u16 FRAME_WIDTH = 64;
constexpr u16 FRAME_HEIGHT = 32;
constexpr u16 SCALE = 8;
constexpr u16 BORDER_SIZE = 8;
constexpr u16 IMGUI_MENU_BAR_HEIGHT = 0;
constexpr u16 WINDOW_WIDTH = FRAME_WIDTH * SCALE + 2 * BORDER_SIZE;
constexpr u16 WINDOW_HEIGHT = FRAME_HEIGHT * SCALE + 2 * BORDER_SIZE;

static void glfwErrorCallback(int error, const char* description)
{
    std::cerr << "GLFW error " << error << ": " << description << '\n';
}

static void glfwKeyCallback(GLFWwindow* window, int key, int /*scancode*/, int action, int /*mods*/)
{
    auto chip8 = reinterpret_cast<CHIP8*>(glfwGetWindowUserPointer(window));
    chip8->handleKey(key, action);
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

    imgui::init(window);

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
    chip8.loadProgram("C:/Users/kmisiak/myplace/retro-extras/programs/chip8/Space Invaders [David Winter].ch8");
    //chip8.loadProgram("C:/Users/Konstanty/Desktop/retro-extras/programs/chip8/Chip8 Picture.ch8");

    imgui::DisassemblyView disasmView;

    std::thread emuThread{
        [&]() {
            std::chrono::steady_clock::time_point lastFrameTime;
            std::chrono::duration<double, std::micro> elapsedTime{ 0 };
            while (!glfwWindowShouldClose(window)) {
                auto time = std::chrono::steady_clock::now();
                std::chrono::duration<double, std::micro> dt = time - lastFrameTime;
                elapsedTime += dt;
                lastFrameTime = time;

                chip8.update(dt.count() * 0.000001);

                std::this_thread::sleep_for(std::chrono::nanoseconds{ 64 });
            }
        }
    };

    glfwSetKeyCallback(window, glfwKeyCallback);
    glfwSetWindowUserPointer(window, &chip8);

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

        imgui::beginFrame();
        disasmView.updateWindow(chip8.getDisassembly());
        imgui::endFrame();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    emuThread.join();

    imgui::shutdown();
    glw::Renderer::shutdown();
    glfwTerminate();
	return 0;
}
