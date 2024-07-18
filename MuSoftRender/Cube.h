#pragma once
#include "Object.h"
#include "Mesh.h"
#include "MeshObject.h"

class Cube : public MeshObject
{
private:
    float sideLength;

public:
    Cube(float side = 1.0f) :
        sideLength(side)
    {
        std::shared_ptr<Mesh> CubeMesh = GenerateGeometry();
        SetMesh(CubeMesh);
    }

    void SetSideLength(float side)
    {
        sideLength = side;
        std::shared_ptr<Mesh> CubeMesh = GenerateGeometry();
        SetMesh(CubeMesh);
    }

    float GetSideLength() const
    {
        return sideLength;
    }

    void Update(float deltaTime) override
    {
        Object::Update(deltaTime);
    }

private:
    std::shared_ptr<Mesh> GenerateGeometry() const
    {
        std::shared_ptr<Mesh> CubeMesh = std::make_shared<Mesh>();
        float halfSide = sideLength / 2.0f;

        CubeMesh->AddVertex(Eigen::Vector3f(-halfSide, -halfSide, -halfSide));
        CubeMesh->AddVertex(Eigen::Vector3f(halfSide, -halfSide, -halfSide));
        CubeMesh->AddVertex(Eigen::Vector3f(halfSide, halfSide, -halfSide));
        CubeMesh->AddVertex(Eigen::Vector3f(-halfSide, halfSide, -halfSide));
        CubeMesh->AddVertex(Eigen::Vector3f(-halfSide, -halfSide, halfSide));
        CubeMesh->AddVertex(Eigen::Vector3f(halfSide, -halfSide, halfSide));
        CubeMesh->AddVertex(Eigen::Vector3f(halfSide, halfSide, halfSide));
        CubeMesh->AddVertex(Eigen::Vector3f(-halfSide, halfSide, halfSide));

        CubeMesh->AddTriangle(0, 1, 2);
        CubeMesh->AddTriangle(2, 3, 0); // 前面
        CubeMesh->AddTriangle(4, 5, 6);
        CubeMesh->AddTriangle(6, 7, 4); // 后面
        CubeMesh->AddTriangle(1, 5, 6);
        CubeMesh->AddTriangle(6, 2, 1); // 右面
        CubeMesh->AddTriangle(0, 4, 7);
        CubeMesh->AddTriangle(7, 3, 0); // 左面
        CubeMesh->AddTriangle(3, 2, 6);
        CubeMesh->AddTriangle(6, 7, 3); // 上面
        CubeMesh->AddTriangle(0, 1, 5);
        CubeMesh->AddTriangle(5, 4, 0); // 下面

        return CubeMesh;
    }
};