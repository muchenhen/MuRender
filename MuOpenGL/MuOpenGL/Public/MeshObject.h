#pragma once
#include <memory>
#include <vector>

#include "Object.h"
#include "Shader.h"
#include "Texture.h"

struct Vertex
{
    float position[3];
    float color[3];
    float texCoords[2];
};

class MeshObject : public Object
{
private:
    std::vector<Vertex> m_vertices;
    std::vector<unsigned int> m_indices;
    unsigned int m_vao, m_vbo, m_ebo;
    size_t m_vertexCount;
    size_t m_indexCount;

    std::shared_ptr<Texture> m_texture;

    void SetupMesh();
    
public:
    MeshObject() = default;
    MeshObject(const std::string& name,
               const std::vector<Vertex>& vertices,
               const std::vector<unsigned int>& indices);
    ~MeshObject() override;

    virtual void Draw(const Shader& shader) const;
    void Update(double deltaTime) override;

    static MeshObject* CreateCube(const std::string& name);

    void SetTexture(const std::shared_ptr<Texture>& texture);
    const std::shared_ptr<Texture>& GetTexture() const;
};