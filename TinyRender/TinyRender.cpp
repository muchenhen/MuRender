#include "TGAImage.h"
#include "RasterizeFunc.h"
#include "Maths.h"
#include "Model.h"

using namespace std;

// 定义TGA颜色 白色
const TGAColor WHITE = TGAColor(255, 255, 255, 255);
// 定义TGA颜色 红色
const TGAColor RED = TGAColor(255, 0, 0, 255);

const TGAColor GREEN = TGAColor(0, 255, 0);

constexpr int WIDTH = 800;
constexpr int HEIGHT = 800;

int main() 
{
    Model* ModelInstance = new Model("obj/african_head.obj");

    TGAImage Image(WIDTH, HEIGHT, TGAImage::RGB);
    Image.flip_vertically();

    // 定义光照方向
    Vec3f LightDir(0, 0, -1);
    for (int i = 0; i < ModelInstance->GetFacesNumber(); i++)
    {
        Face Face = ModelInstance->GetFace(i);
        Vec2i ScreenCoords[3];
        Vec3f WorldCoords[3];
        for (int j = 0; j < 3; j++)
        {
            const Vec3f V = ModelInstance->GetVertex(Face[j]);
            ScreenCoords[j] = Vec2i((V.x + 1.) * WIDTH / 2., (V.y + 1.) * HEIGHT / 2.);
            WorldCoords[j] = V;
        }
        // 这里三角形的法线可以简单地计算为其两侧的叉积
        // 两个向量的叉积与这两个向量组成的坐标平面垂直
        Vec3f N = (WorldCoords[2] - WorldCoords[0]) ^ (WorldCoords[1] - WorldCoords[0]);
        N.normalize();
        const float Intensity = N * LightDir;
        // 如果Intensity是0说明光照和法向量平行，就照不到光，小于0就意味着光是从平面法向量背面照射的
        if (Intensity > 0)
        {
            Vec2i Pts[3] = {ScreenCoords[0], ScreenCoords[1], ScreenCoords[2]};
            DrawTriangleEdgeFunc(Pts, Image, TGAColor(Intensity * 255, Intensity * 255, Intensity * 255, 255));
        }
    }

    Image.write_tga_file("lesson2triangleedge.tga");

    delete ModelInstance;

    return 0;
}