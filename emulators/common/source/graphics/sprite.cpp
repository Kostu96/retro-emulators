#include "emu_common/graphics/sprite.hpp"
#include "emu_common/graphics/texture.hpp"

#include <SDL.h>

namespace EmuCommon {

	void SDLSprite::setTexture(const SDLTexture& texture, bool resetRect)
	{
		m_texture = &texture;
		if (resetRect)
			m_textureRect = { 0, 0, (int)texture.getSize().x, (int)texture.getSize().y };
	}

	void SDLSprite::render(SDL_Renderer* renderer, Transform transform)
	{
		transform *= getTransform();
		FRect rect = transform.tranformRect({ 0, 0, (float)m_textureRect.width, (float)m_textureRect.height });

		SDL_RenderCopyF(renderer, m_texture->getHandle(),
			reinterpret_cast<SDL_Rect*>(&m_textureRect), reinterpret_cast<SDL_FRect*>(&rect));
	}

} // EmuCommon