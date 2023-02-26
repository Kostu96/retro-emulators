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
        Application{ "Altair 8800 Emulator by Kostu96", 1024, 400 },
        m_INTEText{ m_labelFont },
        m_PROTText{ m_labelFont },
        m_MEMRText{ m_labelFont },
        m_INPText{ m_labelFont },
        m_MIText{ m_labelFont },
        m_OUTText{ m_labelFont },
        m_HLTAText{ m_labelFont },
        m_STACKText{ m_labelFont },
        m_WOText{ m_labelFont },
        m_INTText{ m_labelFont },
        m_STATUSText{ m_labelFont },
        m_logo1{ m_256bytesFont64 },
        m_logo2{ m_256bytesFont46 }
    {
        m_labelFont.loadFromFile("assets/altair/coolvetica.ttf", 14);
        m_256bytesFont64.loadFromFile("assets/altair/256bytes.ttf", 64);
        m_256bytesFont46.loadFromFile("assets/altair/256bytes.ttf", 46);

        m_INTEText.setText("INTE", 0xF2, 0xF1, 0xED);
        m_PROTText.setText("PROT", 0xF2, 0xF1, 0xED);
        m_MEMRText.setText("MEMR", 0xF2, 0xF1, 0xED);
        m_INPText.setText("INP", 0xF2, 0xF1, 0xED);
        m_MIText.setText("MI", 0xF2, 0xF1, 0xED);
        m_OUTText.setText("OUT", 0xF2, 0xF1, 0xED);
        m_HLTAText.setText("HLTA", 0xF2, 0xF1, 0xED);
        m_STACKText.setText("STACK", 0xF2, 0xF1, 0xED);
        m_WOText.setText("WO", 0xF2, 0xF1, 0xED);
        m_INTText.setText("INT", 0xF2, 0xF1, 0xED);
        m_STATUSText.setText("STATUS", 0xF2, 0xF1, 0xED);

        m_logo1.setText("ALTAIR 8800", 0, 0, 0);
        m_logo2.setText("COMPUTER", 0, 0, 0);

        m_ledTexture.loadFromFile("assets/altair/led.png");
        m_switchTexture.loadFromFile("assets/altair/switch.png");
    }
protected:
    void onRender() override {
        auto renderer = getSDLRenderer();
        SDL_Rect rect;

        // BG and LOGO
        SDL_SetRenderDrawColor(renderer, 0x66, 0x9D, 0xD2, 0xFF);
        SDL_RenderClear(renderer);
        rect = { 16, 16, 1024 - 32, 400 - 32 };
        SDL_SetRenderDrawColor(renderer, 0x6b, 0x69, 0x6A, 0xFF);
        SDL_RenderFillRect(renderer, &rect);
        rect = { 16, 400 - 72 - 32, 1024 - 32, 72 };
        SDL_SetRenderDrawColor(renderer, 0xF2, 0xF1, 0xED, 0xFF);
        SDL_RenderFillRect(renderer, &rect);

        rect = { 16 + 64, 400 - 72 - 32, m_logo1.getWidth(), m_logo1.getHeight() };
        SDL_RenderCopy(renderer, m_logo1.getTexture(), nullptr, &rect);
        rect = { 16 + 64 + 16 + m_logo1.getWidth(), 400 - 72 - 32 + 10, m_logo2.getWidth(), m_logo2.getHeight() };
        SDL_RenderCopy(renderer, m_logo2.getTexture(), nullptr, &rect);

        // 1st row of LEDs
        const int firstRowLevel = 20;
        for (int i = 0; i < 10; i++) {
            const float ledScale = 4.f;
            const int width = m_ledTexture.getWidth() / 2;
            const int height = m_ledTexture.getHeight();
            const SDL_Rect srcRect{ (i % 2) * width, 0, width, height };
            const SDL_FRect dstRect{
                64.f + (18.f + width / ledScale) * i,
                firstRowLevel + height / ledScale - 4.f,
                width / ledScale,
                height / ledScale
            };
            SDL_RenderCopyF(renderer, m_ledTexture, &srcRect, &dstRect);
        }
        rect = { 64, firstRowLevel, m_INTEText.getWidth(), m_INTEText.getHeight() };
        SDL_RenderCopy(renderer, m_INTEText.getTexture(), nullptr, &rect);
        rect = { 105, firstRowLevel, m_PROTText.getWidth(), m_PROTText.getHeight() };
        SDL_RenderCopy(renderer, m_PROTText.getTexture(), nullptr, &rect);
        rect = { 145, firstRowLevel, m_MEMRText.getWidth(), m_MEMRText.getHeight() };
        SDL_RenderCopy(renderer, m_MEMRText.getTexture(), nullptr, &rect);
        rect = { 198, firstRowLevel, m_INPText.getWidth(), m_INPText.getHeight() };
        SDL_RenderCopy(renderer, m_INPText.getTexture(), nullptr, &rect);
        rect = { 242, firstRowLevel, m_MIText.getWidth(), m_MIText.getHeight() };
        SDL_RenderCopy(renderer, m_MIText.getTexture(), nullptr, &rect);
        rect = { 279, firstRowLevel, m_OUTText.getWidth(), m_OUTText.getHeight() };
        SDL_RenderCopy(renderer, m_OUTText.getTexture(), nullptr, &rect);
        rect = { 321, firstRowLevel, m_HLTAText.getWidth(), m_HLTAText.getHeight() };
        SDL_RenderCopy(renderer, m_HLTAText.getTexture(), nullptr, &rect);
        rect = { 360, firstRowLevel, m_STACKText.getWidth(), m_STACKText.getHeight() };
        SDL_RenderCopy(renderer, m_STACKText.getTexture(), nullptr, &rect);
        rect = { 410, firstRowLevel, m_WOText.getWidth(), m_WOText.getHeight() };
        SDL_RenderCopy(renderer, m_WOText.getTexture(), nullptr, &rect);
        rect = { 456, firstRowLevel, m_INTText.getWidth(), m_INTText.getHeight() };
        SDL_RenderCopy(renderer, m_INTText.getTexture(), nullptr, &rect);

        rect = { 144, firstRowLevel + 50, 338, 2 };
        SDL_SetRenderDrawColor(renderer, 0xF2, 0xF1, 0xED, 0xFF);
        SDL_RenderFillRect(renderer, &rect);
        rect = { 295, firstRowLevel + 51, m_STATUSText.getWidth(), m_STATUSText.getHeight() };
        SDL_RenderCopy(renderer, m_STATUSText.getTexture(), nullptr, &rect);

        // switches
        for (int i = 0; i < 16; i++) {
            const float switchScale = 3.f;
            const int width = m_switchTexture.getWidth() / 3;
            const int height = m_switchTexture.getHeight();
            const SDL_Rect srcRect{ (i % 3) * width, 0, width, height };
            const SDL_FRect dstRect{ (10.f + width / switchScale) * i + 50.f, 200, width / switchScale, height / switchScale };
            SDL_RenderCopyF(renderer, m_switchTexture, &srcRect, &dstRect);
        }     
    }
private:
    EmuCommon::SDLFont m_labelFont;
    EmuCommon::SDLFont m_256bytesFont64;
    EmuCommon::SDLFont m_256bytesFont46;
    EmuCommon::SDLText m_INTEText;
    EmuCommon::SDLText m_PROTText;
    EmuCommon::SDLText m_MEMRText;
    EmuCommon::SDLText m_INPText;
    EmuCommon::SDLText m_MIText;
    EmuCommon::SDLText m_OUTText;
    EmuCommon::SDLText m_HLTAText;
    EmuCommon::SDLText m_STACKText;
    EmuCommon::SDLText m_WOText;
    EmuCommon::SDLText m_INTText;
    EmuCommon::SDLText m_STATUSText;
    EmuCommon::SDLText m_logo1;
    EmuCommon::SDLText m_logo2;
    EmuCommon::SDLTexture m_ledTexture;
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
