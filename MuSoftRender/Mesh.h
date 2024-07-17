#pragma once
#include <vector>
#include <Eigen/Core>

class Mesh
{
public:
    struct Vertex
    {
        Eigen::Vector3f position;
    };

    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    void AddVertex(const Eigen::Vector3f& position)
    {
        vertices.push_back({position});
    }

    void AddTriangle(unsigned int i1, unsigned int i2, unsigned int i3)
    {
        indices.push_back(i1);
        indices.push_back(i2);
        indices.push_back(i3);
    }
};