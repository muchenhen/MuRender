#include "Scene.h"

void Scene::AddObject(std::shared_ptr<Object> InObject)
{
    Objects.push_back(std::move(InObject));
}

void Scene::AddCamera(std::shared_ptr<Camera> InCamera)
{
    Cameras.push_back(std::move(InCamera));
}

void Scene::SetDirectionalLight(std::shared_ptr<DirectionalLight> InDirectionalLight)
{
    SceneDirectionalLight = std::move(InDirectionalLight);
}

bool Scene::HasLight() const
{
    return SceneDirectionalLight != nullptr;
}

const std::vector<std::shared_ptr<Object>>& Scene::GetObjects() const
{
    return Objects;
}

const std::vector<std::shared_ptr<Camera>>& Scene::GetCameras() const
{
    return Cameras;
}

const std::shared_ptr<DirectionalLight>& Scene::GetDirectionalLight() const
{
    return SceneDirectionalLight;
}

void Scene::Update(const float DeltaTime) const
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