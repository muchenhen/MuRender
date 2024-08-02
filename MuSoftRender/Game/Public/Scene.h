#pragma once
#include <vector>
#include "Camera.h"
#include "DirectionalLight.h"
#include "BoundingBox.h"

class Scene
{
private:
    std::vector<std::shared_ptr<Object>> Objects;

    std::vector<std::shared_ptr<Camera>> Cameras;

    std::shared_ptr<DirectionalLight> SceneDirectionalLight;

public:
    void AddObject(std::shared_ptr<Object> InObject);

    void AddCamera(std::shared_ptr<Camera> InCamera);

    void SetDirectionalLight(std::shared_ptr<DirectionalLight> InDirectionalLight);

    bool HasLight() const;

    const std::vector<std::shared_ptr<Object>>& GetObjects() const;

    const std::vector<std::shared_ptr<Camera>>& GetCameras() const;

    const std::shared_ptr<DirectionalLight>& GetDirectionalLight() const;

    void Update(const float DeltaTime) const;

    BoundingBox CalculateSceneBounds() const;
};