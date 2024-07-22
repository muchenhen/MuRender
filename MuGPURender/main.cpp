#define SDL_MAIN_HANDLED
#include "RHI/RHIDevice.h"
#include <iostream>
#include <SDL.h>

#include "VulkanDevice.h"

int main(int argc, char* argv[])
{
    auto device = CreateRHIDevice(ERHIBackend::Vulkan);
    if (!device)
    {
        std::cerr << "Failed to create RHI device" << std::endl;
        return 1;
    }

    if (!device->Initialize())
    {
        std::cerr << "Failed to initialize RHI device" << std::endl;
        return 1;
    }

     // 设置清除颜色为红色
    static_cast<VulkanDevice*>(device.get())->SetClearColor(1.0f, 0.0f, 0.0f, 1.0f);


    SDL_Event event;
    bool quit = false;
    while (!quit)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                quit = true;
            }
        }
    }

    device->Shutdown();

    return 0;
}