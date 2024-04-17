#include "pet.hpp"

#include "shared/source/application.hpp"

#include <GLFW/glfw3.h> // TODO: abstract this
#include <imgui.h>

#include <chrono>
#include <thread>

class PETApp :
    public Application
{
public:
    explicit PETApp(PET& pet) :
        Application{ {
                .windowTitle = "Commodore PET Emulator by Kostu96",
                .rendererWidth = PET::SCREEN_WIDTH,
                .rendererHeight = PET::SCREEN_HEIGHT,
                .scale = 4,
                .border = 2,
                .hasMenuBar = true
        } },
        m_pet{ pet }
    {}
private:
    std::span<const unsigned int> getScreenPixels() const override { return m_pet.getScreenPixels(); }

    void onImGUIRender() override {
        ImGui::BeginMainMenuBar();
        if (ImGui::BeginMenu("File"))
        {
            /*if (ImGui::MenuItem("Load cartridge...")) {
                static const char* filters[1] = { "*.gb" };
                char* filename = tinyfd_openFileDialog("Load cartridge", nullptr, 1, filters, nullptr, 0);
                if (filename) {
                    gb.loadCartridge(filename);
                    gb.reset();
                }
            }
            if (ImGui::MenuItem("Reset")) gb.reset();*/
            if (ImGui::MenuItem("Exit", "Alt+F4")) stop();

            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Settings"))
        {
            /* if (ImGui::BeginMenu("Emualtion Speed"))
             {
                 if (ImGui::MenuItem("50%", nullptr, &emuSpeed50, !emuSpeed50)) {
                     emuSpeed100 = false;
                     emuSpeed150 = false;
                     tickTime = std::chrono::nanoseconds(2000);
                 }
                 if (ImGui::MenuItem("100%", nullptr, &emuSpeed100, !emuSpeed100)) {
                     emuSpeed50 = false;
                     emuSpeed150 = false;
                     tickTime = std::chrono::nanoseconds(1000);
                 }
                 if (ImGui::MenuItem("150%", nullptr, &emuSpeed150, !emuSpeed150)) {
                     emuSpeed50 = false;
                     emuSpeed100 = false;
                     tickTime = std::chrono::nanoseconds(667);
                 }

                 ImGui::EndMenu();
             }*/

            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }

    void onKeyCallback(int key, int action, int mods) override {
        m_pet.updateKeysFromEvent(key, action != GLFW_RELEASE, mods & GLFW_MOD_SUPER && mods & GLFW_MOD_SHIFT);
    }

    void onTextCallback(unsigned int codepoint) override {
        m_pet.updateKeysFromCodepoint(codepoint);
    }

    PET& m_pet;
};

int main()
{
    std::unique_ptr<PET> pet = std::make_unique<PET>();
    PETApp app{ *pet.get() };

    auto tickTime = std::chrono::nanoseconds(1000);
    std::thread emuThread{
        [&]() {
            constexpr auto sleepTime = std::chrono::milliseconds(8);
            s64 ticks = 0;
            auto t1 = std::chrono::high_resolution_clock().now();
            while (app.isRunning()) {
                auto elapsedTime = std::chrono::high_resolution_clock().now() - t1;
                auto emulatedTime = ticks * tickTime;
                if (emulatedTime.count() - elapsedTime.count() > sleepTime.count())
                    std::this_thread::sleep_for(sleepTime);
                
                pet->clock();
                ticks++;
            }
        }
    };

    app.run();
    emuThread.join();
    return 0;
}
