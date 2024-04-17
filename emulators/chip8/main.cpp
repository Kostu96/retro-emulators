#include "chip8.hpp"

#include "shared/source/application.hpp"

#include <thread>

class CHIP8App :
    public Application
{
public:
    explicit CHIP8App(CHIP8& chip8) :
        Application{ {
                .windowTitle = "CHIP-8 Interpreter by Kostu96",
                .rendererWidth = 64,
                .rendererHeight = 32,
                .scale = 8,
                .border = 10,
                .hasMenuBar = false
        } },
        m_chip8{ chip8 }
    {}
private:
    std::span<const unsigned int> getScreenPixels() const override { return m_chip8.getScreenPixels(); }

    void onKeyCallback(int key, int action, int /*mods*/) override {
        m_chip8.handleKey(key, action);
    }

    CHIP8& m_chip8;
};

int main()
{
    CHIP8 chip8;
    CHIP8App app{ chip8 };

    //chip8.loadProgram("C:/Users/kmisiak/myplace/retro-extras/programs/chip8/Space Invaders [David Winter].ch8");
    chip8.loadProgram("C:/Users/Konstanty/Desktop/retro-extras/programs/chip8/Chip8 Picture.ch8");

    std::thread emuThread{
        [&]() {
            std::chrono::steady_clock::time_point lastFrameTime;
            std::chrono::duration<double, std::micro> elapsedTime{ 0 };
            while (app.isRunning()) {
                auto time = std::chrono::steady_clock::now();
                std::chrono::duration<double, std::micro> dt = time - lastFrameTime;
                elapsedTime += dt;
                lastFrameTime = time;

                chip8.update(dt.count() * 0.000001);

                std::this_thread::sleep_for(std::chrono::nanoseconds{ 64 });
            }
        }
    };

    app.run();
    emuThread.join();
	return 0;
}
