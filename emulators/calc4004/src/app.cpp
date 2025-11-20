#include "emulator.hpp"

#include <emu/application.hpp>

namespace calc4004 {

class App :
    public emu::Application {
public:
    App() = default;
protected:
    void on_update(s64 delta_time) override {
        time_accumulator_ += delta_time;
        
        while (time_accumulator_ >= calc4004::INSTRUCTION_TIME) {
            time_accumulator_ -= calc4004::INSTRUCTION_TIME;
            emulator_.clock();
        }
    }
private:
    calc4004::Emulator emulator_;
    s64 time_accumulator_ = 0; // nanoseconds
};

} // namespace calc4004

emu::Application* create_application() {
    return new calc4004::App;
}
