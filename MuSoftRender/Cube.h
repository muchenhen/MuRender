#pragma once
#include "Object.h"
#include <vector>

class Cube : public Object
{
private:
    float sideLength;
    std::vector<Eigen::Vector3f> vertices;
    std::vector<Eigen::Vector3f> normals;
    std::vector<unsigned int> indices;

public:
    Cube(float side = 1.0f) :
        Object(), sideLength(side)
    {
        generateGeometry();
    }

    void setSideLength(float side)
    {
        sideLength = side;
        generateGeometry();
        SetScale(Eigen::Vector3f(sideLength, sideLength, sideLength));
    }

    float getSideLength() const
    {
        return sideLength;
    }

    const std::vector<Eigen::Vector3f>& getVertices() const
    {
        return vertices;
    }

    const std::vector<Eigen::Vector3f>& getNormals() const
    {
        return normals;
    }

    const std::vector<unsigned int>& getIndices() const
    {
        return indices;
    }

    void Update(float deltaTime) override
    {
        Object::Update(deltaTime);
    }

private:
    void generateGeometry()
    {
        float halfSide = 0.5f; // 生成一个单位立方体，然后通过 scale 来调整大小

        // 清空之前的数据
        vertices.clear();
        normals.clear();
        indices.clear();

        // 生成顶点
        vertices = {
            {-halfSide, -halfSide, -halfSide},
            {halfSide, -halfSide, -halfSide},
            {halfSide, halfSide, -halfSide},
            {-halfSide, halfSide, -halfSide},
            {-halfSide, -halfSide, halfSide},
            {halfSide, -halfSide, halfSide},
            {halfSide, halfSide, halfSide},
            {-halfSide, halfSide, halfSide}};

        // 生成法线 (简化处理，每个面一个法线)
        normals = {
            {0.0f, 0.0f, -1.0f},
            {0.0f, 0.0f, 1.0f},
            {1.0f, 0.0f, 0.0f},
            {-1.0f, 0.0f, 0.0f},
            {0.0f, 1.0f, 0.0f},
            {0.0f, -1.0f, 0.0f}};

        // 生成索引 (6个面，每个面2个三角形)
        indices = {
            0, 1, 2, 2, 3, 0, // 前面
            4, 5, 6, 6, 7, 4, // 后面
            1, 5, 6, 6, 2, 1, // 右面
            0, 4, 7, 7, 3, 0, // 左面
            3, 2, 6, 6, 7, 3, // 上面
            0, 1, 5, 5, 4, 0  // 下面
        };

        // 使用 scale 来调整立方体大小
        SetScale(Eigen::Vector3f(sideLength, sideLength, sideLength));
    }
};