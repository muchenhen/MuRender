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
    int WidthMinus1;
    int HeightMinus1;
    uint32_t WidthMask;
    uint32_t HeightMask;
    static constexpr float Inv255 = 1.0f / 255.0f;
    bool IsPowerOfTwo;

    static bool IsPowerOf2(int Value)
    {
        return (Value & (Value - 1)) == 0 && Value > 0;
    }

public:
    Texture() :
        Width(0), Height(0), WidthMinus1(0), HeightMinus1(0)
    {
    }

    Texture(int InWidth, int InHeight)
    {
        Width = InWidth;
        Height = InHeight;

        CheckIsPowerOf2();

        Data.resize(static_cast<size_t>(Width) * Height * 3);
    }

    void CheckIsPowerOf2()
    {
        if (Width > 1)
        {
            WidthMinus1 = Width - 1;
        }
        else
        {
            WidthMinus1 = 0;
        }
        if (Height > 1)
        {
            HeightMinus1 = Height - 1;
        }
        else
        {
            HeightMinus1 = 0;
        }

        IsPowerOfTwo = IsPowerOf2(Width) && IsPowerOf2(Height);

        if (IsPowerOfTwo)
        {
            WidthMask = WidthMinus1;
            HeightMask = HeightMinus1;
        }
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