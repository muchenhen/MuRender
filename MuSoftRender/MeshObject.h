#pragma once
#include "Mesh.h"
#include "Material.h"
#include "Object.h"

class MeshObject : public Object
{
private:
    std::shared_ptr<Mesh> mesh;
    std::shared_ptr<Material> material;

public:
    MeshObject()
    {
        mesh = nullptr;
        material = nullptr;
    }

    MeshObject(std::shared_ptr<Mesh> mesh) :
        mesh(std::move(mesh))
    {
    }

    MeshObject(std::shared_ptr<Material> material) :
        material(std::move(material))
    {
    }

    MeshObject(std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> material) :
        mesh(std::move(mesh)), material(std::move(material))
    {
    }

    const Mesh& GetMesh() const
    {
        return *mesh;
    }
    const Material& GetMaterial() const
    {
        return *material;
    }

    void SetMesh(std::shared_ptr<Mesh> mesh)
    {
        mesh = std::move(mesh);
    }

    void SetMaterial(std::shared_ptr<Material> material)
    {
        material = std::move(material);
    }

};