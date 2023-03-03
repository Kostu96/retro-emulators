#pragma once
#include "emu_common/vec2.hpp"

struct SDL_Texture;
struct SDL_Surface;

namespace EmuCommon {

	class SDLText;

	class SDLTexture
	{
	public:
		SDLTexture() = default;
		explicit SDLTexture(const char* filename) { loadFromFile(filename); }
		~SDLTexture();

		bool loadFromFile(const char* filename);

		Vec2u getSize() const { return m_size; }

		SDL_Texture* getHandle() const { return m_handle; }
	private:
		bool createFromSurface(SDL_Surface* surface);

		SDL_Texture* m_handle = nullptr;
		Vec2u m_size;

		friend class SDLText;
	};

} // namespace EmuCommon
