#pragma once

class Renderer
{
public:
    Renderer();
    ~Renderer();

    void Initialize();
    void CleanUp();
    void Render();
    void Clear();
    void SetViewport(int x, int y, int width, int height);

public:
    
};
