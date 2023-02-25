#include "altair.hpp"
#include "gui.hpp"
#include  "emu_common/gui.hpp"

#include <SFML/GpuPreference.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/System/Sleep.hpp>

#include <iostream>
#include <thread>

SFML_DEFINE_DISCRETE_GPU_PREFERENCE

int main()
{
    sf::RenderWindow window{ { 600, 300 }, "Altair 8800 Emulator by Kostu96", sf::Style::Close };
    window.setVerticalSyncEnabled(true);

    Altair altair;
    bool isPaused = true;

    std::thread emuThread{
        [&]() {
            sf::Clock clock;
            u64 updates = 0;
            while (window.isOpen()) {
                u64 realTimeNS = clock.restart().asMicroseconds() * 1000;
                u64 emuTimeNS = updates * 500;
                updates = 0;
                if (realTimeNS - emuTimeNS > 1000000)
                    sf::sleep(sf::microseconds(1000));
                
                if (!isPaused)
                    altair.update();

                updates++;
            }
        }
    };

    while (window.isOpen())
    {
        window.clear();

        window.display();

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
    }

    emuThread.join();
    return 0;
}
