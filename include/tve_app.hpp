#pragma once

#include "tve_engine_device.hpp"
#include "tve_engine_swap_chain.hpp"
#include "tve_pipeline.hpp"
#include "tve_window.hpp"

// std
#include <memory>
#include <vector>

namespace tve
{
class App
{
  public:
    static constexpr int WIDTH = 800;
    static constexpr int HEIGHT = 600;

    App();
    ~App();

    App(const App &) = delete;
    App &operator=(const App &) = delete;

    void run();

  private:
    void createPipelineLayout();
    void createPipeline();
    void createCommandBuffers();
    void drawFrame();

    TveWindow tveWindow{WIDTH, HEIGHT, "Vulkan"};
    TveDevice tveDevice{tveWindow};
    TveSwapChain tveSwapChain{tveDevice, tveWindow.getExtent()};

    std::unique_ptr<TvePipeline> tvePipeline;
    VkPipelineLayout pipelineLayout;
    std::vector<VkCommandBuffer> commandBuffers;
};
} // namespace tve
