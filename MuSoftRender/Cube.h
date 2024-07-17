#pragma once
#include "Object.h"
#include <vector>

#include "Mesh.h"

class Cube : public Object
{
private:
    float sideLength;
    Mesh mesh;

public:
    Cube(float side = 1.0f) :
        Object(), sideLength(side)
    {
        generateGeometry();
    }

    void setSideLength(float side)
    {
        sideLength = side;
        generateGeometry();
        SetScale(Eigen::Vector3f(sideLength, sideLength, sideLength));
    }

    float getSideLength() const
    {
        return sideLength;
    }

    const Mesh& GetMesh() const
    {
        return mesh;
    }

    void Update(float deltaTime) override
    {
        Object::Update(deltaTime);
    }

private:
    void generateGeometry()
    {
        float halfSide = sideLength / 2.0f;

        // 定义8个顶点
        mesh.AddVertex(Eigen::Vector3f(-halfSide, -halfSide, -halfSide));
        mesh.AddVertex(Eigen::Vector3f(halfSide, -halfSide, -halfSide));
        mesh.AddVertex(Eigen::Vector3f(halfSide, halfSide, -halfSide));
        mesh.AddVertex(Eigen::Vector3f(-halfSide, halfSide, -halfSide));
        mesh.AddVertex(Eigen::Vector3f(-halfSide, -halfSide, halfSide));
        mesh.AddVertex(Eigen::Vector3f(halfSide, -halfSide, halfSide));
        mesh.AddVertex(Eigen::Vector3f(halfSide, halfSide, halfSide));
        mesh.AddVertex(Eigen::Vector3f(-halfSide, halfSide, halfSide));

        // 定义6个面，每个面2个三角形
        mesh.AddTriangle(0, 1, 2);
        mesh.AddTriangle(2, 3, 0); // 前面
        mesh.AddTriangle(4, 5, 6);
        mesh.AddTriangle(6, 7, 4); // 后面
        mesh.AddTriangle(1, 5, 6);
        mesh.AddTriangle(6, 2, 1); // 右面
        mesh.AddTriangle(0, 4, 7);
        mesh.AddTriangle(7, 3, 0); // 左面
        mesh.AddTriangle(3, 2, 6);
        mesh.AddTriangle(6, 7, 3); // 上面
        mesh.AddTriangle(0, 1, 5);
        mesh.AddTriangle(5, 4, 0); // 下面
    }
};