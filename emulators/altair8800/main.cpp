#include "altair.hpp"
#include "constants.hpp"
#include "gui.hpp"
#include "entities/led_group.hpp"
#include "entities/logo.hpp"

#include "emu_common/application.hpp"
#include "emu_common/graphics/font.hpp"
#include "emu_common/graphics/texture.hpp"

#include <SDL.h>
#include <iostream>
#include <thread>

static const char* s_StatusLabels[8] = { "MEMR", "INP", "MI", "OUT", "HLTA", "STACK", "WO", "INT" };
static const char* s_DataLabels[8] = { "D7", "D6", "D5", "D4", "D3", "D2", "D1", "D0" };
static const char* s_AddressLabels[16] = { "A15", "A14", "A13", "A12", "A11", "A10", "A9", "A8",
                                           "A7", "A6", "A5", "A4", "A3", "A2", "A1", "A0" };

class Emulator :
    public EmuCommon::Application
{
public:
    Emulator(Altair& altair) :
        Application{ "Altair 8800 Emulator by Kostu96", WINDOW_WIDTH, WINDOW_HEIGHT },
        m_altair{ altair },
        m_labelFont{ "assets/altair/coolvetica.ttf" },
        m_256bytesFont{ "assets/altair/256bytes.ttf" },
        m_ledTexture{ "assets/altair/led.png" },
        m_switchTexture{ "assets/altair/switch.png" },
        m_background{ { float(WINDOW_WIDTH - 2 * ALTAIR_OUTLINE_SIZE), float(WINDOW_HEIGHT - 2 * ALTAIR_OUTLINE_SIZE) }, ALTAIR_GRAY_COLOR },
        m_logo{ m_256bytesFont },
        m_INTE{ "INTE", m_labelFont, m_ledTexture },
        m_PROT{ "PROT", m_labelFont, m_ledTexture },
        m_statusLEDs{ m_labelFont, m_ledTexture, 8, s_StatusLabels, 44.f },
        m_STATUSText{ m_labelFont, "STATUS" },
        m_dataLEDs{ m_labelFont, m_ledTexture, 8, s_DataLabels, 44.f, 16.f },
        m_addressLEDs{ m_labelFont, m_ledTexture, 16, s_AddressLabels, 44.f, 16.f },
        m_stopRunBtn{ "STOP", "RUN", m_labelFont, m_switchTexture },
        m_stepBtn{ "SINGLE\nSTEP", "", m_labelFont, m_switchTexture },
        m_examineBtn{ "EXAMINE", "EXAMINE\nNEXT", m_labelFont, m_switchTexture },
        m_depositBtn{ "DEPOSIT", "DEPOSIT\nNEXT", m_labelFont, m_switchTexture },
        m_rstClrBtn{ "RESET", "CLR", m_labelFont, m_switchTexture },
        m_protectBtn{ "PROTECT", "UNPROTECT", m_labelFont, m_switchTexture }
    {
        m_background.setPosition({ float(ALTAIR_OUTLINE_SIZE), float(ALTAIR_OUTLINE_SIZE) });
        m_logo.setPosition({ float(ALTAIR_OUTLINE_SIZE), float(WINDOW_HEIGHT) - m_logo.getSize().y - 30 });

        m_statusLEDs.setPosition({ 64, 20 });
        m_STATUSText.setColor({ 0xF2, 0xF1, 0xED });
        m_STATUSText.setOrigin({ m_STATUSText.getSize().x / 2.f, 0 });
        m_STATUSText.setPosition({ m_statusLEDs.getPosition().x + m_statusLEDs.getSize().x / 2.f,
            m_statusLEDs.getPosition().y + m_statusLEDs.getSize().y + 4 });

        m_dataLEDs.setPosition({ 500, 20 });
        m_addressLEDs.setPosition({ 100, 100 });

        unsigned int maxWidth = m_INTE.getSize().x;
        for (const auto& i : { m_PROT })
            if (i.getSize().x > maxWidth) maxWidth = i.getSize().x;
        
        for (auto* i : { &m_INTE, &m_PROT }) {
            static unsigned int x = 56;
            unsigned int off = (maxWidth + 3 - i->getSize().x) / 2;
            i->setPosition({ x + off, 20 });
            x += maxWidth + 3;
        }

        maxWidth = m_stopRunBtn.getWidth();
        for (const auto& i : { m_stepBtn, m_examineBtn, m_depositBtn, m_rstClrBtn, m_protectBtn })
            if (unsigned int(i.getWidth()) > maxWidth) maxWidth = i.getWidth();

        for (auto* i : { &m_stopRunBtn, &m_stepBtn, &m_examineBtn, &m_depositBtn, &m_rstClrBtn, &m_protectBtn }) {
            static int x = 166;
            int off = (maxWidth - i->getWidth()) / 2;
            i->setPosition({ x + off, 310 });
            x += maxWidth + 7;
        }
    }
protected:
    void onUpdate() override {
        Vec2i mousePos;;
        SDL_GetMouseState(&mousePos.x, &mousePos.y);
        m_stopRunBtn.handleMousePos(mousePos);
        m_stepBtn.handleMousePos(mousePos);
        m_examineBtn.handleMousePos(mousePos);
        m_depositBtn.handleMousePos(mousePos);
        m_rstClrBtn.handleMousePos(mousePos);
        m_protectBtn.handleMousePos(mousePos);

        auto renderer = getSDLRenderer();

        SDL_SetRenderDrawColor(renderer, ALTAIR_BLUE_COLOR.r, ALTAIR_BLUE_COLOR.g, ALTAIR_BLUE_COLOR.b, ALTAIR_BLUE_COLOR.a);
        SDL_RenderClear(renderer);
        m_background.render(renderer);
        m_logo.render(renderer);

        m_statusLEDs.render(renderer);

        // TODO: chnage to RectShape
        SDL_FRect rect = { m_statusLEDs.getPosition().x - 5, m_statusLEDs.getPosition().y + m_statusLEDs.getSize().y + 4,
            m_statusLEDs.getSize().x + 10, 2};
        SDL_SetRenderDrawColor(renderer, 0xF2, 0xF1, 0xED, 0xFF);
        SDL_RenderFillRectF(renderer, &rect);
        m_STATUSText.render(renderer);

        m_dataLEDs.render(renderer);
        m_addressLEDs.render(renderer);

        float x = 188.f;
        for (int i = 0; i < 16; i++) {
            const float switchScale = 3.f;
            const int width = m_switchTexture.getSize().x / 3;
            const int height = m_switchTexture.getSize().y;
            const SDL_Rect srcRect{ 2 * width, 0, width, height };
            const SDL_FRect dstRect{ x, 220, width / switchScale, height / switchScale };
            SDL_RenderCopyF(renderer, m_switchTexture.getHandle(), &srcRect, &dstRect);
            x += 42.f + (i % 3 == 0 ? 20.f : 0);
        }

        m_stopRunBtn.render(renderer);
        m_stepBtn.render(renderer);
        m_examineBtn.render(renderer);
        m_depositBtn.render(renderer);
        m_rstClrBtn.render(renderer);
        m_protectBtn.render(renderer);
    }

    void onEvent(SDL_Event& e) override {
        m_stopRunBtn.onEvent(e);
        m_stepBtn.onEvent(e);
        m_examineBtn.onEvent(e);
        m_depositBtn.onEvent(e);
        m_rstClrBtn.onEvent(e);
        m_protectBtn.onEvent(e);
    }
private:
    Altair& m_altair;
    EmuCommon::SDLFont m_labelFont;
    EmuCommon::SDLFont m_256bytesFont;
    EmuCommon::SDLTexture m_ledTexture;
    EmuCommon::SDLTexture m_switchTexture;

    EmuCommon::RectShape m_background;
    Logo m_logo;
    LabeledLED m_INTE, m_PROT;
    LEDGroup m_statusLEDs;
    EmuCommon::SDLText m_STATUSText;
    LEDGroup m_dataLEDs;
    LEDGroup m_addressLEDs;

    TwoWayButton m_stopRunBtn;
    TwoWayButton m_stepBtn;
    TwoWayButton m_examineBtn;
    TwoWayButton m_depositBtn;
    TwoWayButton m_rstClrBtn;
    TwoWayButton m_protectBtn;
};

int main(int /*argc*/, char* /*argv*/[])
{
    Altair altair;
    Emulator emulator{ altair };

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
