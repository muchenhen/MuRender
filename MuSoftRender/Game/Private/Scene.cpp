#include "Scene.h"

#include "Mesh.h"
#include "MeshObject.h"

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

BoundingBox Scene::CalculateSceneBounds() const
{
    BoundingBox SceneBounds;

    for (const auto& Object : Objects)
    {
        const MeshObject* MeshObjectPtr = dynamic_cast<const MeshObject*>(Object.get());
        if (!MeshObjectPtr) continue;

        const Mesh* MeshPtr = MeshObjectPtr->GetMesh();
        if (!MeshPtr) continue;

        Eigen::Matrix4f ModelMatrix = MeshObjectPtr->GetModelMatrix();

        for (const auto& Vertex : MeshPtr->Vertices)
        {
            Eigen::Vector4f WorldPos = ModelMatrix * Vertex.Position.homogeneous();
            SceneBounds.Expand(WorldPos.head<3>());
        }
    }

    constexpr float MinSize = 0.001f;
    if ((SceneBounds.Max - SceneBounds.Min).norm() < MinSize)
    {
        Eigen::Vector3f Center = (SceneBounds.Min + SceneBounds.Max) / 2.0f;
        SceneBounds.Min = Center - Eigen::Vector3f::Constant(MinSize / 2);
        SceneBounds.Max = Center + Eigen::Vector3f::Constant(MinSize / 2);
    }

    return SceneBounds;
}