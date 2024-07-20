#pragma once
#include "Object.h"
#include "Mesh.h"
#include "MeshObject.h"

class Cube : public MeshObject
{
private:
    float SideLength;

public:
    Cube(float InSide = 1.0f) :
        SideLength(InSide)
    {
        std::shared_ptr<Mesh> CubeMesh = GenerateGeometry();
        SetMesh(CubeMesh);
    }

    void SetSideLength(float InSide)
    {
        SideLength = InSide;
        std::shared_ptr<Mesh> CubeMesh = GenerateGeometry();
        SetMesh(CubeMesh);
    }

    float GetSideLength() const
    {
        return SideLength;
    }

    void Update(float InDeltaTime) override
    {
        Object::Update(InDeltaTime);
    }

private:
    std::shared_ptr<Mesh> GenerateGeometry() const
    {
        std::shared_ptr<Mesh> CubeMesh = std::make_shared<Mesh>();
        float HalfSide = SideLength / 2.0f;

        auto Point1 = Eigen::Vector3f(-HalfSide, -HalfSide, -HalfSide);
        auto Point2 = Eigen::Vector3f(HalfSide, -HalfSide, -HalfSide);
        auto Point3 = Eigen::Vector3f(HalfSide, HalfSide, -HalfSide);
        auto Point4 = Eigen::Vector3f(-HalfSide, HalfSide, -HalfSide);
        auto Point5 = Eigen::Vector3f(-HalfSide, -HalfSide, HalfSide);
        auto Point6 = Eigen::Vector3f(HalfSide, -HalfSide, HalfSide);
        auto Point7 = Eigen::Vector3f(HalfSide, HalfSide, HalfSide);
        auto Point8 = Eigen::Vector3f(-HalfSide, HalfSide, HalfSide);

        auto UV1 = Eigen::Vector2f(0, 0);
        auto UV2 = Eigen::Vector2f(1, 0);
        auto UV3 = Eigen::Vector2f(1, 1);
        auto UV4 = Eigen::Vector2f(0, 1);

        CubeMesh->AddVertex(Point1, UV1);
        CubeMesh->AddVertex(Point2, UV2);
        CubeMesh->AddVertex(Point3, UV3);
        CubeMesh->AddVertex(Point4, UV4);

        CubeMesh->AddVertex(Point6, UV1);
        CubeMesh->AddVertex(Point5, UV2);
        CubeMesh->AddVertex(Point8, UV3);
        CubeMesh->AddVertex(Point7, UV4);

        CubeMesh->AddVertex(Point2, UV1);
        CubeMesh->AddVertex(Point6, UV2);
        CubeMesh->AddVertex(Point7, UV3);
        CubeMesh->AddVertex(Point3, UV4);

        CubeMesh->AddVertex(Point5, UV1);
        CubeMesh->AddVertex(Point1, UV2);
        CubeMesh->AddVertex(Point4, UV3);
        CubeMesh->AddVertex(Point8, UV4);

        CubeMesh->AddVertex(Point4, UV1);
        CubeMesh->AddVertex(Point3, UV2);
        CubeMesh->AddVertex(Point7, UV3);
        CubeMesh->AddVertex(Point8, UV4);

        CubeMesh->AddVertex(Point5, UV1);
        CubeMesh->AddVertex(Point6, UV2);
        CubeMesh->AddVertex(Point2, UV3);
        CubeMesh->AddVertex(Point1, UV4);

        for (int i = 0; i < 6; ++i)
        {
            int baseIndex = i * 4;
            CubeMesh->AddTriangle(baseIndex, baseIndex + 1, baseIndex + 2);
            CubeMesh->AddTriangle(baseIndex, baseIndex + 2, baseIndex + 3);
        }

        return CubeMesh;
    }
};