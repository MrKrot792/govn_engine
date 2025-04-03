#include "app.hpp"
#include <GLFW/glfw3.h>

void App::run()
{
}

void App::mainLoop()
{
    while (!window.shouldClose()) 
    {
        glfwPollEvents();
    }
}
