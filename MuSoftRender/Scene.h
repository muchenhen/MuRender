#pragma once
#include "Object.h"

class Scene
{
private:
    std::vector<std::unique_ptr<Object>> objects;

    std::vector<std::unique_ptr<Camera>> Cameras;

public:
    void AddObject(std::unique_ptr<Object> obj)
    {
        objects.push_back(std::move(obj));
    }

    void AddCamera(std::unique_ptr<Camera> camera)
    {
        Cameras.push_back(std::move(camera));
    }

    const std::vector<std::unique_ptr<Object>>& GetObjects() const
    {
        return objects;
    }

    const std::vector<std::unique_ptr<Camera>>& GetCameras() const
    {
        return Cameras;
    }

    void Update(float deltaTime)
    {
        for (auto& obj : objects)
        {
            obj->Update(deltaTime);
        }
        for (auto& camera : Cameras)
        {
            camera->Update(deltaTime);
        }
    }
};