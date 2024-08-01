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
    bool IsFirstValidObject = true;

    for (const auto& Object : Objects)
    {
        const MeshObject* MeshObjectPtr = dynamic_cast<const MeshObject*>(Object.get());
        if (!MeshObjectPtr || !MeshObjectPtr->GetCastShadow() || !MeshObjectPtr->GetMesh()) continue;

        const Mesh* MeshPtr = MeshObjectPtr->GetMesh();
        const Eigen::Matrix4f& ModelMatrix = MeshObjectPtr->GetModelMatrix();

        // 计算物体的AABB
        BoundingBox ObjectBounds;
        for (const auto& Vertex : MeshPtr->Vertices)
        {
            ObjectBounds.Expand(Vertex.Position);
        }

        // 变换AABB的8个角点
        std::vector<Eigen::Vector3f> CornerPoints = {
            {ObjectBounds.Min.x(), ObjectBounds.Min.y(), ObjectBounds.Min.z()},
            {ObjectBounds.Max.x(), ObjectBounds.Min.y(), ObjectBounds.Min.z()},
            {ObjectBounds.Min.x(), ObjectBounds.Max.y(), ObjectBounds.Min.z()},
            {ObjectBounds.Max.x(), ObjectBounds.Max.y(), ObjectBounds.Min.z()},
            {ObjectBounds.Min.x(), ObjectBounds.Min.y(), ObjectBounds.Max.z()},
            {ObjectBounds.Max.x(), ObjectBounds.Min.y(), ObjectBounds.Max.z()},
            {ObjectBounds.Min.x(), ObjectBounds.Max.y(), ObjectBounds.Max.z()},
            {ObjectBounds.Max.x(), ObjectBounds.Max.y(), ObjectBounds.Max.z()}
        };

        for (auto& Point : CornerPoints)
        {
            Eigen::Vector4f TransformedPoint = ModelMatrix * Point.homogeneous();
            if (IsFirstValidObject)
            {
                SceneBounds.Min = SceneBounds.Max = TransformedPoint.head<3>();
                IsFirstValidObject = false;
            }
            else
            {
                SceneBounds.Expand(TransformedPoint.head<3>());
            }
        }
    }

    // 处理边缘情况
    if (IsFirstValidObject) // 没有有效对象
    {
        SceneBounds.Min = SceneBounds.Max = Eigen::Vector3f::Zero();
    }

    constexpr float MinSize = 0.001f;
    Eigen::Vector3f Size = SceneBounds.Max - SceneBounds.Min;
    if (Size.maxCoeff() < MinSize)
    {
        Eigen::Vector3f Center = (SceneBounds.Min + SceneBounds.Max) / 2.0f;
        SceneBounds.Min = Center - Eigen::Vector3f::Constant(MinSize / 2);
        SceneBounds.Max = Center + Eigen::Vector3f::Constant(MinSize / 2);
    }

    return SceneBounds;
}