#include "gameboy.hpp"
#include "gui.hpp"

#include "shared/source/application.hpp"

#include <thread>

class GameboyApp :
    public Application
{
public:
    explicit GameboyApp(Gameboy& gameboy) :
        Application{ {
                .windowTitle = "Gameboy Emulator by Kostu96",
                .rendererWidth = 160,
                .rendererHeight = 144,
                .scale = 3,
                .border = 10,
                .hasMenuBar = true
        } },
        m_gameboy{ gameboy }
    {}
private:
    std::span<const unsigned int> getScreenPixels() const override { return m_gameboy.getPPU().getScreenPixels(); }

    void onImGUIRender() override {
        GUI::update(m_gameboy);
    }

    Gameboy& m_gameboy;
};

int main()
{
    Gameboy gameboy;
    GameboyApp app{ gameboy };

    GUI::init(&app);

    std::thread emuThread{
        [&]() {
            while (app.isRunning()) {
                std::this_thread::sleep_for(std::chrono::nanoseconds{ 24 }); // TODO: temp
                gameboy.update();
            }
        }
    };

    app.run();
    emuThread.join();
    GUI::shutdown();
    return 0;
}
