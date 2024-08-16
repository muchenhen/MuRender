#pragma once
#include "Shader.h"

class Scene;

class Renderer
{
public:
    Renderer();
    Renderer(int width, int height);

    ~Renderer();

    void Initialize();
    void CleanUp();
    void Render();
    void Clear();
    void SetViewport(int x, int y, int width, int height);
    
    void Render(const Scene& scene, const Shader& shader);

private:
    glm::mat4 projection;
};