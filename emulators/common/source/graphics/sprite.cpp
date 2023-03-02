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

	void SDLSprite::render(SDL_Renderer* renderer, const RenderStates& states)
	{
		const SDL_Rect* srcRect = (const SDL_Rect*)&m_textureRect;
		const SDL_FRect dstRect{
			getPosition().x + states.position.x,
			getPosition().y + states.position.y,
			(float)m_textureRect.width * getScale().x * states.scale.x,
			(float)m_textureRect.height * getScale().y * states.scale.y
		};
		SDL_RenderCopyF(renderer, m_texture->getHandle(), srcRect, &dstRect);
	}

} // EmuCommon
