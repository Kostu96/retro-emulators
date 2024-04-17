#pragma once

class Application;
class Gameboy;

namespace GUI {

    void init(Application* window);
    void shutdown();
    void update(Gameboy& gb);

} // namespace GUI
