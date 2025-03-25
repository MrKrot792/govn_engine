#include "tve_app.hpp"
#include "tve_model.hpp"
#include "tve_log.hpp"
#include <GLFW/glfw3.h>
#include <array>
#include <cstdint>
#include <cstdio>
#include <memory>
#include <stdexcept>
#include <vector>
#include <vulkan/vulkan_core.h>

#define CLEAR_VALUE {0.231f, 0.243f, 0.259f, 1.0f}

namespace tve
{

App::App()
{
    loadModels();
    LOG_INFO("Loaded models!");
    createPipelineLayout();
    LOG_INFO("Created pipeline layout!");
    recreateSwapChain();
    LOG_INFO("Created swap chain!");
    createCommandBuffers();
    LOG_INFO("Created command buffers!");
}

App::~App()
{
    vkDestroyPipelineLayout(tveDevice.device(), pipelineLayout, nullptr);
}

void App::run()
{
    while (!tveWindow.shouldClose())
    {
        glfwPollEvents();
        drawFrame();
    }

    vkDeviceWaitIdle(tveDevice.device());
}

void App::createPipelineLayout()
{
    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 0;
    pipelineLayoutInfo.pSetLayouts = nullptr;

    pipelineLayoutInfo.pushConstantRangeCount = 0;
    pipelineLayoutInfo.pPushConstantRanges = nullptr;

    if (vkCreatePipelineLayout(tveDevice.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create pipeline layout");
    }
}

void App::createPipeline()
{
    PipelineConfigInfo pipelineConfig{};
    TvePipeline::defaultPipelineConfigInfo(pipelineConfig);

    pipelineConfig.renderPass = tveSwapChain->getRenderPass();
    pipelineConfig.pipelineLayout = pipelineLayout;
    tvePipeline = std::make_unique<TvePipeline>(tveDevice, "./shaders/simple_shader.vert.spv",
                                                "./shaders/simple_shader.frag.spv", pipelineConfig);
}

void App::recreateSwapChain()
{
    auto extent = tveWindow.getExtent();
    while (extent.width == 0 || extent.height == 0)
    {
        extent = tveWindow.getExtent();
        glfwWaitEvents();
    }

    vkDeviceWaitIdle(tveDevice.device());
    tveSwapChain = std::make_unique<TveSwapChain>(tveDevice, extent);
    createPipeline();
}

void App::createCommandBuffers()
{
    commandBuffers.resize(tveSwapChain->imageCount());

    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;

    allocInfo.commandPool = tveDevice.getCommandPool();
    allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

    if (vkAllocateCommandBuffers(tveDevice.device(), &allocInfo, commandBuffers.data()) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to allocate command buffers");
    }
}

void App::recordCommandBuffer(int imageIndex)
{
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

    if (vkBeginCommandBuffer(commandBuffers[imageIndex], &beginInfo) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to begin recording command buffer");
    }

    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = tveSwapChain->getRenderPass();
    renderPassInfo.framebuffer = tveSwapChain->getFrameBuffer(imageIndex);

    renderPassInfo.renderArea.offset = {0, 0};
    renderPassInfo.renderArea.extent = tveSwapChain->getSwapChainExtent();

    std::array<VkClearValue, 2> clearValues{};
    clearValues[0] = CLEAR_VALUE;
    clearValues[1] = {1.0f, 0.0f};

    renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
    renderPassInfo.pClearValues = clearValues.data();

    vkCmdBeginRenderPass(commandBuffers[imageIndex], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = static_cast<float>(tveSwapChain->getSwapChainExtent().width);
    viewport.height = static_cast<float>(tveSwapChain->getSwapChainExtent().height);
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 0.0f;
    VkRect2D scissor{{0, 0}, tveSwapChain->getSwapChainExtent()};
    vkCmdSetViewport(commandBuffers[imageIndex], 0, 1, &viewport);
    vkCmdSetScissor(commandBuffers[imageIndex], 0, 1, &scissor);

    tvePipeline->bind(commandBuffers[imageIndex]);
    tveModel->bind(commandBuffers[imageIndex]);
    tveModel->draw(commandBuffers[imageIndex]);

    vkCmdEndRenderPass(commandBuffers[imageIndex]);
    if (vkEndCommandBuffer(commandBuffers[imageIndex]) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to record a command buffer");
    }
}

void App::drawFrame()
{
    uint32_t imageIndex;
    auto result = tveSwapChain->acquireNextImage(&imageIndex);

    if (result == VK_ERROR_OUT_OF_DATE_KHR)
    {
        recreateSwapChain();
        return;
    }

    if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
    {
        throw std::runtime_error("Failed to acquire swap chain image");
    }

    recordCommandBuffer(imageIndex);
    result = tveSwapChain->submitCommandBuffers(&commandBuffers[imageIndex], &imageIndex);
    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || tveWindow.wasWindowResized()) 
    {
        tveWindow.resetWindowResizedFlag();    
        recreateSwapChain();
        return;
    }
    if (result != VK_SUCCESS)
    {
        LOG_ERROR("Failed to present swap chain image");
        throw std::runtime_error("Failed to present swap chain image");
    }
}

void App::loadModels()
{
    // std::vector<TveModel::Vertex> vertices{{{0.0f, -0.5f}}, {{0.5f, 0.5f}}, {{-0.5f, 0.5f}}};
    std::vector<TveModel::Vertex> vertices{
        {{0.0f, -1.0f}, {1.0, 0.0, 0.0}},
        {{1.0f, 1.0f}, {0.0, 1.0, 0.0}},
        {{-1.0f, 1.0f}, {0.0, 0.0, 1.0}},
    };

    tveModel = std::make_unique<TveModel>(tveDevice, vertices);
}

} // namespace tve
