#pragma once
#include "Object.h"

class Scene
{
private:
    std::vector<std::unique_ptr<Object>> Objects;

    std::vector<std::unique_ptr<Camera>> Cameras;

public:
    void AddObject(std::unique_ptr<Object> InObject)
    {
        Objects.push_back(std::move(InObject));
    }

    void AddCamera(std::unique_ptr<Camera> InCamera)
    {
        Cameras.push_back(std::move(InCamera));
    }

    const std::vector<std::unique_ptr<Object>>& GetObjects() const
    {
        return Objects;
    }

    const std::vector<std::unique_ptr<Camera>>& GetCameras() const
    {
        return Cameras;
    }

    void Update(const float DeltaTime) const
    {
        for (auto& Object : Objects)
        {
            Object->Update(DeltaTime);
        }
        for (auto& Camera : Cameras)
        {
            Camera->Update(DeltaTime);
        }
    }
};