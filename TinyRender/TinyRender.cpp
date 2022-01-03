#include "TGAImage.h"
#include "RasterizeFunc.h"
#include "Maths.h"
#include "Model.h"

// 定义TGA颜色 白色
const TGAColor white = TGAColor(255, 255, 255, 255);
// 定义TGA颜色 红色
const TGAColor red = TGAColor(255, 0, 0, 255);

const int width = 800;
const int height = 800;

int main() 
{
    Model* model = new Model("obj/african_head.obj");

    TGAImage image(width, height, TGAImage::RGB);
    image.flip_vertically();

    DrawWireframe(model, image, white);

    image.write_tga_file("lesson1Wireframe.tga");

    delete model;

    return 0;
}