#pragma once
#include "Object.h"
#include "Mesh.h"
#include "MeshObject.h"
#include <cmath>

class Sphere : public MeshObject
{
private:
    float Radius;
    int Segments;
    int Rings;

public:
    Sphere(float InRadius = 1.0f, int InSegments = 32, int InRings = 16) :
        Radius(InRadius),
        Segments(InSegments),
        Rings(InRings)
    {
        std::shared_ptr<Mesh> SphereMesh = GenerateGeometry();
        SetMesh(SphereMesh);
    }

    void SetParameters(float InRadius, int InSegments, int InRings)
    {
        Radius = InRadius;
        Segments = InSegments;
        Rings = InRings;
        std::shared_ptr<Mesh> SphereMesh = GenerateGeometry();
        SetMesh(SphereMesh);
    }

    float GetRadius() const { return Radius; }
    int GetSegments() const { return Segments; }
    int GetRings() const { return Rings; }

    void Update(float InDeltaTime) override
    {
        Object::Update(InDeltaTime);
    }

private:
    std::shared_ptr<Mesh> GenerateGeometry() const
    {
        std::shared_ptr<Mesh> SphereMesh = std::make_shared<Mesh>();

        for (int ring = 0; ring <= Rings; ++ring)
        {
            float theta = ring * M_PI / Rings;
            float sinTheta = std::sin(theta);
            float cosTheta = std::cos(theta);

            for (int segment = 0; segment <= Segments; ++segment)
            {
                float phi = segment * 2 * M_PI / Segments;
                float sinPhi = std::sin(phi);
                float cosPhi = std::cos(phi);

                float x = Radius * sinTheta * cosPhi;
                float y = Radius * cosTheta;
                float z = Radius * sinTheta * sinPhi;

                Eigen::Vector3f position(x, y, z);
                Eigen::Vector3f normal = position.normalized();
                Eigen::Vector2f uv((float)segment / Segments, (float)ring / Rings);

                SphereMesh->AddVertex(position, uv, normal);
            }
        }

        for (int ring = 0; ring < Rings; ++ring)
        {
            for (int segment = 0; segment < Segments; ++segment)
            {
                int current = ring * (Segments + 1) + segment;
                int next = current + Segments + 1;

                SphereMesh->AddTriangle(current, next, current + 1);
                SphereMesh->AddTriangle(current + 1, next, next + 1);
            }
        }

        return SphereMesh;
    }
};