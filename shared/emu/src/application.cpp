#define SDL_MAIN_USE_CALLBACKS
#include "emu/application.hpp"
#include "emu/renderer_2d.hpp"

#include <SDL3/SDL_main.h>
#include <SDL3/SDL.h>

#include <iostream>
#include <print>

namespace emu {

// TODO(Kostu): temp
constexpr u16 SUPERSAMPLE_SCALE = 2;

Application::Application(const Properties& properties) :
    last_time_(clock_.now()),
    properties_(properties) {

    SDL_SetHint(SDL_HINT_RENDER_LINE_METHOD, "3");
    SDL_Renderer* renderer;
    if (!SDL_CreateWindowAndRenderer("calc4004", properties_.window_width, properties_.window_height, SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIGH_PIXEL_DENSITY, &window_, &renderer)) {
        throw utils::Exception("Could not create SDL window and renderer!");
    }

    renderer2d_ = std::make_unique<Renderer2D>(renderer,
        Renderer2D::Properties{
            .window_width = properties_.window_width,
            .window_height = properties_.window_height,
            .scale = SUPERSAMPLE_SCALE
        });
}

Application::~Application() {
    SDL_DestroyWindow(window_);
}

void Application::update() {
    auto now = clock_.now();
    auto dt = std::chrono::duration_cast<std::chrono::nanoseconds>(now - last_time_).count();
    last_time_ = now;

    renderer2d_->begin_frame(Color{ 10, 20, 10, 255 });
    on_update(dt);
    renderer2d_->end_frame();
}

} // namespace emu

extern SDL_AppResult SDL_AppInit(void** appstate, int /*argc*/, char** /*argv*/) {
    try {
        if (!SDL_Init(SDL_INIT_VIDEO)) {
            throw utils::Exception("Could not initialize SDL!");
        }

        emu::Application* app = create_application();
        *appstate = app;
        return SDL_APP_CONTINUE;
    }
    catch (const utils::Exception& e) {
        std::println(std::cerr, "{}", e);
        return SDL_APP_FAILURE;
    }
    catch (const std::exception& e) {
        std::println(std::cerr, "{}", e.what());
        return SDL_APP_FAILURE;
    }
    catch (...) {
        std::println(std::cerr, "unknown exception");
        return SDL_APP_FAILURE;
    }
}

extern void SDL_AppQuit(void* appstate, SDL_AppResult /*result*/) {
    auto* app = static_cast<emu::Application*>(appstate);
    delete app;
}

extern SDL_AppResult SDL_AppEvent(void* /*appstate*/, SDL_Event* event) {
    try {
        switch (event->type) {
        case SDL_EVENT_QUIT: return SDL_APP_SUCCESS;
        }

        return SDL_APP_CONTINUE;
    }
    catch (const utils::Exception& e) {
        std::println(std::cerr, "{}", e);
        return SDL_APP_FAILURE;
    }
    catch (const std::exception& e) {
        std::println(std::cerr, "{}", e.what());
        return SDL_APP_FAILURE;
    }
    catch (...) {
        std::println(std::cerr, "unknown exception");
        return SDL_APP_FAILURE;
    }
}

extern SDL_AppResult SDL_AppIterate(void* appstate) {
    try {
        auto* app = static_cast<emu::Application*>(appstate);
        app->update();
        return SDL_APP_CONTINUE;
    }
    catch (const utils::Exception& e) {
        std::println(std::cerr, "{}", e);
        return SDL_APP_FAILURE;
    }
    catch (const std::exception& e) {
        std::println(std::cerr, "{}", e.what());
        return SDL_APP_FAILURE;
    }
    catch (...) {
        std::println(std::cerr, "unknown exception");
        return SDL_APP_FAILURE;
    }
}
