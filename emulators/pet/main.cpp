#include "pet.hpp"
#include "emu_common/application.hpp"
#include "emu_common/imgui_helper.hpp"

#include <glad/gl.h>
#include <glw/glw.hpp>
#include <GLFW/glfw3.h>
#include <imgui.h>

#include <chrono>
#include <iostream>
#include <thread>
#include <memory>

constexpr u16 SCALE = 4;
constexpr u16 BORDER_SIZE = 16;
constexpr u16 VIEWPORT_X = BORDER_SIZE;
constexpr u16 VIEWPORT_Y = BORDER_SIZE/* - 10*/;
constexpr u16 VIEWPORT_WIDTH = PET::SCREEN_WIDTH * SCALE;
constexpr u16 VIEWPORT_HEIGHT = PET::SCREEN_HEIGHT * SCALE;
constexpr u16 WINDOW_WIDTH = VIEWPORT_WIDTH + 2 * BORDER_SIZE;
constexpr u16 WINDOW_HEIGHT = VIEWPORT_HEIGHT + 2 * BORDER_SIZE;

static void glfwKeyCallback(GLFWwindow* window, int key, int /*scancode*/, int action, int mods)
{
    auto pet = reinterpret_cast<PET*>(glfwGetWindowUserPointer(window));
    pet->updateKeysFromEvent(key, action != GLFW_RELEASE, mods & GLFW_MOD_SUPER && mods & GLFW_MOD_SHIFT);
}

static void glfwTextCallback(GLFWwindow* window, unsigned int codepoint)
{
    auto pet = reinterpret_cast<PET*>(glfwGetWindowUserPointer(window));
    pet->updateKeysFromCodepoint(codepoint);
}

int main()
{
    std::unique_ptr<PET> pet = std::make_unique<PET>();

    glfwSetErrorCallback(EmuCommon::glfwErrorCallback);
    if (!glfwInit()) {
        std::cerr << "GLFW init failed!\n";
        std::terminate();
    }

    glfwWindowHint(GLFW_RESIZABLE, 0);
    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT + 16, "Commodore PET emulator by Kostu96", nullptr, nullptr);
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

    EmuCommon::GUI::init(window);
    /*bool emuSpeed50 = false;
    bool emuSpeed100 = true;
    bool emuSpeed150 = false;*/

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

    auto tickTime = std::chrono::nanoseconds(1000);
    std::thread emuThread{
        [&]() {
            constexpr auto sleepTime = std::chrono::milliseconds(8);
            s64 ticks = 0;
            auto t1 = std::chrono::high_resolution_clock().now();
            while (!glfwWindowShouldClose(window)) {
                auto elapsedTime = std::chrono::high_resolution_clock().now() - t1;
                auto emulatedTime = ticks * tickTime;
                if (emulatedTime.count() - elapsedTime.count() > sleepTime.count())
                    std::this_thread::sleep_for(sleepTime);
                
                pet->clock();
                ticks++;
            }
        }
    };
    
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);
        glw::Renderer::beginFrame();
        glViewport(VIEWPORT_X, VIEWPORT_Y, VIEWPORT_WIDTH, VIEWPORT_HEIGHT);
        auto pixels = pet->getScreenPixels();
        screenTexture.setData(pixels.data(), pixels.size() * sizeof(u32));
        screenTexture.bind(0);
        glw::Renderer::renderTexture(-1.f, 1.f, 1.f, -1.f, 0.f, 0.f, 1.f, 1.f);
        glw::Renderer::endFrame();

        EmuCommon::GUI::beginFrame();

        ImGui::BeginMainMenuBar();
        if (ImGui::BeginMenu("File"))
        {
            /*if (ImGui::MenuItem("Load cartridge...")) {
                static const char* filters[1] = { "*.gb" };
                char* filename = tinyfd_openFileDialog("Load cartridge", nullptr, 1, filters, nullptr, 0);
                if (filename) {
                    gb.loadCartridge(filename);
                    gb.reset();
                }
            }
            if (ImGui::MenuItem("Reset")) gb.reset();*/
            if (ImGui::MenuItem("Exit", "Alt+F4")) glfwSetWindowShouldClose(window, true);

            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Settings"))
        {
           /* if (ImGui::BeginMenu("Emualtion Speed"))
            {
                if (ImGui::MenuItem("50%", nullptr, &emuSpeed50, !emuSpeed50)) {
                    emuSpeed100 = false;
                    emuSpeed150 = false;
                    tickTime = std::chrono::nanoseconds(2000);
                }
                if (ImGui::MenuItem("100%", nullptr, &emuSpeed100, !emuSpeed100)) {
                    emuSpeed50 = false;
                    emuSpeed150 = false;
                    tickTime = std::chrono::nanoseconds(1000);
                }
                if (ImGui::MenuItem("150%", nullptr, &emuSpeed150, !emuSpeed150)) {
                    emuSpeed50 = false;
                    emuSpeed100 = false;
                    tickTime = std::chrono::nanoseconds(667);
                }

                ImGui::EndMenu();
            }*/

            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();

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
