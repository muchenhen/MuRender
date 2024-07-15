#pragma once

#include <vector>
#include <cstdint>

class Renderer
{
private:
    int ScreenWidth;
    int ScreenHeight;
    std::vector<uint32_t> FrameBuffer;

public:
    Renderer(int width, int height);

    void Clear(uint32_t color = 0);

    const std::vector<uint32_t>& GetFrameBuffer() const;

    int GetWidth() const;

    int GetHeight() const;


public:
    
};
