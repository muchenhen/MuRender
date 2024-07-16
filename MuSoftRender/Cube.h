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
        float halfSide = 0.5f; // ����һ����λ�����壬Ȼ��ͨ�� scale ��������С

        // ���֮ǰ������
        vertices.clear();
        normals.clear();
        indices.clear();

        // ���ɶ���
        vertices = {
            {-halfSide, -halfSide, -halfSide},
            {halfSide, -halfSide, -halfSide},
            {halfSide, halfSide, -halfSide},
            {-halfSide, halfSide, -halfSide},
            {-halfSide, -halfSide, halfSide},
            {halfSide, -halfSide, halfSide},
            {halfSide, halfSide, halfSide},
            {-halfSide, halfSide, halfSide}};

        // ���ɷ��� (�򻯴���ÿ����һ������)
        normals = {
            {0.0f, 0.0f, -1.0f},
            {0.0f, 0.0f, 1.0f},
            {1.0f, 0.0f, 0.0f},
            {-1.0f, 0.0f, 0.0f},
            {0.0f, 1.0f, 0.0f},
            {0.0f, -1.0f, 0.0f}};

        // �������� (6���棬ÿ����2��������)
        indices = {
            0, 1, 2, 2, 3, 0, // ǰ��
            4, 5, 6, 6, 7, 4, // ����
            1, 5, 6, 6, 2, 1, // ����
            0, 4, 7, 7, 3, 0, // ����
            3, 2, 6, 6, 7, 3, // ����
            0, 1, 5, 5, 4, 0  // ����
        };

        // ʹ�� scale �������������С
        SetScale(Eigen::Vector3f(sideLength, sideLength, sideLength));
    }
};