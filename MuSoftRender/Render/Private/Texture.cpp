#include "Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


bool Texture::LoadFromFile(const std::string& Filename)
{
    int Channels;
    unsigned char* Image = stbi_load(Filename.c_str(), &Width, &Height, &Channels, 3);

    if (!Image) return false;

    CheckIsPowerOf2();

    std::ptrdiff_t DataSize = static_cast<std::ptrdiff_t>(Width) *
                              static_cast<std::ptrdiff_t>(Height) *
                              static_cast<std::ptrdiff_t>(3);
    Data.assign(Image, Image + DataSize);
    stbi_image_free(Image);
    return true;
}

Eigen::Vector3f Texture::Sample(const Eigen::Vector2f& TexCoord) const
{
    if (Data.empty()) return Eigen::Vector3f::Ones();

    uint32_t u, v;

    if (IsPowerOfTwo)
    {
        u = static_cast<uint32_t>(TexCoord.x() * WidthMinus1) & WidthMask;
        v = static_cast<uint32_t>(TexCoord.y() * HeightMinus1) & HeightMask;
    }
    else
    {
        float uf = fmod(TexCoord.x(), 1.0f);
        float vf = fmod(TexCoord.y(), 1.0f);
        if (uf < 0.0f) uf += 1.0f;
        if (vf < 0.0f) vf += 1.0f;
        u = static_cast<uint32_t>(uf * WidthMinus1);
        v = static_cast<uint32_t>(vf * HeightMinus1);
    }

    uint32_t Index = (v * Width + u) * 3;
    
    return {
        static_cast<float>(Data[Index]) / 255.0f,
        static_cast<float>(Data[Index + 1]) / 255.0f,
        static_cast<float>(Data[Index + 2]) / 255.0f
    };
}