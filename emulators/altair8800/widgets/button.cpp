#include "button.hpp"
#include "emu_common/application.hpp"

void Button::render()
{
    auto renderer = EmuCommon::Application::get().getSDLRenderer();

    m_text.render(renderer);
}

void Button::onEvent(SDL_Event& /*e*/)
{

}
