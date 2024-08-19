#pragma once
#include <glad/glad.h>
#include <vector>
#include "Shader.h"

struct Vertex {
    float position[3];
    float color[3];
    float texCoords[2];
};

class Mesh {
public:
    Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);
    ~Mesh();

    void Draw(const Shader& shader) const;

    static Mesh CreateCube();
private:
    unsigned int VAO, VBO, EBO;
    size_t indexCount;

    void setupMesh();
};