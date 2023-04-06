#include "emu_common/graphics/sprite.hpp"
#include "emu_common/graphics/texture.hpp"

#include <SDL.h>

namespace EmuCommon {

	SDLSprite::SDLSprite(const SDLTexture& texture) :
		m_texture{ &texture },
		m_textureRect{ 0, 0, (int)texture.getSize().x, (int)texture.getSize().y } {}

	SDLSprite::SDLSprite(const SDLTexture& texture, const IRect& rect) :
		m_texture{ &texture },
		m_textureRect{ rect } {}

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
		const SDL_Rect iRect{ (int)rect.x, (int)rect.y, (int)rect.width, (int)rect.height };
		SDL_RenderCopy(renderer, m_texture->getHandle(), reinterpret_cast<SDL_Rect*>(&m_textureRect), &iRect);
	}

} // EmuCommon
