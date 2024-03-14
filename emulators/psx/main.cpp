#include "psx.hpp"
#include <glad/gl.h>
#include <glw/glw.hpp>
#include <GLFW/glfw3.h>

#include <iostream>
#include <thread>
#include <memory>

constexpr u16 SCALE = 3;
constexpr u16 BORDER_SIZE = 10;
constexpr u16 VIEWPORT_X = BORDER_SIZE;
constexpr u16 VIEWPORT_Y = BORDER_SIZE;
constexpr u16 VIEWPORT_WIDTH = KIM1::SCREEN_WIDTH * SCALE;
constexpr u16 VIEWPORT_HEIGHT = KIM1::SCREEN_HEIGHT * SCALE;
constexpr u16 WINDOW_WIDTH = VIEWPORT_WIDTH + 2 * BORDER_SIZE;
constexpr u16 WINDOW_HEIGHT = VIEWPORT_HEIGHT + 2 * BORDER_SIZE;

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
    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Commodore KIM-1 emulator by Kostu96", nullptr, nullptr);
    if (!window) {
        std::cerr << "GLFW window creation failed!\n";
        std::terminate();
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    glw::init(glfwGetProcAddress);
    glw::Renderer::init();
    glClearColor(0.2f, 0.2f, 0.2f, 1.f);

    glw::Texture screenTexture{
        glw::Texture::Properties{
                glw::TextureSpecification{
                    glw::TextureFormat::RGBA8,
                    glw::TextureFilter::Nearest,
                    glw::TextureFilter::Nearest,
                    glw::TextureWrapMode::Clamp
                },
                PSX::SCREEN_WIDTH, PSX::SCREEN_HEIGHT
        }
    };

    std::unique_ptr<PSX> psx = std::make_unique<PSX>();
    std::thread emuThread{
        [&]() {
            while (!glfwWindowShouldClose(window)) {
                std::this_thread::sleep_for(std::chrono::nanoseconds{ 24 }); // TODO: temp
                psx->clock();
            }
        }
    };

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);
        glw::Renderer::beginFrame();
        glViewport(VIEWPORT_X, VIEWPORT_Y, VIEWPORT_WIDTH, VIEWPORT_HEIGHT);
        auto pixels = psx->getScreenPixels();
        screenTexture.setData(pixels.data(), pixels.size() * sizeof(u32));
        screenTexture.bind(0);
        glw::Renderer::renderTexture(-1.f, 1.f, 1.f, -1.f, 0.f, 0.f, 1.f, 1.f);
        glw::Renderer::endFrame();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    emuThread.join();

    glw::Renderer::shutdown();
    glfwTerminate();
    return 0;
}
