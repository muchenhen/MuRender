#pragma once
#include "RHIDefinitions.h"
#include <memory>

class RHIDevice
{
public:
    virtual ~RHIDevice() = default;
    virtual bool Initialize() = 0;
    virtual void Shutdown() = 0;
};

std::unique_ptr<RHIDevice> CreateRHIDevice(ERHIBackend backend);