#include "Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


bool Texture::LoadFromFile(const std::string& Filename)
{
    int Channels;
    unsigned char* Image = stbi_load(Filename.c_str(), &Width, &Height, &Channels, 3);
    if (!Image) return false;

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

    float u = std::fmod(TexCoord.x(), 1.0f);
    float v = std::fmod(TexCoord.y(), 1.0f);
    if (u < 0) u += 1.0f;
    if (v < 0) v += 1.0f;

    int X = static_cast<int>(u * (Width - 1));
    int Y = static_cast<int>(v * (Height - 1));
    int Index = (Y * Width + X) * 3;
    
    return {
        static_cast<float>(Data[Index]) / 255.0f,
        static_cast<float>(Data[Index + 1]) / 255.0f,
        static_cast<float>(Data[Index + 2]) / 255.0f
    };
}