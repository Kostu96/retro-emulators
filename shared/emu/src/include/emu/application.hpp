#pragma once
#include <utils/types.hpp>
#include <chrono>

struct SDL_Window;
struct SDL_Renderer;
struct SDL_Texture;

namespace emu {

class Application {
public:
    Application();
    virtual ~Application() = default;

    bool start();

    void update();
protected:
    virtual void on_event() {}
    virtual void on_update(s64 /*delta_time*/) {}
private:
    std::chrono::steady_clock clock_;
    std::chrono::steady_clock::time_point last_time_;

    SDL_Window* window_ = nullptr;
    SDL_Renderer* renderer_ = nullptr;
    SDL_Texture* render_texture_ = nullptr;
};

} // namespace emu

// Client applcation need to define this and return newed object.
extern emu::Application* create_application();
