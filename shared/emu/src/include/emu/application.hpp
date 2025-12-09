#pragma once
#include <utils/common.hpp>
#include <chrono>

struct SDL_Window;

namespace emu {

class Renderer2D;

class Application :
    utils::NonCopyable {
public:
    struct Properties {
        u16 window_width;
        u16 window_height;
    };

    explicit Application(const Properties& properties);
    virtual ~Application();

    void update();
protected:
    const Renderer2D& getRenderer() const { return *renderer2d_.get(); }

    virtual void on_event() {}
    virtual void on_update(s64 /*delta_time*/) {}
private:
    std::chrono::steady_clock clock_;
    std::chrono::steady_clock::time_point last_time_;

    const Properties properties_;
    std::unique_ptr<Renderer2D> renderer2d_;
    SDL_Window* window_ = nullptr;
};

} // namespace emu

// Client applcation need to define this and return newed object.
extern emu::Application* create_application();
