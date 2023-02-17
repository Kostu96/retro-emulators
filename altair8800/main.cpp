#include "altair.hpp"
#include "gui.hpp"

#include <glad/gl.h>
#include <glw/glw.hpp>
#include <GLFW/glfw3.h>

#include <iostream>
#include <thread>

static void glfwErrorCallback(int error, const char* description)
{
	std::cerr << "GLFW error " << error << ": " << description << '\n';
}

int main()
{
    glfwSetErrorCallback(glfwErrorCallback);
    if (!glfwInit()) {
        std::cerr << "GLFW init failed!\n";
        std::terminate();
    }

    glfwWindowHint(GLFW_RESIZABLE, 0);
    GLFWwindow* window = glfwCreateWindow(400, 300, "Altair 8800 Emulator by Kostu96", nullptr, nullptr);
    if (!window) {
        std::cerr << "GLFW window creation failed!\n";
        std::terminate();
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    glw::init(glfwGetProcAddress);
    GUI::init(window);

	Altair altair;
	
    std::thread emuThread{
        [&]() {
            while (!glfwWindowShouldClose(window)) {
                std::this_thread::sleep_for(std::chrono::nanoseconds{ 24 }); // TODO: temp
                altair.update();
            }
        }
    };

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        GUI::update(altair);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    emuThread.join();
	
    GUI::shutdown();
    glfwTerminate();
	return 0;
}
