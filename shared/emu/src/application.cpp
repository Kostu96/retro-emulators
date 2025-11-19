#include "emu/application.hpp"

#define _USE_MATH_DEFINES
#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL_main.h>
#include <SDL3/SDL.h>
#include <cmath>

static SDL_FPoint operator*=(SDL_FPoint lhs, float rhs) {
    lhs.x *= rhs;
    lhs.y *= rhs;
    return lhs;
}

static SDL_FPoint operator*(SDL_FPoint lhs, float rhs) {
    return lhs *= rhs;
}

static SDL_FPoint& operator+=(SDL_FPoint& lhs, SDL_FPoint rhs) {
    lhs.x += rhs.x;
    lhs.y += rhs.y;
    return lhs;
}

static SDL_FPoint operator+(SDL_FPoint lhs, SDL_FPoint rhs) {
    lhs += rhs;
    return lhs;
}

namespace emu {

// TODO(Kostu): temp
constexpr int WINDOW_WIDTH = 640;
constexpr int WINDOW_HEIGHT = 640;
constexpr int SUPERSAMPLE_SCALE = 2;
constexpr int RENDER_WIDTH = SUPERSAMPLE_SCALE * WINDOW_WIDTH;
constexpr int RENDER_HEIGHT = SUPERSAMPLE_SCALE * WINDOW_HEIGHT;

constexpr SDL_FColor red_color = { 1.f, 0.f, 0.f, 1.f };

struct LEDSegmentGeometry {
    static constexpr int max_num_vertices = 6;
    static constexpr int max_num_indices = 12;
    int num_vertices;
    int num_indices;
    SDL_FPoint positions[max_num_vertices];
    int indices[max_num_indices];
};

constexpr LEDSegmentGeometry a_segment{
    .num_vertices = 6,
    .num_indices = 12,
    .positions = {
        {  9.6f, 1.0f },
        { 22.9f, 1.0f },
        { 24.1f, 2.3f },
        { 21.4f, 4.7f },
        { 10.1f, 4.7f },
        {  8.0f, 2.6f }
    },
    .indices = {
        0, 1, 2,
        0, 2, 3,
        3, 4, 5,
        3, 5, 0
    }
};

constexpr LEDSegmentGeometry b_segment{
    .num_vertices = 5,
    .num_indices = 9,
    .positions = {
        { 24.7f, 3.4f },
        { 26.0f, 4.7f },
        { 23.6f, 17.2f },
        { 20.4f, 15.3f },
        { 22.1f, 5.8f }
    },
    .indices = {
        0, 1, 2,
        0, 2, 3,
        3, 4, 0
    }
};

constexpr LEDSegmentGeometry c_segment{
    .num_vertices = 5,
    .num_indices = 9,
    .positions = {
        { 19.5f, 21.1f },
        { 17.8f, 30.1f },
        { 19.8f, 32.4f },
        { 21.3f, 30.6f },
        { 23.7f, 17.9f }
    },
    .indices = {
        0, 1, 2,
        0, 2, 3,
        3, 4, 0
    }
};

constexpr LEDSegmentGeometry d_segment{
    .num_vertices = 6,
    .num_indices = 12,
    .positions = {
        {  2.6f, 33.0f },
        {  3.8f, 34.3f },
        { 17.2f, 34.3f },
        { 18.7f, 32.8f },
        { 16.8f, 30.5f },
        {  5.3f, 30.5f }
    },
    .indices = {
        0, 1, 2,
        0, 2, 3,
        3, 4, 5,
        3, 5, 0
    }
};

constexpr LEDSegmentGeometry e_segment{
    .num_vertices = 5,
    .num_indices = 9,
    .positions = {
        { 3.1f, 18.1f },
        { 0.7f, 30.7f },
        { 2.0f, 31.9f },
        { 4.5f, 29.5f },
        { 6.4f, 20.2f }
    },
    .indices = {
        0, 1, 2,
        0, 2, 3,
        3, 4, 0
    }
};

constexpr LEDSegmentGeometry f_segment{
    .num_vertices = 5,
    .num_indices = 9,
    .positions = {
        { 5.5f,  4.8f },
        { 3.1f, 17.1f },
        { 7.3f, 14.9f },
        { 8.9f,  4.7f },
        { 7.2f,  2.6f }
    },
    .indices = {
        0, 1, 2,
        0, 2, 3,
        3, 4, 0
    }
};

constexpr LEDSegmentGeometry g_segment{
    .num_vertices = 6,
    .num_indices = 12,
    .positions = {
        {  7.3f, 16.1f },
        {  4.4f, 17.7f },
        {  7.4f, 19.6f },
        { 19.3f, 19.6f },
        { 22.0f, 17.9f },
        { 19.1f, 16.1f }
    },
    .indices = {
        0, 1, 2,
        0, 2, 3,
        3, 4, 5,
        3, 5, 0
    }
};

Application::Application() :
    last_time_(clock_.now()) {}

bool Application::start() {
    if (!SDL_CreateWindowAndRenderer("calc4004", WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIGH_PIXEL_DENSITY, &window_, &renderer_)) {
        return false;
    }
    SDL_SetHint(SDL_HINT_RENDER_LINE_METHOD, "3");
    SDL_SetRenderLogicalPresentation(renderer_, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_LOGICAL_PRESENTATION_LETTERBOX);

    render_texture_ = SDL_CreateTexture(renderer_, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, RENDER_WIDTH, RENDER_HEIGHT);
    SDL_SetTextureScaleMode(render_texture_, SDL_SCALEMODE_LINEAR);
    SDL_SetRenderTarget(renderer_, render_texture_);
    SDL_SetRenderScale(renderer_, SUPERSAMPLE_SCALE, SUPERSAMPLE_SCALE);
    
    return true;
}

void Application::update() {
    auto now = clock_.now();
    auto dt = std::chrono::duration_cast<std::chrono::nanoseconds>(now - last_time_).count();
    last_time_ = now;

    SDL_FRect rect;

    SDL_SetRenderTarget(renderer_, render_texture_);
    SDL_SetRenderDrawColor(renderer_, 10, 20, 10, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer_);

    constexpr int num_leds = 12;
    constexpr int display_margin = 21; // TODO(Kostu): fix this to account for padding
    constexpr int led_width = 48;
    constexpr int led_height = 60;
    constexpr int display_spacing = 2;
    constexpr int display_padding = 8;

    SDL_SetRenderDrawColor(renderer_, 10, 40, 50, SDL_ALPHA_OPAQUE);
    rect.x = display_margin - display_padding;
    rect.y = display_margin - display_padding;
    rect.w = led_width * num_leds + display_spacing * (num_leds - 1) + display_padding * 2;
    rect.h = led_height + display_padding * 2;
    SDL_RenderFillRect(renderer_, &rect);

    auto drawSegment = [this](const LEDSegmentGeometry& segment, float scale, SDL_FPoint offset) {
        SDL_Vertex vertices[LEDSegmentGeometry::max_num_vertices];
        for (int i = 0; i < segment.num_vertices; i++) {
            vertices[i].position = segment.positions[i] * scale + offset;
            vertices[i].color = red_color;
        }
        int indices[LEDSegmentGeometry::max_num_indices];
        for (int i = 0; i < segment.num_indices; i++) {
            indices[i] = segment.indices[i];
        }
        SDL_RenderGeometry(renderer_, nullptr, vertices, segment.num_vertices, indices, segment.num_indices);
    };

    SDL_FPoint offset = { 23.f, 23.f };
    float scale = 1.6f;

    for (int i = 0; i < num_leds; i++) {
        SDL_SetRenderDrawColor(renderer_, 10, 240, 50, SDL_ALPHA_OPAQUE);
        rect.x = to_f32(display_margin + i * (led_width + display_spacing));
        rect.y = display_margin;
        rect.w = led_width;
        rect.h = led_height;
        SDL_RenderRect(renderer_, &rect);

        drawSegment(a_segment, scale, offset);
        drawSegment(b_segment, scale, offset);
        drawSegment(c_segment, scale, offset);
        drawSegment(d_segment, scale, offset);
        drawSegment(e_segment, scale, offset);
        drawSegment(f_segment, scale, offset);
        drawSegment(g_segment, scale, offset);

        // draw dot
        float radius = 2.f;
        SDL_FPoint center = { 24.f, 32.8f };
        int num_ring_vertices = 12;
        std::vector<SDL_Vertex> vertices;
        std::vector<int> indices;
        vertices.reserve(num_ring_vertices + 1);
        indices.reserve(num_ring_vertices * 3);
        
        vertices.emplace_back(SDL_Vertex{ .position = center * scale + offset, .color = red_color });

        const float step = 2.f * to_f32(M_PI) / num_ring_vertices;
        for (int x = 0; x < num_ring_vertices; x++) {
            float alpha = x * step;
            SDL_Vertex vert;
            vert.position.x = (center.x + std::cos(alpha) * radius) * scale + offset.x;
            vert.position.y = (center.y + std::sin(alpha) * radius) * scale + offset.y;
            vert.color = red_color;
            vertices.emplace_back(std::move(vert));
        }

        for (int x = 1; x <= num_ring_vertices; x++) {
            int i1 = x;
            int i2 = (x < num_ring_vertices) ? (x + 1) : 1;
            indices.push_back(0);
            indices.push_back(i1);
            indices.push_back(i2);
        }

        SDL_RenderGeometry(renderer_, nullptr, vertices.data(), to_s32(vertices.size()), indices.data(), to_s32(indices.size()));

        offset.x += led_width + display_spacing;
    }

    on_update(dt);

    SDL_SetRenderTarget(renderer_, nullptr);
    SDL_SetRenderDrawColor(renderer_, 255, 0, 255, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer_);
    rect = { 0.f, 0.f, WINDOW_WIDTH, WINDOW_HEIGHT };
    SDL_RenderTexture(renderer_, render_texture_, nullptr, &rect);
    SDL_RenderPresent(renderer_);
}

} // namespace emu

SDL_AppResult SDL_AppInit(void** appstate, int /*argc*/, char** /*argv*/) {
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

void SDL_AppQuit(void* appstate, SDL_AppResult /*result*/) {
    auto* app = static_cast<emu::Application*>(appstate);
    delete app;
}

SDL_AppResult SDL_AppEvent(void* /*appstate*/, SDL_Event* event) {
    switch (event->type) {
    case SDL_EVENT_QUIT: return SDL_APP_SUCCESS;
    }

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void* appstate) {
    auto* app = static_cast<emu::Application*>(appstate);
    app->update();
    return SDL_APP_CONTINUE;
}
