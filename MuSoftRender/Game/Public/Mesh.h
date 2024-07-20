#pragma once
#include <vector>
#pragma warning(disable : 4819)
#include <Eigen/Core>
#pragma warning(default : 4819)

struct Vertex
{
    Eigen::Vector3f Position;
    Eigen::Vector2f UV;
};

class Mesh
{
public:
    std::vector<Vertex> Vertices;
    std::vector<unsigned int> Indices;
    
public:
    Mesh() = default;

    void AddVertex(const Eigen::Vector3f& InPosition);
    
    void AddVertex(const Eigen::Vector3f& InPosition, const Eigen::Vector2f& InUV);

    void AddTriangle(unsigned int Index1, unsigned int Index2, unsigned int Index3);

    void SetVertexUV(size_t Index, const Eigen::Vector2f& InUV);
};