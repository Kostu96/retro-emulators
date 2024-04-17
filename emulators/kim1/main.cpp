#include "kim1.hpp"

#include "shared/source/application.hpp"

#include <thread>

//constexpr u16 BORDER_SIZE = 16;
//constexpr u16 IMGUI_MENU_BAR_HEIGHT = 18;
//constexpr u16 WINDOW_WIDTH = 6 * 32 + 3 * BORDER_SIZE;
//constexpr u16 WINDOW_HEIGHT = 8 * 32 + 2 * BORDER_SIZE;

class KIM1App :
    public Application
{
public:
    KIM1App() :
        Application{ {
                .windowTitle = "KIM-1 Emulator by Kostu96",
                .rendererWidth = 6 * 32,
                .rendererHeight = 8 * 32, // TODO: extract magic values as in PSX
                .scale = 2,
                .border = 16
        } }
    {}
private:
    std::span<const unsigned int> getScreenPixels() const override { return {}; }
};

int main()
{
    KIM1App app;
    std::unique_ptr<KIM1> kim1 = std::make_unique<KIM1>();

    std::thread emuThread{
        [&]() {
            while (app.isRunning()) {
                std::this_thread::sleep_for(std::chrono::nanoseconds{ 32 }); // TODO: temp
                kim1->clock();
            }
        }
    };

    app.run();
    emuThread.join();
    return 0;
}
