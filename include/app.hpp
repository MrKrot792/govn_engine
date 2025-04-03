#pragma once

#include "window.hpp"

class App
{
  public:
    void run(); // A starting point of Vulkan

  private:
    Window window;

    void mainLoop();
};
