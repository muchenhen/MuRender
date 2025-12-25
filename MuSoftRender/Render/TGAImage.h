#pragma once
#include <string>

const int TGAFormat = 4;

struct TGAColor
{
    unsigned char r, g, b, a;
};

class TGAImage
{
public:
    TGAImage(int w, int h); // bpp: Bytes Per Pixel
    ~TGAImage();

    // 基础操作
    bool set(int x, int y, const TGAColor& c); // 设置像素
    TGAColor get(int x, int y) const;          // 获取像素（用于后期混合）

    // 核心功能：写入文件
    bool write_tga_file(const std::string& filename, bool flip_vertically = true);

    // 辅助功能
    void clear(); // 清空画布（背景色）
    int get_width() const;
    int get_height() const;
    unsigned char* buffer(); // 返回原始指针，方便 memset 或多线程操作

private:
    int width;
    int height;
    unsigned char* data; 
};
