#include "cpu8080.hpp"
#include "cartridge.hpp"
#include "ppu.hpp"
#include "../shared/memory_map.hpp"
#include "gui.hpp"
#include "timer.hpp"

#include <glad/gl.h>
#include <glw/glw.hpp>
#include <GLFW/glfw3.h>

#include <cassert>
#include <iostream>
#include <fstream>

constexpr u16 FRAME_WIDTH = 160;
constexpr u16 FRAME_HEIGHT = 144;
constexpr u16 SCALE = 3;
constexpr u16 BORDER_SIZE = 12;
constexpr u16 IMGUI_MENU_BAR_HEIGHT = 6;
constexpr u16 WINDOW_WIDTH = FRAME_WIDTH * SCALE + 2 * BORDER_SIZE;
constexpr u16 WINDOW_HEIGHT = FRAME_HEIGHT * SCALE + 2 * BORDER_SIZE;

static void glfwErrorCallback(int error, const char* description)
{
    std::cerr << "GLFW error " << error << ": " << description << '\n';
}

int main(int /*argc*/, char* argv[])
{
    glfwSetErrorCallback(glfwErrorCallback);
    if (!glfwInit()) {
        std::cerr << "GLFW init failed!\n";
        std::terminate();
    }

    GLFWwindow* window =
        glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT + IMGUI_MENU_BAR_HEIGHT, "Game Boy Emulator", nullptr, nullptr);
    if (!window) {
        std::cerr << "GLFW window creation failed!\n";
        std::terminate();
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(0);

    glw::init(glfwGetProcAddress);
    glw::Renderer::init();

    GUI::init(window);

    u8 bootloader[256];
    std::ifstream file{ argv[1], std::ios_base::binary};
    if (!file.is_open()) {
        std::cerr << "Cannot open bootloader file!\n";
        std::terminate();
    }
    file.read((char*)bootloader, 256);
    file.close();
    bootloader[0x0003] = 0x18;
    bootloader[0x0004] = 0x07;

    Cartridge cart;
    std::string path{ "C:/Users/Konstanty/Desktop/retro-extras/programs/gameboy/" };
    //std::string path{ "C:/Users/kmisiak/myplace/retro-extras/programs/gameboy/" };
    //path += "Blargg_tests/cpu_instrs.gb";
    //path += "Blargg_tests/01-special.gb";            // +
    path += "Blargg_tests/02-interrupts.gb";
    //path += "Blargg_tests/03-op sp,hl.gb";
    //path += "Blargg_tests/04-op r,imm.gb";
    //path += "Blargg_tests/05-op rp.gb";
    //path += "Blargg_tests/06-ld r,r.gb";             // +
    //path += "Blargg_tests/07-jr,jp,call,ret,rst.gb"; // +
    //path += "Blargg_tests/08-misc instrs.gb";        // +
    //path += "Blargg_tests/09-op r,r.gb";
    //path += "Blargg_tests/10-bit ops.gb";            // +
    //path += "Blargg_tests/11-op a,(hl).gb";
    //path += "Blargg_tests/instr_timing.gb";
    //path += "dmg-acid2.gb";
    //path += "tetris.gb";
    cart.loadFromFile(path.c_str());

    u8 wram[0x2000];
    u8 hram[0x80];

    u8 serial[2];

    u8 InterruptFlag = 0;
    u8 unmapBootloader = 0;
    u8 InterruptEnable = 0;

    CPU8080 cpu{ CPU8080::Mode::GameBoy };
    Timer timer{ InterruptFlag };
    PPU ppu;

    const AddressRange CART_RANGE{   0x0000, 0x7FFF };
    const AddressRange VRAM_RANGE{   0x8000, 0x9FFF };
    const AddressRange WRAM_RANGE{   0xC000, 0xDFFF };
    const AddressRange OAM_RANGE{    0xFE00, 0xFE9F };
    const AddressRange SERIAL_RANGE{ 0xFF01, 0xFF02 };
    const AddressRange TIMERS_RANGE{ 0xFF04, 0xFF07 };
    const AddressRange APU_RANGE{    0xFF10, 0xFF26 };
    const AddressRange PPU_RANGE{    0xFF40, 0xFF4B };
    const AddressRange HRAM_RANGE{   0xFF80, 0xFFFF };

    bool mapBootloader = true;

    cpu.mapReadMemoryCallback([&](u16 address)
        {
            u16 offset;
            if (CART_RANGE.contains(address, offset))
            {
                if (mapBootloader && offset < 0x100) return bootloader[offset];
                return cart.load8(offset);
            }

            if (WRAM_RANGE.contains(address, offset))
                return wram[offset];

            if (PPU_RANGE.contains(address, offset))
                return ppu.load8(offset);
            
            if (HRAM_RANGE.contains(address, offset))
                return hram[offset];

            if (address == 0xFF0F) return InterruptFlag;
            if (address == 0xFF50) return unmapBootloader;
            if (address == 0xFFFF) return InterruptEnable;

            assert(false && "Unhandled read");
            return u8{};
        });

    cpu.mapWriteMemoryCallback([&](u16 address, u8 data)
        {
            u16 offset;
            if (CART_RANGE.contains(address, offset)) {
                cart.store8(offset, data);
                return;
            }

            if (VRAM_RANGE.contains(address, offset)) {
                ppu.storeVRAM8(offset, data);
                return;
            }

            if (WRAM_RANGE.contains(address, offset)) {
                wram[offset] = data;
                return;
            }

            if (OAM_RANGE.contains(address, offset)) {
                // TODO: sprites
                return;
            }

            if (SERIAL_RANGE.contains(address, offset)) {
                serial[offset] = data;
                if (offset == 1 && data == 0x81)
                {
                    std::cout << serial[0];
                    serial[1] = 0;
                }
                return;
            }

            if (TIMERS_RANGE.contains(address, offset)) {
                timer.store8(offset, data);
                return;
            }

            if (APU_RANGE.contains(address, offset)) {
                // TODO: sound
                return;
            }

            if (PPU_RANGE.contains(address, offset)) {
                ppu.store8(offset, data);
                return;
            }

            if (HRAM_RANGE.contains(address, offset)) {
                hram[offset] = data;
                return;
            }

            if (address == 0xFF0F) {
                InterruptFlag = data & 0xE;
                return;
            }

            if (address == 0xFFFF) {
                InterruptEnable = data & 0xE;
            }

            if (address == 0xFF50) {
                if (mapBootloader) mapBootloader = false;
                return;
            }

            assert(false && "Unhandled write");
        });

    cpu.reset();
    timer.reset();
    ppu.reset();

    glClearColor(0.f, 0.f, 0.f, 0.f);
    while (!glfwWindowShouldClose(window))
    {
        int repeats = 128;
        while (repeats--)
        {
            // bootloader routine to clear the VRAM
            if (mapBootloader && cpu.getPC() == 0x0003) ppu.clearVRAM();

            cpu.clock();
            timer.clock();
            //ppu.clock();
        }

        glClear(GL_COLOR_BUFFER_BIT);

        /*glw::Renderer::beginFrame();
        glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
        tileMap0FBO->getAttachments()[0].bind(0);
        float left = (float)BORDER_SIZE / (WINDOW_WIDTH * 0.5f) - 1.f;
        float right = (float)(BORDER_SIZE + FRAME_WIDTH * SCALE) / (WINDOW_WIDTH * 0.5f) - 1.f;
        float top = -((float)BORDER_SIZE / (WINDOW_HEIGHT * 0.5f) - 1.f);
        float bottom = -((float)(BORDER_SIZE + FRAME_HEIGHT * SCALE) / (WINDOW_HEIGHT * 0.5f) - 1.f);
        float u0 = (float)ppu.getSCX() / 256.f;
        float v0 = 1.f - (float)ppu.getSCY() / 256.f;
        float u1 = (float)(ppu.getSCX() + 160) / 256.f;
        float v1 = 1.f - (float)(ppu.getSCY() + 144) / 256.f;
        glw::Renderer::renderTexture(left, top, right, bottom, u0, v0, u1, v1);
        glw::Renderer::endFrame();*/

        //GUI::update(ppu);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    GUI::shutdown();
    glw::Renderer::shutdown();
    glfwTerminate();
    return 0;
}
