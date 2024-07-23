#pragma once
#include "DirectionalLight.h"
#include "Object.h"

class Scene
{
private:
    std::vector<std::shared_ptr<Object>> Objects;

    std::vector<std::shared_ptr<Camera>> Cameras;

    std::shared_ptr<DirectionalLight> SceneDirectionalLight;

public:
    void AddObject(std::shared_ptr<Object> InObject)
    {
        Objects.push_back(std::move(InObject));
    }

    void AddCamera(std::shared_ptr<Camera> InCamera)
    {
        Cameras.push_back(std::move(InCamera));
    }

    void SetDirectionalLight(std::shared_ptr<DirectionalLight> InDirectionalLight)
    {
        SceneDirectionalLight = std::move(InDirectionalLight);
    }

    const std::vector<std::shared_ptr<Object>>& GetObjects() const
    {
        return Objects;
    }

    const std::vector<std::shared_ptr<Camera>>& GetCameras() const
    {
        return Cameras;
    }

    const std::shared_ptr<DirectionalLight>& GetDirectionalLight() const
    {
        return SceneDirectionalLight;
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