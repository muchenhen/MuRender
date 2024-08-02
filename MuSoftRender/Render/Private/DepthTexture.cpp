#include "DepthTexture.h"

#include <iostream>

DepthTexture::DepthTexture(int InWidth, int InHeight):
    Width(InWidth), Height(InHeight)
{
    Data.resize(static_cast<size_t>(Width) * Height, 1.0f);
}

void DepthTexture::SetDepth(int x, int y, float InDepth)
{
    if (x >= 0 && x < Width && y >= 0 && y < Height)
    {
        Data[y * Width + x] = InDepth;
    }
}

float DepthTexture::GetDepth(int x, int y) const
{
    if (x >= 0 && x < Width && y >= 0 && y < Height)
    {
        return Data[y * Width + x];
    }
    return 1.0f;
}

int DepthTexture::GetWidth() const
{
    return Width;
}

int DepthTexture::GetHeight() const
{
    return Height;
}

void DepthTexture::SaveDepthTextureToBMP(const DepthTexture* depthTexture, const std::string& filename)
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
            uint8_t r, g, b;

            if (depth >= 0.0f && depth <= 1.0f)
            {
                // Valid depth, map to grayscale
                uint8_t gray = static_cast<uint8_t>(depth * 255);
                r = g = b = gray;
            }
            else if (depth < 0.0f)
            {
                // Invalid depth, mark as blue
                r = g = 0;
                b = 255;
            }
            else
            {
                // Invalid depth, mark as red
                r = 255;
                g = b = 0;
            }

            int index = 54 + (y * width + x) * 3;
            img[index] = b;
            img[index + 1] = g;
            img[index + 2] = r;
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
    float x = u * (Width - 1);
    float y = v * (Height - 1);
    int x0 = std::floor(x);
    int y0 = std::floor(y);
    int x1 = std::min(x0 + 1, Width - 1);
    int y1 = std::min(y0 + 1, Height - 1);

    float fx = x - x0;
    float fy = y - y0;

    float d00 = GetDepth(x0, y0);
    float d10 = GetDepth(x1, y0);
    float d01 = GetDepth(x0, y1);
    float d11 = GetDepth(x1, y1);

    float d0 = d00 * (1 - fx) + d10 * fx;
    float d1 = d01 * (1 - fx) + d11 * fx;

    return d0 * (1 - fy) + d1 * fy;
}