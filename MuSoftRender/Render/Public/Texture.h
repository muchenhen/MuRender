#pragma once
#include <vector>
#include <Eigen/Core>
#include <string>


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

    bool LoadFromFile(const std::string& filename);

    Eigen::Vector3f Sample(const Eigen::Vector2f& texCoord) const;
};