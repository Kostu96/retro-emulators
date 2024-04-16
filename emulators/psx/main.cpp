#include "psx.hpp"

#include "shared/source/application.hpp"

#include <glad/gl.h>
#include <glw/glw.hpp>
#include <GLFW/glfw3.h>

#include <iostream>
#include <thread>
#include <memory>

class PSXApp :
    public Application
{
public:
    PSXApp() :
        Application{ {
                .windowTitle = "PSX emulator by Kostu96",
                .rendererWidth = PSX::SCREEN_WIDTH,
                .rendererHeight = PSX::SCREEN_HEIGHT,
                .scale = 2,
                .border = 10
        } }
    {}
private:

};

int main()
{
    PSXApp app;
    std::unique_ptr<PSX::Emulator> psx = std::make_unique<PSX::Emulator>();

    std::thread emuThread{
        [&]() {
            while (app.isRunning()) {
                std::this_thread::sleep_for(std::chrono::nanoseconds{ 24 }); // TODO: temp
                psx->clock();
            }
        }
    };

    app.run();
    emuThread.join();

    glw::Renderer::shutdown();
    glfwTerminate();
    return 0;
}