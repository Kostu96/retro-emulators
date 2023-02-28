#include "altair.hpp"
#include "emu_common/application.hpp"
#include "emu_common/sdl_helpers.hpp"

#include <SDL.h>

#include <iostream>
#include <thread>

using EmuCommon::Vec2i;

class LabeledLED
{
public:
    LabeledLED(const char* text, EmuCommon::SDLFont& font, EmuCommon::SDLTexture& ledTexture) :
        m_label{ font },
        m_LEDTexture{ ledTexture }
    {
        m_label.setText(text);
        m_label.setTextSize(14);
        m_label.setColor({ 0xF2, 0xF1, 0xED });

        m_size.x = std::max(m_label.getSize().x, (m_LEDTexture.getSize().x / 2) / LED_SCALE);
        m_size.y = m_label.getSize().y + m_LEDTexture.getSize().y / LED_SCALE + 2;
    }

    void setPosition(Vec2i position)
    {
        m_position = position;
        int xPos = position.x + ((m_size.x == m_label.getSize().x) ? 0 : (m_size.x - m_label.getSize().x) / 2);
        m_label.setPosition({ xPos, position.y });
    }

    Vec2i getSize() const { return m_size; }

    void render(SDL_Renderer* renderer)
    {
        m_label.render(renderer);
        const int width = m_LEDTexture.getSize().x / 2;
        const int scaledWidth = width / LED_SCALE;
        const int height = m_LEDTexture.getSize().y;
        const SDL_Rect srcRect{ m_status ? width : 0, 0, width, height };
        int xPos = m_position.x + ((m_size.x == scaledWidth) ? 0 : (m_size.x - scaledWidth) / 2);
        const SDL_Rect dstRect{ xPos, m_position.y + m_label.getSize().y + 2, scaledWidth, height / LED_SCALE };
        SDL_RenderCopy(renderer, m_LEDTexture, &srcRect, &dstRect);
    }
private:
    static constexpr int LED_SCALE = 4;

    bool m_status = false;
    Vec2i m_position;
    Vec2i m_size;
    EmuCommon::SDLText m_label;
    EmuCommon::SDLTexture& m_LEDTexture;
};

class Emulator :
    public EmuCommon::Application
{
public:
    Emulator() :
        Application{ "Altair 8800 Emulator by Kostu96", 1024, 400 },
        m_labelFont{ "assets/altair/coolvetica.ttf" },
        m_ledTexture{ "assets/altair/led.png" },
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
        m_STATUSText{ m_labelFont },
        m_logo1{ m_256bytesFont },
        m_logo2{ m_256bytesFont }
    {
        int maxWidth = m_INTE.getSize().x;
        for (const auto& i : { m_PROT, m_MEMR, m_INP, m_MI, m_OUT, m_HLTA, m_STACK, m_WO, m_INT })
            if (i.getSize().x > maxWidth) maxWidth = i.getSize().x;

        for (auto* i : { &m_INTE, &m_PROT, &m_MEMR, &m_INP, &m_MI, &m_OUT, &m_HLTA, &m_STACK, &m_WO, &m_INT }) {
            static int x = 64;
            int off = (maxWidth + 3 - i->getSize().x) / 2;
            i->setPosition({ x + off, 20 });
            x += maxWidth + 3;
        }

        m_256bytesFont.loadFromFile("assets/altair/256bytes.ttf");

        m_STATUSText.setText("STATUS"); m_STATUSText.setTextSize(14); m_STATUSText.setColor({ 0xF2, 0xF1, 0xED }); m_STATUSText.setPosition({ 295, 70 });

        m_logo1.setText("ALTAIR 8800"); m_logo1.setTextSize(64); m_logo1.setColor({ 0, 0, 0 });
        m_logo1.setPosition({ 16 + 64, 400 - 72 - 32 });
        m_logo2.setText("COMPUTER"); m_logo2.setTextSize(46); m_logo2.setColor({ 0, 0, 0 });
        m_logo2.setPosition({ m_logo1.getPosition().x + m_logo1.getSize().x + 16, m_logo1.getPosition().y + 10 });

        m_switchTexture.loadFromFile("assets/altair/switch.png");
    }
protected:
    void onUpdate() override {
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
        m_logo1.render(renderer);
        m_logo2.render(renderer);

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

        rect = { 145, 67, 337, 2 };
        SDL_SetRenderDrawColor(renderer, 0xF2, 0xF1, 0xED, 0xFF);
        SDL_RenderFillRect(renderer, &rect);
        m_STATUSText.render(renderer);

        // switches
        for (int i = 0; i < 16; i++) {
            const float switchScale = 3.f;
            const int width = m_switchTexture.getSize().x / 3;
            const int height = m_switchTexture.getSize().y;
            const SDL_Rect srcRect{ (i % 3) * width, 0, width, height };
            const SDL_FRect dstRect{ (10.f + width / switchScale) * i + 50.f, 200, width / switchScale, height / switchScale };
            SDL_RenderCopyF(renderer, m_switchTexture, &srcRect, &dstRect);
        }     
    }
private:
    EmuCommon::SDLFont m_labelFont;
    EmuCommon::SDLFont m_256bytesFont;
    EmuCommon::SDLTexture m_ledTexture;
    EmuCommon::SDLTexture m_switchTexture;
    LabeledLED m_INTE, m_PROT, m_MEMR, m_INP, m_MI,
               m_OUT, m_HLTA, m_STACK, m_WO, m_INT;
    EmuCommon::SDLText m_STATUSText;
    EmuCommon::SDLText m_logo1;
    EmuCommon::SDLText m_logo2;
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
