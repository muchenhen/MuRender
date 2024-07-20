#pragma once
#include <vector>
#pragma warning(disable : 4819)
#include <Eigen/Core>
#pragma warning(default : 4819)
class Mesh
{
public:
    Mesh() = default;

    struct Vertex
    {
        Eigen::Vector3f Position;
    };

    std::vector<Vertex> Vertices;
    std::vector<unsigned int> Indices;

    void AddVertex(const Eigen::Vector3f& InPosition)
    {
        Vertices.push_back({InPosition});
    }

    void AddTriangle(unsigned int Index1, unsigned int Index2, unsigned int Index3)
    {
        Indices.push_back(Index1);
        Indices.push_back(Index2);
        Indices.push_back(Index3);
    }
};