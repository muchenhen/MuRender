#include "VulkanDevice.h"
#include <vector>
#include <algorithm>

bool VulkanDevice::Initialize()
{
    if (!CreateWindow()) return false;
    if (!CreateInstance()) return false;
    if (!CreateSurface()) return false;
    if (!PickPhysicalDevice()) return false;
    if (!CreateLogicalDevice()) return false;
    if (!CreateSwapChain()) return false;
    if (!CreateImageViews()) return false;
    if (!CreateRenderPass()) return false;
    if (!CreateFramebuffers()) return false;
    if (!CreateCommandPool()) return false;
    if (!CreateCommandBuffers()) return false;
    return true;
}

void VulkanDevice::Shutdown()
{
    vkDeviceWaitIdle(m_device);

    vkDestroyCommandPool(m_device, m_commandPool, nullptr);
    for (auto framebuffer : m_swapChainFramebuffers)
    {
        vkDestroyFramebuffer(m_device, framebuffer, nullptr);
    }
    vkDestroyRenderPass(m_device, m_renderPass, nullptr);
    for (auto imageView : m_swapChainImageViews)
    {
        vkDestroyImageView(m_device, imageView, nullptr);
    }
    vkDestroySwapchainKHR(m_device, m_swapChain, nullptr);
    vkDestroyDevice(m_device, nullptr);
    vkDestroySurfaceKHR(m_instance, m_surface, nullptr);
    vkDestroyInstance(m_instance, nullptr);
    SDL_DestroyWindow(m_window);
    SDL_Quit();
}

void VulkanDevice::SetClearColor(float r, float g, float b, float a)
{
    m_clearColor = {r, g, b, a};
}

void VulkanDevice::DrawFrame()
{
    uint32_t imageIndex;
    vkAcquireNextImageKHR(m_device, m_swapChain, UINT64_MAX, VK_NULL_HANDLE, VK_NULL_HANDLE, &imageIndex);

    VkCommandBuffer commandBuffer = m_commandBuffers[imageIndex];

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

    vkBeginCommandBuffer(commandBuffer, &beginInfo);

    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = m_renderPass;
    renderPassInfo.framebuffer = m_swapChainFramebuffers[imageIndex];
    renderPassInfo.renderArea.offset = {0, 0};
    renderPassInfo.renderArea.extent = m_swapChainExtent;
    renderPassInfo.clearValueCount = 1;
    renderPassInfo.pClearValues = &m_clearColor;

    vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
    vkCmdEndRenderPass(commandBuffer);

    vkEndCommandBuffer(commandBuffer);

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;

    vkQueueSubmit(m_graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);

    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = &m_swapChain;
    presentInfo.pImageIndices = &imageIndex;

    vkQueuePresentKHR(m_graphicsQueue, &presentInfo);

    vkQueueWaitIdle(m_graphicsQueue);
}

bool VulkanDevice::CreateInstance()
{
    return false;
}

bool VulkanDevice::CreateWindow()
{
    return false;
}

bool VulkanDevice::PickPhysicalDevice()
{
    return false;
}

bool VulkanDevice::CreateLogicalDevice()
{
    return false;
}

bool VulkanDevice::CreateSurface()
{
    return false;
}

bool VulkanDevice::CreateSwapChain()
{
}

bool VulkanDevice::CreateImageViews()
{
}

bool VulkanDevice::CreateRenderPass()
{
}

bool VulkanDevice::CreateFramebuffers()
{
}

bool VulkanDevice::CreateCommandPool()
{
}

bool VulkanDevice::CreateCommandBuffers()
{
    return false;
}
