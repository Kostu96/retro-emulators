#include "invaders.hpp"

#include "shared/source/application.hpp"
#include "shared/source/imgui/debug_view.hpp"

#include <imgui.h>
#include <thread>

static constexpr u16 SCREEN_WIDTH = 256;
static constexpr u16 SCREEN_HEIGHT = 224;

class InvadersApp :
    public Application
{
public:
    InvadersApp(Invaders& invaders) :
        Application{ {
                .windowTitle = "Space Invaders Emulator by Kostu96",
                .rendererWidth = SCREEN_WIDTH,
                .rendererHeight = SCREEN_HEIGHT,
                .scale = 2,
                .border = 10,
                .hasMenuBar = true
        } },
        m_invaders{ invaders },
        dummyPixelData{ new u32[SCREEN_WIDTH * SCREEN_HEIGHT] }
    {
        m_debugView.cpuStepCallback = [&]() {
            m_invaders.clock();
            };

        m_debugView.cpuStatusCallback = [&]() {
            const auto& cpuStatus = m_invaders.getCPU().getState();
            ImGui::Text("CPU Registers");
            ImGui::Separator();
            ImGui::Text("PC: %04X", cpuStatus.PC); ImGui::SameLine();
            ImGui::Text("SP: %04X", cpuStatus.SP); ImGui::SameLine();
            ImGui::Text("A: %02X", cpuStatus.A); ImGui::SameLine();
            ImGui::Text("F: %02X", cpuStatus.F);
            ImGui::Text("BC: %04X", cpuStatus.BC); ImGui::SameLine();
            ImGui::Text("DE: %04X", cpuStatus.DE); ImGui::SameLine();
            ImGui::Text("HL: %04X", cpuStatus.HL); ImGui::SameLine();
            };
    }
private:
    std::span<const unsigned int> getScreenPixels() const override { return { dummyPixelData.get(), SCREEN_WIDTH * SCREEN_HEIGHT }; }

    void onImGUIRender() override {
        ImGui::BeginMainMenuBar();
        if (ImGui::BeginMenu("Emulator"))
        {
            ImGui::MenuItem("Start");
            if (ImGui::MenuItem("Reset")) m_invaders.reset();
            if (ImGui::MenuItem("Exit", "Alt+F4")) exit();
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Settings"))
        {
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Debug"))
        {
            ImGui::MenuItem("Debug Control&Status", nullptr, &m_debugView.open);
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();

        m_debugView.updateWindow();
    }

    Invaders& m_invaders;
    imgui::DebugView m_debugView;

    std::unique_ptr<u32> dummyPixelData;
};

int main()
{
    std::unique_ptr<Invaders> invaders = std::make_unique<Invaders>();
    InvadersApp app{ *invaders.get() };

    std::thread emuThread{
        [&]() {
            while (app.isRunning()) {
                //std::this_thread::sleep_for(std::chrono::nanoseconds{ 32 }); // TODO: temp
                invaders->clock();
            }
        }
    };

    app.run();
    emuThread.join();
    return 0;
}
