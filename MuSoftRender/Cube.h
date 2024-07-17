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

        // ����8������
        mesh.AddVertex(Eigen::Vector3f(-halfSide, -halfSide, -halfSide));
        mesh.AddVertex(Eigen::Vector3f(halfSide, -halfSide, -halfSide));
        mesh.AddVertex(Eigen::Vector3f(halfSide, halfSide, -halfSide));
        mesh.AddVertex(Eigen::Vector3f(-halfSide, halfSide, -halfSide));
        mesh.AddVertex(Eigen::Vector3f(-halfSide, -halfSide, halfSide));
        mesh.AddVertex(Eigen::Vector3f(halfSide, -halfSide, halfSide));
        mesh.AddVertex(Eigen::Vector3f(halfSide, halfSide, halfSide));
        mesh.AddVertex(Eigen::Vector3f(-halfSide, halfSide, halfSide));

        // ����6���棬ÿ����2��������
        mesh.AddTriangle(0, 1, 2);
        mesh.AddTriangle(2, 3, 0); // ǰ��
        mesh.AddTriangle(4, 5, 6);
        mesh.AddTriangle(6, 7, 4); // ����
        mesh.AddTriangle(1, 5, 6);
        mesh.AddTriangle(6, 2, 1); // ����
        mesh.AddTriangle(0, 4, 7);
        mesh.AddTriangle(7, 3, 0); // ����
        mesh.AddTriangle(3, 2, 6);
        mesh.AddTriangle(6, 7, 3); // ����
        mesh.AddTriangle(0, 1, 5);
        mesh.AddTriangle(5, 4, 0); // ����
    }
};