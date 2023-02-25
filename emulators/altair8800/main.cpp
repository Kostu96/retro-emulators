#include "altair.hpp"
#include "gui.hpp"
#include  "emu_common/gui.hpp"

#include <SDL.h>

#include <iostream>
#include <thread>

int main(int /*argc*/, char* /*argv*/[])
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
        return -1;

    SDL_Window* window = SDL_CreateWindow("Altair 8800 Emulator by Kostu96", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 600, 400, SDL_WINDOW_SHOWN);
    if (!window)
        return -1;

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer)
        return -1;

    Altair altair;
    bool shouldQuit = false;

    std::thread emuThread{
        [&]() {
            /*sf::Clock clock;
            u64 updates = 0;*/
            while (!shouldQuit) {
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
    
    while (!shouldQuit)
    {
        SDL_RenderClear(renderer);

        SDL_RenderPresent(renderer);

        SDL_Event e;
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT) shouldQuit = true;
        }
    }

    emuThread.join();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
