#include "DepthTexture.h"

#include <iostream>

DepthTexture::DepthTexture(int InWidth, int InHeight):
    Width(InWidth), Height(InHeight)
{
    DepthBuffer.resize(static_cast<size_t>(Width) * Height, 1.0f);
}

void DepthTexture::Clear()
{
    std::fill(DepthBuffer.begin(), DepthBuffer.end(), INVALID_DEPTH);
}

void DepthTexture::SetDepth(int x, int y, float InDepth)
{
    if (x >= 0 && x < Width && y >= 0 && y < Height)
    {
        int index = y * Width + x;
        if (InDepth >= 0.0f && InDepth <= 1.0f && InDepth < DepthBuffer[index])
        {
            DepthBuffer[index] = InDepth;
        }
    }
}

float DepthTexture::GetDepth(int x, int y) const
{
    if (x >= 0 && x < Width && y >= 0 && y < Height)
    {
        return DepthBuffer[y * Width + x];
    }
    return INVALID_DEPTH;
}

int DepthTexture::GetWidth() const
{
    return Width;
}

int DepthTexture::GetHeight() const
{
    return Height;
}

void DepthTexture::RotateDepthBuffer90DegreesCounterClockwise()
{
    std::vector<float> rotatedBuffer(DepthBuffer.size());
    
    for (int y = 0; y < Height; ++y)
    {
        for (int x = 0; x < Width; ++x)
        {
            // 计算原始索引
            int originalIndex = y * Width + x;
            
            // 计算旋转后的新位置
            int newX = Height - 1 - y;
            int newY = x;
            
            // 计算新索引
            int newIndex = newY * Height + newX;
            
            // 将原始数据复制到新位置
            rotatedBuffer[newIndex] = DepthBuffer[originalIndex];
        }
    }
    
    // 交换宽度和高度
    std::swap(Width, Height);
    
    // 用旋转后的数据替换原始数据
    DepthBuffer = std::move(rotatedBuffer);
}

void DepthTexture::SaveDepthTextureToBMP(DepthTexture* depthTexture, const std::string& filename)
{
    int width = depthTexture->GetWidth();
    int height = depthTexture->GetHeight();

    // BMP file header
    uint32_t fileSize = 54 + 3 * width * height;
    uint32_t dataOffset = 54;

    std::vector<uint8_t> img(fileSize);

    // File header
    img[0] = 'B';
    img[1] = 'M';
    *reinterpret_cast<uint32_t*>(&img[2]) = fileSize;
    *reinterpret_cast<uint32_t*>(&img[10]) = dataOffset;

    // Image header
    *reinterpret_cast<uint32_t*>(&img[14]) = 40;
    *reinterpret_cast<int32_t*>(&img[18]) = width;
    *reinterpret_cast<int32_t*>(&img[22]) = -height; // Negative for top-down
    *reinterpret_cast<uint16_t*>(&img[26]) = 1;
    *reinterpret_cast<uint16_t*>(&img[28]) = 24; // 24 bits per pixel
    *reinterpret_cast<uint32_t*>(&img[30]) = 0;  // No compression
    *reinterpret_cast<uint32_t*>(&img[34]) = 3 * width * height;
    *reinterpret_cast<uint32_t*>(&img[38]) = 2835; // 72 DPI
    *reinterpret_cast<uint32_t*>(&img[42]) = 2835; // 72 DPI

    // Write pixel data
    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            float depth = depthTexture->GetDepth(x, y);
            uint8_t color;

            if (depth >= 0.0f && depth <= 1.0f)
            {
                color = static_cast<uint8_t>(depth * 255);
            }
            else if (depth < 0.0f)
            {
                // 无效的过近深度，标记为黑色
                color = 0;
            }
            else
            {
                // 无效的过远深度，标记为白色
                color = 255;
            }

            int index = 54 + (y * width + x) * 3;
            img[index] = color;     // B
            img[index + 1] = color; // G
            img[index + 2] = color; // R
        }
    }

    // Save to file
    std::ofstream file(filename, std::ios::binary);
    if (file.is_open())
    {
        file.write(reinterpret_cast<char*>(img.data()), img.size());
        file.close();
    }
    else
    {
        // Handle error: unable to open file
        std::cerr << "Error: Unable to open file for writing: " << filename << std::endl;
    }
}

float DepthTexture::SampleDepth(float u, float v) const
{
    int x = static_cast<int>(u * (Width - 1) + 0.5f);
    int y = static_cast<int>(v * (Height - 1) + 0.5f);
    x = std::clamp(x, 0, Width - 1);
    y = std::clamp(y, 0, Height - 1);

    float depth = GetDepth(x, y);
    if (depth == INVALID_DEPTH)
    {
        // 返回一个表示"不在阴影中"的值
        return 0.0f;
    }
    return depth;
}