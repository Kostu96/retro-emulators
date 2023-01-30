#include "../shared/emulator_core.hpp"
#include "dll_loader.hpp"
#include "gui.hpp"
#include "renderer.hpp"

#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <chrono>

static void glfwErrorCallback(int error, const char* description)
{
    std::cerr << "GLFW error " << error << ": " << description << '\n';
}

static void glfwKeyCallback(GLFWwindow* window, int key, int /*scancode*/, int action, int /*mods*/)
{
    auto core = reinterpret_cast<EmulatorCore*>(glfwGetWindowUserPointer(window));
    core->handleKey(key, action);
}

int main(int argc, char* argv[])
{
    if (argc < 2) {
        std::cerr << "Invalid number of arguments.\n";
        std::terminate();
    }

    DllLoader<EmulatorCore> loader{ argv[1] };
    if (!loader.openLib()) {
        std::cerr << "Cannot open library: " << argv[1] << '\n';
        std::terminate();
    }

    {
        auto core = loader.createInstance();
        core->setClearCallback(Renderer::clear);
        core->setRenderPointCallback(Renderer::renderPoint);
            
        if (argc > 2)
            core->loadROM(argv[2]);
            
        core->reset();

        glfwSetErrorCallback(glfwErrorCallback);
        if (!glfwInit()) {
            std::cerr << "GLFW init failed!\n";
            std::terminate();
        }

        constexpr u16 BORDER_SIZE = 16;
        auto& settings = core->getEmulatorSettings();
        GLFWwindow* window = glfwCreateWindow(settings.windowWidth + 2 * BORDER_SIZE, settings.windowHeight + 2 * BORDER_SIZE,
                                                settings.windowTitle, nullptr, nullptr);
        if (!window) {
            std::cerr << "GLFW window creation failed!\n";
            std::terminate();
        }
        glfwSetKeyCallback(window, glfwKeyCallback);
        glfwSetWindowUserPointer(window, core.get());
        glfwMakeContextCurrent(window);

        GUI::init(window);
        Renderer::init(settings.frameWidth, settings.frameHeight, glfwGetProcAddress);

        GUI::MemoryView memoryView;
        GUI::DisassemblyView disassemblyView;
        GUI::StateView stateView;

        bool paused = true;
        std::chrono::steady_clock::time_point lastFrameTime;
        std::chrono::duration<double, std::micro> elapsedTime{ 0 };
        while (!glfwWindowShouldClose(window))
        {
            auto time = std::chrono::steady_clock::now();
            std::chrono::duration<double, std::micro> dt = time - lastFrameTime;
            elapsedTime += dt;
            lastFrameTime = time;

            if (elapsedTime > std::chrono::duration<double, std::milli>(16.67))
            {
                elapsedTime -= std::chrono::duration<double, std::milli>(16.67);

                glClear(GL_COLOR_BUFFER_BIT);

                Renderer::beginFrame();

                if (!paused)
                    core->update(dt.count() * 0.001);

                Renderer::renderFrame(BORDER_SIZE, BORDER_SIZE, settings.windowWidth, settings.windowHeight);

                GUI::beginFrame();

                for (size_t i = 0; i < core->getNumMemories(); i++)
                    memoryView.drawWindow((std::string{ "Memory " } + std::to_string(i)).c_str(), core, i);
                disassemblyView.drawWindow(core);
                stateView.drawWindow(core, paused, dt.count() * 0.001);

                GUI::renderFrame();

                glfwSwapBuffers(window);
            }

            glfwPollEvents();
        }

        Renderer::shutdown();
        GUI::shutdown();
        glfwTerminate();

    }
    loader.closeLib();

    return 0;
}
