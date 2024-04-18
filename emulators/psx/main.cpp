#include "psx.hpp"

#include "shared/source/application.hpp"
#include "shared/source/imgui/debug_view.hpp"
#include "shared/source/imgui/memory_view.hpp"

#include <imgui.h>
#include <thread>

class PSXApp :
    public Application
{
public:
    PSXApp(PSX::Emulator& psx) :
        Application{ {
                .windowTitle = "PSX Emulator by Kostu96",
                .rendererWidth = PSX::SCREEN_WIDTH,
                .rendererHeight = PSX::SCREEN_HEIGHT,
                .scale = 2,
                .border = 10,
                .hasMenuBar = true
        } },
        m_psx{ psx }
    {
        m_debugView.cpuStatusCallback = [&]() {
            ImGui::Text("CPU Registers");
            ImGui::Separator();
            ImGui::Text("PC: %08X", m_psx.getCPU().getCPUStatus().PC);
            for (auto i = 0; i < 32; i++) {
                if (i % 8 != 0) ImGui::SameLine();
                ImGui::Text("R%02u: %08X", i, m_psx.getCPU().getCPUStatus().regs[i]);
            }
            ImGui::NewLine();

            ImGui::Text("COP0 Registers");
            ImGui::Separator();
            ImGui::Text("SR: %08X", m_psx.getCPU().getCOP0Status().SR);
        };
    }
private:
    std::span<const unsigned int> getScreenPixels() const override { return dummyPixelData; }

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
            }*/
            //if (ImGui::MenuItem("Reset")) gb.reset();
            if (ImGui::MenuItem("Exit", "Alt+F4")) stop();

            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Debug"))
        {
            ImGui::MenuItem("Debug Control&Status", nullptr, &m_debugView.open);
            ImGui::MenuItem("Memory", nullptr, &m_memoryView.open);

            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();

        m_debugView.updateWindow();
        m_memoryView.updateWindow();
    }

    PSX::Emulator& m_psx;
    imgui::DebugView m_debugView;
    imgui::MemoryView m_memoryView;

    unsigned int dummyPixelData[PSX::SCREEN_WIDTH * PSX::SCREEN_HEIGHT];
};

int main()
{
    std::unique_ptr<PSX::Emulator> psx = std::make_unique<PSX::Emulator>();
    PSXApp app{ *psx.get()};

    std::thread emuThread{
        [&]() {
            while (app.isRunning()) {
                std::this_thread::sleep_for(std::chrono::nanoseconds{ 20 }); // TODO: temp
                psx->clock();
            }
        }
    };

    app.run();
    emuThread.join();
    return 0;
}
