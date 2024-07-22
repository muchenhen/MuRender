#include "RHI/RHIDevice.h"

#include "RHIDefinitions.h"
#include "VulkanDevice.h"
// 未来会包含 DirectX/DirectXDevice.h

std::unique_ptr<RHIDevice> CreateRHIDevice(ERHIBackend backend)
{
    switch (backend)
    {
        case ERHIBackend::Vulkan:
            return std::make_unique<VulkanDevice>();
        case ERHIBackend::DirectX12:
            // return std::make_unique<DirectXDevice>();
            // 暂时返回 nullptr，直到我们实现 DirectX 支持
            return nullptr;
        default:
            return nullptr;
    }
}