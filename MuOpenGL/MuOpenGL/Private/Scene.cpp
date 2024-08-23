#include "Scene.h"

Scene::Scene()
{
}

Scene::~Scene()
{
}

void Scene::AddObject(std::shared_ptr<MeshObject> object)
{
    m_objects.push_back(object);
}

void Scene::SetCamera(std::shared_ptr<Camera> camera)
{
    m_camera = camera;
}

void Scene::SetShader(std::shared_ptr<Shader> shader)
{
    m_shader = shader;
}

void Scene::AddLight(std::shared_ptr<DirectionalLight> light)
{
    m_lights.push_back(light);
}

void Scene::RemoveLight(const std::string& name)
{
    auto it =
        std::remove_if(
            m_lights.begin(),
            m_lights.end(),
            [&name](const std::shared_ptr<DirectionalLight>& light)
            {
                return light->GetName() == name;
            });
    m_lights.erase(it, m_lights.end());
}

std::shared_ptr<DirectionalLight> Scene::GetLight(const std::string& name)
{
    auto it =
        std::find_if(
            m_lights.begin(),
            m_lights.end(),
            [&name](const std::shared_ptr<DirectionalLight>& light)
            {
                return light->GetName() == name;
            });
    return (it != m_lights.end()) ? *it : nullptr;
}

void Scene::Update(double deltaTime)
{
    for (auto& object : m_objects)
    {
        object->Update(deltaTime);
    }
    m_camera->Update(deltaTime);
}

void Scene::Render()
{
    m_shader->Use();
    m_shader->SetMat4("projection", m_camera->GetProjectionMatrix());
    m_shader->SetMat4("view", m_camera->GetViewMatrix());

    // 设置光照信息
    for (size_t i = 0; i < m_lights.size(); ++i)
    {
        if (m_lights[i]->IsEnabled())
        {
        }
    }

    for (auto& object : m_objects)
    {
        m_shader->SetMat4("model", object->GetModelMatrix());
        object->Draw(*m_shader);
    }
}