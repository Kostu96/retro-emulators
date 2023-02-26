#include "emu_common/application.hpp"

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include <cassert>
#include <iostream>

namespace EmuCommon {

	Application::Application(const char* title, int width, int height)
	{
		assert(s_instance == nullptr);
		s_instance = this;

		if (SDL_Init(SDL_INIT_VIDEO) < 0) {
			std::cerr << "Could not initialize SDL.\n";
			abort();
		}

		m_window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
		if (!m_window) {
			std::cerr << "Could not create window.\n";
			abort();
		}

		m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);
		if (!m_renderer) {
			std::cerr << "Could not create renderer.\n";
			abort();
		}
		SDL_RenderSetVSync(m_renderer, 1);
		
		const int IMG_FLAGS = IMG_INIT_PNG;
		if ((IMG_Init(IMG_FLAGS) & IMG_FLAGS) != IMG_FLAGS) {
			std::cerr << "Could not initialize SDL_image.\n";
			abort();
		}

		if (TTF_Init() == -1) {
			std::cerr << "Could not initialize SDL_ttf.\n";
			abort();
		}

		m_isRunning = true;
	}

	Application::~Application()
	{
		SDL_DestroyRenderer(m_renderer);
		SDL_DestroyWindow(m_window);
		SDL_Quit();
	}

	void Application::run()
	{
		while (m_isRunning)
		{
			SDL_RenderClear(m_renderer);
			onRender();
			SDL_RenderPresent(m_renderer);

			SDL_Event e;
			while (SDL_PollEvent(&e))
			{
				if (e.type == SDL_QUIT) m_isRunning = false;
			}
		}
	}

	Application* Application::s_instance = nullptr;

} // namespace EmuCommon
