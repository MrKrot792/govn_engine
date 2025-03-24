#pragma once

#include "tve_engine_device.hpp"
#include <cstdint>
#include <vector>
#include <vulkan/vulkan_core.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

namespace tve
{
class TveModel
{
  public:
    struct Vertex
    {
        glm::vec2 position;

        static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();
        static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();
    };

    TveModel(TveDevice &device, std::vector<Vertex> &vertices);
    ~TveModel();

    TveModel(const TveModel &) = delete;
    TveModel &operator=(const TveModel &) = delete;

    void bind(VkCommandBuffer commandBuffer);
    void draw(VkCommandBuffer commandBuffer);

  private:
    void createVertexBuffers(const std::vector<Vertex> &vertices);

    TveDevice &tveDevice;
    VkBuffer vertexBuffer;
    VkDeviceMemory vertexBufferMemory;

    uint32_t vertexCount;
};
} // namespace tve
