#include "apple2.hpp"

#include "emu_common/application.hpp"

#include <thread>

static constexpr u16 SCREEN_WIDTH = 40 * 8;
static constexpr u16 SCREEN_HEIGHT = 24 * 8;
static constexpr u16 SCALE = 2;
static constexpr u16 BORDER_SIZE = 10;
static constexpr u16 WINDOW_WIDTH = BORDER_SIZE * 2 + SCREEN_WIDTH * SCALE;
static constexpr u16 WINDOW_HEGHT = BORDER_SIZE * 2 + SCREEN_HEIGHT * SCALE;

class Emulator :
    public EmuCommon::Application
{
public:
    Emulator() :
        Application{"Apple II Emulator by Kostu96", WINDOW_WIDTH, WINDOW_HEGHT }
    {}
private:
};

int main()
{
    Emulator emulator;
    Apple2 apple;

    std::thread appleThread{
        [&]() {
            /*sf::Clock clock;
            u64 updates = 0;*/
            while (emulator.isRunning()) {
                //u64 realTimeNS = clock.restart().asMicroseconds() * 1000;
                //u64 emuTimeNS = updates * 500;
                //updates = 0;
                //if (realTimeNS - emuTimeNS > 1000000)
                //    sf::sleep(sf::microseconds(1000));

                apple.update();

                //updates++;
            }
        }
    };

    emulator.run();
    appleThread.join();
    return 0;
}
