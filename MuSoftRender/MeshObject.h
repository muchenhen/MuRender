#pragma once
#include "Mesh.h"
#include "Material.h"
#include "Object.h"

class MeshObject : public Object
{
private:
    std::shared_ptr<Mesh> m_mesh;
    std::shared_ptr<Material> m_material;

public:
    MeshObject() = default;

    MeshObject(std::shared_ptr<Mesh> mesh) :
        m_mesh(std::move(mesh))
    {
    }

    MeshObject(std::shared_ptr<Material> material) :
        m_material(std::move(material))
    {
    }

    MeshObject(std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> material) :
        m_mesh(std::move(mesh)), m_material(std::move(material))
    {
    }

    virtual Mesh* GetMesh() const
    {
        return m_mesh.get();
    }

    const Material* GetMaterial() const
    {
        return m_material.get();
    }

    void SetMesh(std::shared_ptr<Mesh> mesh)
    {
        m_mesh = std::move(mesh);
    }

    void SetMaterial(std::shared_ptr<Material> material)
    {
        m_material = std::move(material);
    }
};