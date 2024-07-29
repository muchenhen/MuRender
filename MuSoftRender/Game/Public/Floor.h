#pragma once
#include "MeshObject.h"

class Floor : public MeshObject
{
public:
    Floor() :
        MeshObject()
    {
        std::shared_ptr<Mesh> FloorMesh = GenerateGeometry();
        SetMesh(FloorMesh);
    }

private:
    std::shared_ptr<Mesh> GenerateGeometry() const
    {
        std::shared_ptr<Mesh> FloorMesh = std::make_shared<Mesh>();

        auto Point1 = Eigen::Vector3f(-5.0f, 0.0f, -5.0f);
        auto Point2 = Eigen::Vector3f(5.0f, 0.0f, -5.0f);
        auto Point3 = Eigen::Vector3f(5.0f, 0.0f, 5.0f);
        auto Point4 = Eigen::Vector3f(-5.0f, 0.0f, 5.0f);

        auto UV1 = Eigen::Vector2f(0, 0);
        auto UV2 = Eigen::Vector2f(1, 0);
        auto UV3 = Eigen::Vector2f(1, 1);
        auto UV4 = Eigen::Vector2f(0, 1);

        auto Normal = Eigen::Vector3f(0, 1, 0);

        FloorMesh->AddVertex(Point1, UV1, Normal);
        FloorMesh->AddVertex(Point2, UV2, Normal);
        FloorMesh->AddVertex(Point3, UV3, Normal);
        FloorMesh->AddVertex(Point4, UV4, Normal);

        FloorMesh->AddTriangle(0, 1, 2);
        FloorMesh->AddTriangle(0, 2, 3);

        return FloorMesh;
    }
};