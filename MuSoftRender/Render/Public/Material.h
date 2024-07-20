#pragma once
#pragma warning(disable : 4819)
#include <Eigen/Core>
#pragma warning(default : 4819)
#include "Texture.h"

class Material
{
private:
    Eigen::Vector3f BaseColor;
    std::shared_ptr<Texture> TexturePtr;

public:
    Material() :
        BaseColor(1, 1, 1)
    {
    }

    void SetBaseColor(const Eigen::Vector3f& Color)
    {
        BaseColor = Color;
    }

    void SetTexture(const std::shared_ptr<Texture>& InTexture)
    {
        TexturePtr = InTexture;
    }

    bool HasTexture() const
    {
        return TexturePtr != nullptr;
    }

    Eigen::Vector3f SampleTexture(const Eigen::Vector2f& TexCoord) const
    {
        return TexturePtr->Sample(TexCoord);
    }

    Eigen::Vector3f GetBaseColor() const
    {
        return BaseColor;
    }

    Eigen::Vector3f GetColor(const Eigen::Vector2f& TexCoord) const
    {
        Eigen::Vector3f FinalColor;
        if (TexturePtr)
        {
            Eigen::Vector3f TextureColor = TexturePtr->Sample(TexCoord);
            FinalColor = TextureColor.cwiseProduct(BaseColor);
        }
        else
        {
            FinalColor = BaseColor;
        }

        return FinalColor;
    }
};