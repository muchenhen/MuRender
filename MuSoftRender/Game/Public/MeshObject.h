#pragma once
#include "Mesh.h"
#include "Material.h"
#include "Object.h"

class MeshObject : public Object
{
private:
    std::shared_ptr<Mesh> MeshPtr;
    std::shared_ptr<Material> MaterialPtr;

public:
    MeshObject() = default;

    MeshObject(std::shared_ptr<Mesh> InMesh) :
        MeshPtr(std::move(InMesh))
    {
    }

    MeshObject(std::shared_ptr<Material> InMaterial) :
        MaterialPtr(std::move(InMaterial))
    {
    }

    MeshObject(std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> material) :
        MeshPtr(std::move(mesh)), MaterialPtr(std::move(material))
    {
    }

    virtual Mesh* GetMesh() const
    {
        return MeshPtr.get();
    }

    const Material* GetMaterial() const
    {
        return MaterialPtr.get();
    }

    void SetMesh(std::shared_ptr<Mesh> InMesh)
    {
        MeshPtr = std::move(InMesh);
    }

    void SetMaterial(std::shared_ptr<Material> InMaterial)
    {
        MaterialPtr = std::move(InMaterial);
    }
};