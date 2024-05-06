#include "psx.hpp"

#include "shared/source/application.hpp"
#include "shared/source/disassembly_line.hpp"
#include "shared/source/imgui/debug_view.hpp"
#include "shared/source/imgui/disassembly_view.hpp"
#include "shared/source/imgui/memory_view.hpp"

#include <imgui.h>
#include <thread>

class PSXApp :
    public Application
{
public:
    PSXApp(PSX::Emulator& psx, Disassembly& disassembly) :
        Application{ {
                .windowTitle = "PSX Emulator by Kostu96",
                .rendererWidth = PSX::SCREEN_WIDTH,
                .rendererHeight = PSX::SCREEN_HEIGHT,
                .scale = 1,
                .border = 10,
                .hasMenuBar = true
        } },
        m_psx{ psx },
        m_disassembly{ disassembly },
        m_disasmView{ m_disassembly },
        dummyPixelData{ new unsigned int[PSX::SCREEN_WIDTH * PSX::SCREEN_HEIGHT] }
    {
        m_debugView.cpuStepCallback = [&]() {
            m_psx.clock();
        };

        m_debugView.cpuStatusCallback = [&]() {
            const auto& cpuStatus = m_psx.getCPU().getCPUStatus();
            ImGui::Text("CPU Registers");
            ImGui::Separator();
            ImGui::Text("PC: %08X", cpuStatus.PC); ImGui::SameLine();
            ImGui::Text("HI: %08X", cpuStatus.HI); ImGui::SameLine();
            ImGui::Text("LO: %08X", cpuStatus.LO);
            for (auto i = 0; i < 32; i++) {
                if (i % 4 != 0) ImGui::SameLine();
                ImGui::Text("R%02u: %08X", i, cpuStatus.regs[i]);
            }
            ImGui::NewLine();

            ImGui::Text("COP0 Registers");
            ImGui::Separator();
            ImGui::Text("SR: %08X", m_psx.getCPU().getCOP0Status().SR);
        };
    }
private:
    std::span<const unsigned int> getScreenPixels() const override { return { dummyPixelData.get(), PSX::SCREEN_WIDTH * PSX::SCREEN_HEIGHT}; }

    void onImGUIRender() override {
        ImGui::BeginMainMenuBar();
        if (ImGui::BeginMenu("Emulator"))
        {
            ImGui::MenuItem("Start");
            if (ImGui::MenuItem("Reset")) m_psx.reset();
            if (ImGui::MenuItem("Exit", "Alt+F4")) exit();
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Settings"))
        {
            ImGui::MenuItem("Autostart emulation", nullptr, &m_autostart);
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Debug"))
        {
            ImGui::MenuItem("Debug Control&Status", nullptr, &m_debugView.open);
            ImGui::MenuItem("Disassembly", nullptr, &m_disasmView.open);
            ImGui::MenuItem("Memory", nullptr, &m_memoryView.open);
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();

        m_debugView.updateWindow();
        m_disasmView.updateWindow(m_psx.getCPU().getCPUStatus().PC);
        m_memoryView.updateWindow();
    }

    PSX::Emulator& m_psx;
    Disassembly& m_disassembly;
    imgui::DebugView m_debugView;
    imgui::DisassemblyView m_disasmView;
    imgui::MemoryView m_memoryView;
    bool m_autostart = false;

    std::unique_ptr<unsigned int> dummyPixelData;
};

int main()
{
    Disassembly disassembly;
    std::unique_ptr<PSX::Emulator> psx = std::make_unique<PSX::Emulator>(disassembly);
    PSXApp app{ *psx.get(), disassembly }; // TODO: toooo much spagetti

    std::thread emuThread{
        [&]() {
            while (app.isRunning()) {
                std::this_thread::sleep_for(std::chrono::nanoseconds{ 32 }); // TODO: temp
                psx->clock();
            }
        }
    };
    
    app.run();
    emuThread.join();
    return 0;
}
