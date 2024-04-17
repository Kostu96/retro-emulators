#include "vic20.hpp"

#include "shared/source/application.hpp"

#include <glad/gl.h>
#include <glw/glw.hpp>
#include <GLFW/glfw3.h>

#include <iostream>
#include <thread>
#include <memory>

class VIC20App :
    public Application
{
public:
    explicit VIC20App(VIC20& vic20) :
        Application{ {
                .windowTitle = "Commodore VIC-20 Emulator by Kostu96",
                .rendererWidth = VIC20::SCREEN_WIDTH,
                .rendererHeight = VIC20::SCREEN_HEIGHT,
                .scale = 3,
                .border = 10,
                .hasMenuBar = false
        } },
        m_vic20{ vic20 }
    {}
private:
    std::span<const unsigned int> getScreenPixels() const override { return m_vic20.getScreenPixels(); }

    VIC20& m_vic20;
};

int main()
{
    std::unique_ptr<VIC20> vic20 = std::make_unique<VIC20>();
    VIC20App app{ *vic20.get() };

    std::thread emuThread{
        [&]() {
            while (app.isRunning()) {
                std::this_thread::sleep_for(std::chrono::nanoseconds{ 24 }); // TODO: temp
                vic20->clock();
            }
        }
    };

    app.run();
    emuThread.join();
    return 0;
}
