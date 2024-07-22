#pragma once
#include <vector>

#include "RHI/RHIDevice.h"
#include <vulkan/vulkan.h>
#include <SDL2/SDL.h>

class VulkanDevice : public RHIDevice
{
public:
    virtual bool Initialize() override;
    virtual void Shutdown() override;
    void SetClearColor(float r, float g, float b, float a);
    void DrawFrame();

private:
    VkInstance m_instance = VK_NULL_HANDLE;
    SDL_Window* m_window = nullptr;
    VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE;
    VkDevice m_device = VK_NULL_HANDLE;
    VkQueue m_graphicsQueue = VK_NULL_HANDLE;
    VkSurfaceKHR m_surface = VK_NULL_HANDLE;
    VkSwapchainKHR m_swapChain = VK_NULL_HANDLE;
    std::vector<VkImage> m_swapChainImages;
    std::vector<VkImageView> m_swapChainImageViews;
    VkFormat m_swapChainImageFormat;
    VkExtent2D m_swapChainExtent;
    VkRenderPass m_renderPass = VK_NULL_HANDLE;
    std::vector<VkFramebuffer> m_swapChainFramebuffers;
    VkCommandPool m_commandPool = VK_NULL_HANDLE;
    std::vector<VkCommandBuffer> m_commandBuffers;

    VkClearValue m_clearColor = {0.0f, 0.0f, 0.0f, 1.0f};

    bool CreateInstance();
    bool CreateWindow();
    bool PickPhysicalDevice();
    bool CreateLogicalDevice();
    bool CreateSurface();
    bool CreateSwapChain();
    bool CreateImageViews();
    bool CreateRenderPass();
    bool CreateFramebuffers();
    bool CreateCommandPool();
    bool CreateCommandBuffers();
};