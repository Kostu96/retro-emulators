#include "pet.hpp"
#include <glad/gl.h>
#include <glw/glw.hpp>
#include <GLFW/glfw3.h>

#include <chrono>
#include <iostream>
#include <thread>
#include <memory>

constexpr u16 SCALE = 3;
constexpr u16 BORDER_SIZE = 10;
constexpr u16 VIEWPORT_X = BORDER_SIZE;
constexpr u16 VIEWPORT_Y = BORDER_SIZE;
constexpr u16 VIEWPORT_WIDTH = PET::SCREEN_WIDTH * SCALE;
constexpr u16 VIEWPORT_HEIGHT = PET::SCREEN_HEIGHT * SCALE;
constexpr u16 WINDOW_WIDTH = VIEWPORT_WIDTH + 2 * BORDER_SIZE;
constexpr u16 WINDOW_HEIGHT = VIEWPORT_HEIGHT + 2 * BORDER_SIZE;

static void glfwErrorCallback(int error, const char* description)
{
    std::cerr << "GLFW error " << error << ": " << description << '\n';
}

static void glfwKeyCallback(GLFWwindow* window, int key, int /*scancode*/, int action, int mods)
{
    auto pet = reinterpret_cast<PET*>(glfwGetWindowUserPointer(window));
    pet->updateKeysFromEvent(key, action != GLFW_RELEASE, mods & GLFW_MOD_CONTROL);
}

static void glfwTextCallback(GLFWwindow* window, unsigned int codepoint)
{
    auto pet = reinterpret_cast<PET*>(glfwGetWindowUserPointer(window));
    pet->updateKeysFromCodepoint(codepoint);
}

int main()
{
    char title[] = "Commodore PET emulator by Kostu96 | XXXXXXXXXXX MHz";
    std::unique_ptr<PET> pet = std::make_unique<PET>();

    glfwSetErrorCallback(glfwErrorCallback);
    if (!glfwInit()) {
        std::cerr << "GLFW init failed!\n";
        std::terminate();
    }

    glfwWindowHint(GLFW_RESIZABLE, 0);
    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, title, nullptr, nullptr);
    if (!window) {
        std::cerr << "GLFW window creation failed!\n";
        std::terminate();
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    glfwSetWindowUserPointer(window, pet.get());
    glfwSetKeyCallback(window, glfwKeyCallback);
    glfwSetCharCallback(window, glfwTextCallback);

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
                PET::SCREEN_WIDTH, PET::SCREEN_HEIGHT
        }
    };

    size_t tickPerSecond = 0;
    std::thread emuThread{
        [&]() {
            size_t ticks = 0;
            std::chrono::duration<long long, std::nano> timer{};

            while (!glfwWindowShouldClose(window)) {
                auto t1 = std::chrono::steady_clock().now();
                //std::this_thread::sleep_for(std::chrono::nanoseconds{ 10 }); // TODO: temp
                pet->clock();
                ticks++;

                auto t2 = std::chrono::steady_clock().now();
                auto dt = t2 - t1;
                timer += dt;
                if (timer > std::chrono::milliseconds(500)) {
                    timer -= std::chrono::milliseconds(500);
                    t1 = t2;
                    tickPerSecond = ticks * 2;
                    ticks = 0;
                }
            }
        }
    };

    while (!glfwWindowShouldClose(window))
    {
        sprintf_s(title, "Commodore PET emulator by Kostu96 | %zu kHz", tickPerSecond / 1'000);
        glfwSetWindowTitle(window, title);

        glClear(GL_COLOR_BUFFER_BIT);
        glw::Renderer::beginFrame();
        glViewport(VIEWPORT_X, VIEWPORT_Y, VIEWPORT_WIDTH, VIEWPORT_HEIGHT);
        auto pixels = pet->getScreenPixels();
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
