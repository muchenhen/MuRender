#pragma once
#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"
#include "Camera.h"
#include <vector>

class Scene {
public:
    Scene();
    void Update(float deltaTime);
    void Draw(const Shader& shader);

    Camera& GetCamera();

private:
    std::vector<Mesh> objects;
    Camera camera;
    unsigned int texture;
};