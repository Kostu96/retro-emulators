#include "emu_common/graphics/font.hpp"

#include <SDL_ttf.h>
#include <cassert>

namespace EmuCommon {

    SDLFont::~SDLFont()
    {
        if (m_handle)
            TTF_CloseFont(m_handle);
    }

    bool SDLFont::loadFromFile(const char* filename)
    {
        assert(m_handle == nullptr);
        m_handle = TTF_OpenFont(filename, DEFAULT_SIZE);
        if (m_handle == nullptr)
            return false;

        TTF_SetFontHinting(m_handle, TTF_HINTING_LIGHT_SUBPIXEL);
        return true;
    }

    void SDLFont::setSize(int size) const
    {
        assert(m_handle != nullptr);
        [[maybe_unused]] int ret = TTF_SetFontSize(m_handle, size);
        assert(ret == 0);
    }

} // namespace EmuCommon
