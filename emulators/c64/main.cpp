#include "c64.hpp"

#include "shared/source/application.hpp"

#include <thread>

class C64App :
    public Application
{
public:
    C64App() :
        Application{ {
                .windowTitle = "Commodore 64 Emulator by Kostu96",
                .rendererWidth = C64::SCREEN_WIDTH,
                .rendererHeight = C64::SCREEN_HEIGHT,
                .scale = 2,
                .border = 10,
                .hasMenuBar = false
        } }
    {}
private:
    std::span<const unsigned int> getScreenPixels() const override { return {}; }
};

int main()
{
    C64App app;
    std::unique_ptr<C64::Emulator> c64 = std::make_unique<C64::Emulator>();

    std::thread emuThread{
        [&]() {
            while (app.isRunning()) {
                c64->clock();
            }
        }
    };

    app.run();
    emuThread.join();
    return 0;
}
