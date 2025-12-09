#pragma once
#include <utils/types.hpp>

#include <array>

namespace emu {

class Renderer2D;

} // namespace emu

namespace calc4004 {

class Display :
    utils::NonCopyable {
public:
    Display() = default;

    void set_led_state(u8 index, u8 state);

    void render(const emu::Renderer2D& renderer) const;
private:
    static constexpr int num_leds_ = 12;
    std::array<u8, num_leds_> led_states_;
};

} // namespace calc4004
