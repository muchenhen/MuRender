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

        auto NormalFront = Eigen::Vector3f(0, 0, -1);
        auto NormalBack = Eigen::Vector3f(0, 0, 1);
        auto NormalRight = Eigen::Vector3f(1, 0, 0);
        auto NormalLeft = Eigen::Vector3f(-1, 0, 0);
        auto NormalTop = Eigen::Vector3f(0, 1, 0);
        auto NormalBottom = Eigen::Vector3f(0, -1, 0);

        // Front
        CubeMesh->AddVertex(Point1, UV1, NormalFront);
        CubeMesh->AddVertex(Point2, UV2, NormalFront);
        CubeMesh->AddVertex(Point3, UV3, NormalFront);
        CubeMesh->AddVertex(Point4, UV4, NormalFront);

        // Back
        CubeMesh->AddVertex(Point6, UV1, NormalBack);
        CubeMesh->AddVertex(Point5, UV2, NormalBack);
        CubeMesh->AddVertex(Point8, UV3, NormalBack);
        CubeMesh->AddVertex(Point7, UV4, NormalBack);

        // Right
        CubeMesh->AddVertex(Point2, UV1, NormalRight);
        CubeMesh->AddVertex(Point6, UV2, NormalRight);
        CubeMesh->AddVertex(Point7, UV3, NormalRight);
        CubeMesh->AddVertex(Point3, UV4, NormalRight);

        // Left
        CubeMesh->AddVertex(Point5, UV1, NormalLeft);
        CubeMesh->AddVertex(Point1, UV2, NormalLeft);
        CubeMesh->AddVertex(Point4, UV3, NormalLeft);
        CubeMesh->AddVertex(Point8, UV4, NormalLeft);

        // Top
        CubeMesh->AddVertex(Point4, UV1, NormalTop);
        CubeMesh->AddVertex(Point3, UV2, NormalTop);
        CubeMesh->AddVertex(Point7, UV3, NormalTop);
        CubeMesh->AddVertex(Point8, UV4, NormalTop);

        // Bottom
        CubeMesh->AddVertex(Point5, UV1, NormalBottom);
        CubeMesh->AddVertex(Point6, UV2, NormalBottom);
        CubeMesh->AddVertex(Point2, UV3, NormalBottom);
        CubeMesh->AddVertex(Point1, UV4, NormalBottom);

        for (int i = 0; i < 6; ++i)
        {
            int BaseIndex = i * 4;
            CubeMesh->AddTriangle(BaseIndex, BaseIndex + 1, BaseIndex + 2);
            CubeMesh->AddTriangle(BaseIndex, BaseIndex + 2, BaseIndex + 3);
        }

        return CubeMesh;
    }
};