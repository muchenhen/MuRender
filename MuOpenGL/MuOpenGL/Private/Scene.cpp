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

void Scene::Update(float deltaTime)
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

    for (auto& object : m_objects)
    {
        m_shader->SetMat4("model", object->GetModelMatrix());
        object->Draw(*m_shader);
    }
}