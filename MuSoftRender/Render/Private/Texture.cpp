#include "Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


bool Texture::LoadFromFile(const std::string& filename)
{
    int channels;
    unsigned char* img = stbi_load(filename.c_str(), &width, &height, &channels, 3);
    if (!img) return false;

    data.assign(img, img + width * height * 3);
    stbi_image_free(img);
    return true;
}

Eigen::Vector3f Texture::Sample(const Eigen::Vector2f& texCoord) const
{
    if (data.empty()) return Eigen::Vector3f(1, 1, 1);

    int x = static_cast<int>(texCoord.x() * width) % width;
    int y = static_cast<int>(texCoord.y() * height) % height;
    int index = (y * width + x) * 3;

    return Eigen::Vector3f(
        data[index] / 255.0f,
        data[index + 1] / 255.0f,
        data[index + 2] / 255.0f);
}