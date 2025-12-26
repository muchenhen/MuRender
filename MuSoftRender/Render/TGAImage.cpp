#include "TGAImage.h"
#include <iostream>
#include <fstream>

#pragma pack(push, 1)
struct TGAHeader
{
    uint8_t idlength;
    uint8_t colormaptype;
    uint8_t datatypecode;
    uint16_t colormaporigin;
    uint16_t colormaplength;
    uint8_t colormapdepth;
    uint16_t x_origin;
    uint16_t y_origin;
    uint16_t width;
    uint16_t height;
    uint8_t bitsperpixel;
    uint8_t imagedescriptor;
};
#pragma pack(pop)

TGAImage::TGAImage(int w, int h)
{
    width = w;
    height = h;
    data = new unsigned char[width * height * TGAFormat];
}

TGAImage::TGAImage(const TGAImage& other)
{
    width = other.width;
    height = other.height;
    data = new unsigned char[width * height * TGAFormat];
    std::memcpy(data, other.data, static_cast<size_t>(width) * height * TGAFormat);
}

TGAImage::~TGAImage()
{
    delete[] data;
    data = nullptr;
}

TGAImage& TGAImage::operator=(const TGAImage& other)
{
    if (this == &other) return *this; // 自赋值检查
    size_t newSize = static_cast<size_t>(other.width) * other.height * TGAFormat;
    unsigned char* newData = nullptr;

    if (newSize > 0)
    {
        newData = new unsigned char[newSize];
        if (other.data)
            std::memcpy(newData, other.data, newSize);
        else
            std::memset(newData, 0, newSize);
    }

    // 成功分配并复制后才释放旧资源
    delete[] data;
    data = newData;
    width = other.width;
    height = other.height;

    return *this;
}

bool TGAImage::set(int x, int y, const TGAColor& c)
{
    if (x < 0 || x >= width || y < 0 || y >= height)
        return false;
    const size_t idx = (static_cast<size_t>(y) * width + x) * TGAFormat;
    // 存储顺序为 B, G, R, A（TGA 常见顺序）
    data[idx + 0] = c.b;
    data[idx + 1] = c.g;
    data[idx + 2] = c.r;
    data[idx + 3] = c.a;
    return true;
}

TGAColor TGAImage::get(int x, int y) const
{
    TGAColor c{0, 0, 0, 0};
    if (x < 0 || x >= width || y < 0 || y >= height) return c;
    const size_t idx = (static_cast<size_t>(y) * width + x) * TGAFormat;
    c.b = data[idx + 0];
    c.g = data[idx + 1];
    c.r = data[idx + 2];
    c.a = data[idx + 3];
    return c;
}

bool TGAImage::write_tga_file(const std::string& filename, bool flip_vertically)
{
    if (!data || width <= 0 || height <= 0) return false;
    TGAHeader header{};
    header.idlength = 0;
    header.colormaptype = 0;
    header.datatypecode = 2; // uncompressed true-color image
    header.colormaporigin = 0;
    header.colormaplength = 0;
    header.colormapdepth = 0;
    header.x_origin = 0;
    header.y_origin = 0;
    header.width = static_cast<uint16_t>(width);
    header.height = static_cast<uint16_t>(height);
    header.bitsperpixel = static_cast<uint8_t>(TGAFormat * 8); // 32
    // bit5 (0x20) 控制图像原点：1 = top-left, 0 = bottom-left
    header.imagedescriptor = flip_vertically ? 0x20 : 0x00;

    std::ofstream ofs(filename, std::ios::binary);
    if (!ofs) return false;
    // 写入头部
    ofs.write(reinterpret_cast<const char*>(&header), sizeof(header));
    if (!ofs) return false;

    const size_t rowBytes = static_cast<size_t>(width) * TGAFormat;
    // 写入像素数据：按行写，支持垂直翻转
    if (flip_vertically)
    {
        for (int y = height - 1; y >= 0; --y)
        {
            const unsigned char* row = data + static_cast<size_t>(y) * rowBytes;
            ofs.write(reinterpret_cast<const char*>(row), rowBytes);
            if (!ofs) return false;
        }
    }
    else
    {
        ofs.write(reinterpret_cast<const char*>(data), static_cast<std::streamsize>(rowBytes) * height);
        if (!ofs) return false;
    }

    ofs.close();
    return true;
}

void TGAImage::clear()
{
    std::memset(data, 0, static_cast<size_t>(width) * height * TGAFormat);
}

int TGAImage::get_width() const
{
    return width;
}

int TGAImage::get_height() const
{
    return height;
}

unsigned char* TGAImage::buffer()
{
    return data;
}
