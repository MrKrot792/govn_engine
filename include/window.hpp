#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.hpp>

// std
#include <cstdint>

class Window
{
  public:
    Window();
    ~Window();

    bool shouldClose()
    {
        return glfwWindowShouldClose(window);
    };

  private:
    const uint32_t WIDTH = 800;
    const uint32_t HEIGHT = 600;

    GLFWwindow *window;
};
