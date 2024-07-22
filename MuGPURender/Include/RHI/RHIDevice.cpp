#include "RHI/RHIDevice.h"

#include "RHIDefinitions.h"
#include "VulkanDevice.h"
// δ������� DirectX/DirectXDevice.h

std::unique_ptr<RHIDevice> CreateRHIDevice(ERHIBackend backend)
{
    switch (backend)
    {
        case ERHIBackend::Vulkan:
            return std::make_unique<VulkanDevice>();
        case ERHIBackend::DirectX12:
            // return std::make_unique<DirectXDevice>();
            // ��ʱ���� nullptr��ֱ������ʵ�� DirectX ֧��
            return nullptr;
        default:
            return nullptr;
    }
}