// Scene.h
#pragma once
#include <vector>
#include <memory>
#include "Camera.h"
#include "DirectionalLight.h"
#include "MeshObject.h"
#include "Shader.h"

class Scene
{
private:
    std::vector<std::shared_ptr<MeshObject>> m_objects;
    std::shared_ptr<Camera> m_camera;
    std::shared_ptr<Shader> m_shader;
    std::vector<std::shared_ptr<DirectionalLight>> m_lights;

public:
    Scene();
    ~Scene();


    void AddObject(std::shared_ptr<MeshObject> object);
    void SetCamera(std::shared_ptr<Camera> camera);
    void SetShader(std::shared_ptr<Shader> shader);

    void AddLight(std::shared_ptr<DirectionalLight> light);
    void RemoveLight(const std::string& name);
    std::shared_ptr<DirectionalLight> GetLight(const std::string& name);

    void Update(double deltaTime);
    void Render();
};