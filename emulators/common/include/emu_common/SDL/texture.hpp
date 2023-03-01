#pragma once
#include "emu_common/vec2.hpp"

struct SDL_Texture;

namespace EmuCommon {

	class SDLTexture
	{
	public:
		SDLTexture() = default;
		explicit SDLTexture(const char* filename) { loadFromFile(filename); }
		~SDLTexture();

		bool loadFromFile(const char* filename);

		Vec2u getSize() const { return m_size; }

		operator SDL_Texture*() { return m_handle; }
		SDL_Texture* getHandle() const { return m_handle; }
	private:
		SDL_Texture* m_handle = nullptr;
		Vec2u m_size;
	};

} // namespace EmuCommon
