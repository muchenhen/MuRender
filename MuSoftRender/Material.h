#pragma once
#include <vector>
#include <Eigen/Core>
#include "Texture.h"

class Material
{
private:
    Eigen::Vector3f baseColor;
    std::shared_ptr<Texture> texture;

public:
    Material() :
        baseColor(1, 1, 1)
    {
    }

    void SetBaseColor(const Eigen::Vector3f& color)
    {
        baseColor = color;
    }

    void SetTexture(std::shared_ptr<Texture> tex)
    {
        texture = tex;
    }

    Eigen::Vector3f GetColor(const Eigen::Vector2f& texCoord) const
    {
        if (texture)
        {
            Eigen::Vector3f textureColor = texture->Sample(texCoord);
            return textureColor.cwiseProduct(baseColor);
        }
        return baseColor;
    }
};