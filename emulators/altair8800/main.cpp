#include "altair.hpp"
#include "gui.hpp"
#include "emu_common/application.hpp"
#include "emu_common/graphics/font.hpp"
#include "emu_common/graphics/texture.hpp"
#include "emu_common/graphics/rect_shape.hpp"

#include <SDL.h>
#include <iostream>
#include <thread>

static constexpr int WINDOW_WIDTH = 1024;
static constexpr int WINDOW_HEIGHT = 500;
static constexpr int ALTAIR_OUTLINE_SIZE = 16;
static constexpr EmuCommon::Color ALTAIR_WHITE_COLOR{ 0xF2, 0xF1, 0xED };
static constexpr EmuCommon::Color ALTAIR_BLUE_COLOR{ 0x66, 0x9D, 0xD2 };
static constexpr EmuCommon::Color ALTAIR_GRAY_COLOR{ 0x6B, 0x69, 0x6A };

class Logo :
    public EmuCommon::Transformable,
    public EmuCommon::Renderable,
{
public:
    explicit Logo(const EmuCommon::SDLFont& font) :
        m_background{
            { float(WINDOW_WIDTH + 2 * ALTAIR_OUTLINE_SIZE), 72.f },
            ALTAIR_WHITE_COLOR
        },
        m_text1{ font, "ALTAIR 8800" },
        m_text2{ font, "COMPUTER" }
    {

    }
private:
    EmuCommon::RectShape m_background;
    EmuCommon::SDLText m_text1;
    EmuCommon::SDLText m_text2;
};

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
        m_background{
            { float(WINDOW_WIDTH - 2 * ALTAIR_OUTLINE_SIZE), float(WINDOW_HEIGHT - 2 * ALTAIR_OUTLINE_SIZE) },
            ALTAIR_GRAY_COLOR
        },
        m_logo{ m_256bytesFont },
        m_INTE{ "INTE", m_labelFont, m_ledTexture },
        m_PROT{ "PROT", m_labelFont, m_ledTexture },
        m_MEMR{ "MEMR", m_labelFont, m_ledTexture },
        m_INP{ "INP", m_labelFont, m_ledTexture },
        m_MI{ "MI", m_labelFont, m_ledTexture },
        m_OUT{ "OUT", m_labelFont, m_ledTexture },
        m_HLTA{ "HLTA", m_labelFont, m_ledTexture },
        m_STACK{ "STACK", m_labelFont, m_ledTexture },
        m_WO{ "WO", m_labelFont, m_ledTexture },
        m_INT{ "INT", m_labelFont, m_ledTexture },
        m_STATUSText{ m_labelFont, "STATUS" },
        m_D7{ "D7", m_labelFont, m_ledTexture },
        m_D6{ "D6", m_labelFont, m_ledTexture },
        m_D5{ "D5", m_labelFont, m_ledTexture },
        m_D4{ "D4", m_labelFont, m_ledTexture },
        m_D3{ "D3", m_labelFont, m_ledTexture },
        m_D2{ "D2", m_labelFont, m_ledTexture },
        m_D1{ "D1", m_labelFont, m_ledTexture },
        m_D0{ "D0", m_labelFont, m_ledTexture },
        m_A15{ "A15", m_labelFont, m_ledTexture },
        m_A14{ "A14", m_labelFont, m_ledTexture },
        m_A13{ "A13", m_labelFont, m_ledTexture },
        m_A12{ "A12", m_labelFont, m_ledTexture },
        m_A11{ "A11", m_labelFont, m_ledTexture },
        m_A10{ "A10", m_labelFont, m_ledTexture },
        m_A9{ "A9", m_labelFont, m_ledTexture },
        m_A8{ "A8", m_labelFont, m_ledTexture },
        m_A7{ "A7", m_labelFont, m_ledTexture },
        m_A6{ "A6", m_labelFont, m_ledTexture },
        m_A5{ "A5", m_labelFont, m_ledTexture },
        m_A4{ "A4", m_labelFont, m_ledTexture },
        m_A3{ "A3", m_labelFont, m_ledTexture },
        m_A2{ "A2", m_labelFont, m_ledTexture },
        m_A1{ "A1", m_labelFont, m_ledTexture },
        m_A0{ "A0", m_labelFont, m_ledTexture },
        m_stopRunBtn{ "STOP", "RUN", m_labelFont, m_switchTexture },
        m_stepBtn{ "SINGLE\nSTEP", "", m_labelFont, m_switchTexture },
        m_examineBtn{ "EXAMINE", "EXAMINE\nNEXT", m_labelFont, m_switchTexture },
        m_depositBtn{ "DEPOSIT", "DEPOSIT\nNEXT", m_labelFont, m_switchTexture },
        m_rstClrBtn{ "RESET", "CLR", m_labelFont, m_switchTexture },
        m_protectBtn{ "PROTECT", "UNPROTECT", m_labelFont, m_switchTexture }
    {
        m_background.setPosition({ float(ALTAIR_OUTLINE_SIZE), float(ALTAIR_OUTLINE_SIZE) });

        int maxWidth = m_INTE.getSize().x;
        for (const auto& i : { m_PROT, m_MEMR, m_INP, m_MI, m_OUT, m_HLTA, m_STACK, m_WO, m_INT })
            if (i.getSize().x > maxWidth) maxWidth = i.getSize().x;

        for (auto* i : { &m_INTE, &m_PROT, &m_MEMR, &m_INP, &m_MI, &m_OUT, &m_HLTA, &m_STACK, &m_WO, &m_INT }) {
            static int x = 56;
            int off = (maxWidth + 3 - i->getSize().x) / 2;
            i->setPosition({ x + off, 20 });
            x += maxWidth + 3;
        }
        m_STATUSText.setColor({ 0xF2, 0xF1, 0xED }); m_STATUSText.setPosition({ 287, 70 });

        int idx = 7;
        for (auto* i : { &m_D7, &m_D6, &m_D5, &m_D4, &m_D3, &m_D2, &m_D1, &m_D0 }) {
            static int x = 580;
            int off = (maxWidth + 3 - i->getSize().x) / 2;
            i->setPosition({ x + off, 20 });
            x += maxWidth + 3 + (idx % 3 == 0 ? 20 : 0);
            idx--;
        }

        idx = 15;
        for (auto* i : { &m_A15, &m_A14, &m_A13, &m_A12, &m_A11, &m_A10, &m_A9, &m_A8, &m_A7, &m_A6, &m_A5, &m_A4, &m_A3, &m_A2, &m_A1, &m_A0 }) {
            static int x = 184;
            int off = (maxWidth + 3 - i->getSize().x) / 2;
            i->setPosition({ x + off, 130 });
            x += maxWidth + 3 + (idx % 3 == 0 ? 20 : 0);
            idx--;
        }

        maxWidth = m_stopRunBtn.getWidth();
        for (const auto& i : { m_stepBtn, m_examineBtn, m_depositBtn, m_rstClrBtn, m_protectBtn })
            if (i.getWidth() > maxWidth) maxWidth = i.getWidth();

        for (auto* i : { &m_stopRunBtn, &m_stepBtn, &m_examineBtn, &m_depositBtn, &m_rstClrBtn, &m_protectBtn }) {
            static int x = 166;
            int off = (maxWidth - i->getWidth()) / 2;
            i->setPosition({ x + off, 310 });
            x += maxWidth + 7;
        }

        m_logo1.setTextSize(64); m_logo1.setColor({ 0, 0, 0 }); m_logo1.setPosition({ 16 + 64, 500 - 72 - 32 });
        m_logo2.setTextSize(46); m_logo2.setColor({ 0, 0, 0 }); m_logo2.setPosition({ m_logo1.getPosition().x + m_logo1.getSize().x + 16, m_logo1.getPosition().y + 10 });
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
        SDL_Rect rect;

        SDL_SetRenderDrawColor(renderer, ALTAIR_BLUE_COLOR.r, ALTAIR_BLUE_COLOR.g, ALTAIR_BLUE_COLOR.b, ALTAIR_BLUE_COLOR.a);
        SDL_RenderClear(renderer);
        m_background.render(renderer);

        rect = { 16, 500 - 72 - 32, 1024 - 32, 72 };
        SDL_SetRenderDrawColor(renderer, 0xF2, 0xF1, 0xED, 0xFF);
        SDL_RenderFillRect(renderer, &rect);

        m_INTE.render(renderer);
        m_PROT.render(renderer);
        m_MEMR.render(renderer);
        m_INP.render(renderer);
        m_MI.render(renderer);
        m_OUT.render(renderer);
        m_HLTA.render(renderer);
        m_STACK.render(renderer);
        m_WO.render(renderer);
        m_INT.render(renderer);

        m_D7.render(renderer);
        m_D6.render(renderer); 
        m_D5.render(renderer);
        m_D4.render(renderer);
        m_D3.render(renderer);
        m_D2.render(renderer);
        m_D1.render(renderer);
        m_D0.render(renderer);

        m_A15.render(renderer);
        m_A14.render(renderer);
        m_A13.render(renderer);
        m_A12.render(renderer);
        m_A11.render(renderer);
        m_A10.render(renderer);
        m_A9.render(renderer);
        m_A8.render(renderer);
        m_A7.render(renderer);
        m_A6.render(renderer);
        m_A5.render(renderer);
        m_A4.render(renderer);
        m_A3.render(renderer);
        m_A2.render(renderer);
        m_A1.render(renderer);
        m_A0.render(renderer);

        rect = { 142, 67, 333, 2 };
        SDL_SetRenderDrawColor(renderer, 0xF2, 0xF1, 0xED, 0xFF);
        SDL_RenderFillRect(renderer, &rect);
        m_STATUSText.render(renderer);

        float x = 188.f;
        for (int i = 0; i < 16; i++) {
            const float switchScale = 3.f;
            const int width = m_switchTexture.getSize().x / 3;
            const int height = m_switchTexture.getSize().y;
            const SDL_Rect srcRect{ 2 * width, 0, width, height };
            const SDL_FRect dstRect{ x, 220, width / switchScale, height / switchScale };
            SDL_RenderCopyF(renderer, m_switchTexture, &srcRect, &dstRect);
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

    LabeledLED m_INTE, m_PROT, m_MEMR, m_INP, m_MI,
               m_OUT, m_HLTA, m_STACK, m_WO, m_INT;
    EmuCommon::SDLText m_STATUSText;
    LabeledLED m_D7, m_D6, m_D5, m_D4, m_D3, m_D2, m_D1, m_D0;
    LabeledLED m_A15, m_A14, m_A13, m_A12, m_A11, m_A10, m_A9, m_A8, m_A7, m_A6, m_A5, m_A4, m_A3, m_A2, m_A1, m_A0;
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
