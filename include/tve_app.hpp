#pragma once

#include "tve_engine_device.hpp"
#include "tve_engine_swap_chain.hpp"
#include "tve_model.hpp"
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
    static constexpr int WIDTH = 1000;
    static constexpr int HEIGHT = 1000;

    App();
    ~App();

    App(const App &) = delete;
    App &operator=(const App &) = delete;

    void run();

  private:
    void loadModels();
    void createPipelineLayout();
    void createPipeline();
    void createCommandBuffers();
    void drawFrame();
    void recreateSwapChain();
    void recordCommandBuffer(int imageIndex);

    TveWindow tveWindow{WIDTH, HEIGHT, "Vulkan"};
    TveDevice tveDevice{tveWindow};
    std::unique_ptr<TveSwapChain> tveSwapChain;

    std::unique_ptr<TvePipeline> tvePipeline;
    VkPipelineLayout pipelineLayout;
    std::vector<VkCommandBuffer> commandBuffers;
    std::unique_ptr<TveModel> tveModel;
};
} // namespace tve
