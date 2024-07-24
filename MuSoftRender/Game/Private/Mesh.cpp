#include "Mesh.h"

void Mesh::AddVertex(const Eigen::Vector3f& InPosition)
{
    Vertices.push_back({InPosition, Eigen::Vector2f(0, 0)});
}

void Mesh::AddVertex(const Eigen::Vector3f& InPosition, const Eigen::Vector2f& InUV)
{
    Vertices.push_back({InPosition, InUV});
}

void Mesh::AddVertex(const Eigen::Vector3f& InPosition, const Eigen::Vector2f& InUV, const Eigen::Vector3f& InNormal)
{
    Vertices.push_back({InPosition, InUV, InNormal});
}

void Mesh::AddTriangle(unsigned int Index1, unsigned int Index2, unsigned int Index3)
{
    Indices.push_back(Index1);
    Indices.push_back(Index2);
    Indices.push_back(Index3);
}

void Mesh::SetVertexUV(size_t Index, const Eigen::Vector2f& InUV)
{
    if (Index < Vertices.size())
    {
        Vertices[Index].UV = InUV;
    }
}