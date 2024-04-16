#include "c64.hpp"

#include "shared/source/application.hpp"

#include <thread>

class C64App :
    public Application
{
public:
    C64App() :
        Application{ {
                .windowTitle = "Commodore 64 emulaator by Kostu96",
                .rendererWidth = 320,
                .rendererHeight = 200, // TODO: extract magic values as in PSX
                .scale = 2,
                .border = 10
        } }
    {}
private:

};

int main()
{
    C64App app;
    std::unique_ptr<C64> c64 = std::make_unique<C64>();

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
