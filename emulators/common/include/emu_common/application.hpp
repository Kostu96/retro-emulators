#pragma once

struct SDL_Window;
struct SDL_Renderer;
union SDL_Event;

namespace EmuCommon {

	class Application
	{
	public:
		Application(const char* title, int width, int height);
		virtual ~Application();

		void run();

		bool isRunning() const { return m_isRunning; }
		SDL_Renderer* getSDLRenderer() { return m_renderer; }

		static Application& get() { return *s_instance; }
	protected:
		virtual void onUpdate() {}
		virtual void onEvent(SDL_Event& /*e*/) {}
	private:
		bool m_isRunning = false;
		SDL_Window* m_window = nullptr;
		SDL_Renderer* m_renderer = nullptr;

		static Application* s_instance;
	};

} // namespace EmuCommon
