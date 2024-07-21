#pragma once
#include <vector>
#pragma warning(disable : 4819)
#include <Eigen/Core>
#pragma warning(default : 4819)
#include <string>


class Texture
{
private:
    std::vector<unsigned char> Data;
    int Width;
    int Height;

public:
    Texture() :
        Width(0), Height(0)
    {
    }

    Texture(int InWidth, int InHeight)
    {
        Width = InWidth;
        Height = InHeight;
        Data.resize(static_cast<size_t>(Width) * Height * 3);
    }

    bool LoadFromFile(const std::string& Filename);

    Eigen::Vector3f Sample(const Eigen::Vector2f& TexCoord) const;

    int GetWidth() const
    {
        return Width;
    }

    int GetHeight() const
    {
        return Height;
    }
};