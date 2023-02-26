#pragma once

struct SDL_Texture;

namespace EmuCommon {

	class SDLTexture
	{
	public:
		SDLTexture() = default;
		~SDLTexture();

		bool loadFromFile(const char* filename);

		operator SDL_Texture*() { return m_handle; }
	private:
		SDL_Texture* m_handle = nullptr;
	};

} // namespace EmuCommon
