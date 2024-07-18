#pragma once
#include <vector>
#include <Eigen/Core>

class Material
{
public:
    struct TextureData
    {
        std::vector<unsigned char> data;
        int width;
        int height;
    };

    Material() :
        baseColor(1, 1, 1)
    {
    }

    void SetBaseColor(const Eigen::Vector3f& color)
    {
        baseColor = color;
    }

    void SetTexture(const TextureData& tex)
    {
        texture = tex;
    }

    Eigen::Vector3f GetColor(const Eigen::Vector2f& texCoord) const
    {
        if (texture.data.empty())
        {
            return baseColor;
        }

        int x = static_cast<int>(texCoord.x() * texture.width) % texture.width;
        int y = static_cast<int>(texCoord.y() * texture.height) % texture.height;
        int index = (y * texture.width + x) * 3;

        Eigen::Vector3f textureColor(
            texture.data[index] / 255.0f,
            texture.data[index + 1] / 255.0f,
            texture.data[index + 2] / 255.0f);

        // 使用 cwiseProduct 进行逐元素乘法
        return textureColor.cwiseProduct(baseColor);
    }

private:
    Eigen::Vector3f baseColor;
    TextureData texture;
};