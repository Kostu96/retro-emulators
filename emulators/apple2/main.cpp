#include "apple2.hpp"

#include "emu_common/application.hpp"
#include "emu_common/graphics/image.hpp"

//#include <SDL.h>
#include <thread>

static constexpr u16 SCALE = 2;
static constexpr u16 BORDER_SIZE = 10;
static constexpr u16 WINDOW_WIDTH = BORDER_SIZE * 2 + Apple2::SCREEN_WIDTH * SCALE;
static constexpr u16 WINDOW_HEGHT = BORDER_SIZE * 2 + Apple2::SCREEN_HEIGHT * SCALE;

class Emulator :
    public EmuCommon::Application
{
public:
    Emulator(Apple2& apple) :
        m_apple{ apple },
        Application{"Apple II Emulator by Kostu96", WINDOW_WIDTH, WINDOW_HEGHT }
    {
        auto pixelsSpan = m_apple.getScreenPixels();
        m_screen.createFromPixels(pixelsSpan.data(), Apple2::SCREEN_WIDTH, Apple2::SCREEN_HEIGHT);

        m_screen.setScale({ SCALE, SCALE });
        m_screen.setPosition({ BORDER_SIZE, BORDER_SIZE });
    }
protected:
    void onUpdate() override
    {
        auto renderer = getSDLRenderer();

        //SDL_SetRenderDrawColor(renderer, 0xDA, 0xDA, 0xDA, 0xFF);
        //SDL_RenderClear(renderer);

        m_screen.render(renderer);
    }
private:
    Apple2& m_apple;
    EmuCommon::SDLImage m_screen;
};

int main(int /*argc*/, char* /*argv*/[])
{
    Apple2* apple = new Apple2{};
    Emulator emulator{ *apple };

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

                apple->update();

                //updates++;
            }
        }
    };

    emulator.run();
    appleThread.join();
    delete apple;
    return 0;
}
