#pragma once
#include <vector>
#include <Eigen/Core>
#include <string>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

class Texture
{
private:
    std::vector<unsigned char> data;
    int width;
    int height;

public:
    Texture() :
        width(0), height(0)
    {
    }
    Texture(int w, int h) :
        width(w), height(h), data(w * h * 3)
    {
    }

    bool LoadFromFile(const std::string& filename)
    {
        int channels;
        unsigned char* img = stbi_load(filename.c_str(), &width, &height, &channels, 3);
        if (!img) return false;

        data.assign(img, img + width * height * 3);
        stbi_image_free(img);
        return true;
    }

    Eigen::Vector3f Sample(const Eigen::Vector2f& texCoord) const
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
};