#define SDL_MAIN_USE_CALLBACKS
#include "emu/application.hpp"

#include <SDL3/SDL_main.h>
#include <SDL3/SDL.h>

namespace emu {

// TODO(Kostu): temp
constexpr u16 SUPERSAMPLE_SCALE = 2;

Application::Application(const Properties& properties) :
    last_time_(clock_.now()),
    properties_(properties) {}

Application::~Application() {
    SDL_DestroyWindow(window_);
}

// TODO(Kostu):: move to contructor, exceptions
bool Application::start() {
    SDL_SetHint(SDL_HINT_RENDER_LINE_METHOD, "3");
    SDL_Renderer* renderer;
    if (!SDL_CreateWindowAndRenderer("calc4004", properties_.window_width, properties_.window_height, SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIGH_PIXEL_DENSITY, &window_, &renderer)) {
        return false;
    }

    renderer2d_ = std::make_unique<Renderer2D>(renderer,
        Renderer2D::Properties{
            .window_width = properties_.window_width,
            .window_height = properties_.window_height,
            .scale = SUPERSAMPLE_SCALE
        });
    return true;
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
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        return SDL_APP_FAILURE;
    }

    emu::Application* app = create_application();
    *appstate = app;
    if (!app->start()) {
        return SDL_APP_FAILURE;
    }

    return SDL_APP_CONTINUE;
}

extern void SDL_AppQuit(void* appstate, SDL_AppResult /*result*/) {
    auto* app = static_cast<emu::Application*>(appstate);
    delete app;
}

extern SDL_AppResult SDL_AppEvent(void* /*appstate*/, SDL_Event* event) {
    switch (event->type) {
    case SDL_EVENT_QUIT: return SDL_APP_SUCCESS;
    }

    return SDL_APP_CONTINUE;
}

extern SDL_AppResult SDL_AppIterate(void* appstate) {
    auto* app = static_cast<emu::Application*>(appstate);
    app->update();
    return SDL_APP_CONTINUE;
}
