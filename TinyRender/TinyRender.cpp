#include "TGAImage.h"
#include "RasterizeFunc.h"

// 定义TGA颜色 白色
const TGAColor white = TGAColor(255, 255, 255, 255);
// 定义TGA颜色 红色
const TGAColor red = TGAColor(255, 0, 0, 255);

int main(int argc, char** argv) {
    // 创建一个新的TGA对象，长100宽100的RGB格式图像
    TGAImage image(100, 100, TGAImage::RGB);
    // 将图片垂直翻转，这样子原点会变成左下角
    image.flip_vertically();
    Point P1{ 13, 20 };
    Point P2{ 80, 40 };
    SecondLine(P1, P2, image, white);
    P1 = { 20, 13 };
    P2 = { 40, 80 };
    SecondLine(P1, P2, image, white);
    P1 = { 80, 40 };
    P2 = { 13, 20 };
    SecondLine(P1, P2, image, white);

    //DrawLineBresenham(P1, P2, image, white);

    // 将image写入文件output.tga
    //image.write_tga_file("DrawLineBresenham.tga");
    return 0;
}