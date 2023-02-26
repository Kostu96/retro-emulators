#include "altair.hpp"
#include "emu_common/application.hpp"
#include "emu_common/sdl_helpers.hpp"

#include <SDL.h>

#include <iostream>
#include <thread>

class Emulator :
    public EmuCommon::Application
{
public:
    Emulator() :
        Application{ "Altair 8800 Emulator by Kostu96", 1024, 400 }
    {
        SDL_SetRenderDrawColor(getSDLRenderer(), 0x50, 0x50, 0x50, 0xFF);

        m_switchTexture.loadFromFile("assets/altair/switch.png");
    }
protected:
    void onRender() override {
        for (int i = 0; i < 16; i++)
        {
            const SDL_Rect srcRect{ 2 * 100, 0, 100, 143 };
            const SDL_FRect dstRect{ (10.f + 100 / 2.5f) * i, 10, 100 / 2.5f, 143 / 2.5f };
            SDL_RenderCopyF(getSDLRenderer(), m_switchTexture, &srcRect, &dstRect);
        }
    }
private:
    EmuCommon::SDLTexture m_switchTexture;
};

int main(int /*argc*/, char* /*argv*/[])
{
    Emulator emulator;
    Altair altair;

    std::thread altairThread{
        [&]() {
            /*sf::Clock clock;
            u64 updates = 0;*/
            while (emulator.isRunning()) {
                //u64 realTimeNS = clock.restart().asMicroseconds() * 1000;
                //u64 emuTimeNS = updates * 500;
                //updates = 0;
                //if (realTimeNS - emuTimeNS > 1000000)
                //    sf::sleep(sf::microseconds(1000));
                
                //altair.update();

                //updates++;
            }
        }
    };

    emulator.run();
    altairThread.join();
    return 0;
}
