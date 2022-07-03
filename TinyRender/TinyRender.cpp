#include "TGAImage.h"
#include "RasterizeFunc.h"
#include "Maths.h"
#include "Model.h"
#include <memory>

int main()
{
    Model* ModelInstance = new Model("obj/african_head.obj");

    TGAImage Image(WIDTH, HEIGHT, TGAImage::RGB);
    Image.flip_vertically();

    // 读取贴图
    TGAImage Texture(1024, 1024, TGAImage::RGB);
    Texture.read_tga_file("african_head_diffuse.tga");
    Texture.flip_vertically();


    // 初始化ZBuffer
    std::shared_ptr<float> ZBuffer(new float[WIDTH * HEIGHT], std::default_delete<float[]>());
    for (int i = 0; i < (WIDTH * HEIGHT); i++)
    {
        ZBuffer.get()[i] = std::numeric_limits<int>::min();
    }

    // 定义光照方向
    const Vec3f LightDir(0.f, 0.f, -1.f);

    for (int i = 0; i < ModelInstance->GetFacesNumber(); i++)
    {
        Face Face = ModelInstance->GetFace(i);
        Vec3f Pts[3];
        Vec3f UVs[3];
        Vec3f WorldCoords[3];
        // 处理每个面的三个点的空间变换
        for (int j = 0; j < 3; j++)
        {
            const Vec3f V = ModelInstance->GetVertex(Face[j]);
            //Pts[j] = (WorldToScreen(V));
            Pts[j] = WorldToScreen(V);

            UVs[j] = Vec3f(ModelInstance->GetUV(Face.VTIndex[j]).x * 1024, 
                ModelInstance->GetUV(Face.VTIndex[j]).y * 1024, 
                0);
            WorldCoords[j] = V;
        }
        Vec3f N = (WorldCoords[2] - WorldCoords[0]) ^ (WorldCoords[1] - WorldCoords[0]);
        N.normalize();
        const float Intensity = N * LightDir;
        DrawTriangleEdgeFunc(Pts, UVs, Image, Texture, ZBuffer.get(), Intensity);
        //DrawTriangleEdgeFunc(Pts, Image, TGAColor(Intensity * 255, Intensity * 255, Intensity * 255, 255), ZBuffer.get());
        printf("\r %f [%.2lf%%]", Intensity, i * 100.0f / ModelInstance->GetFacesNumber());
    }

    Image.write_tga_file("lesson3diffuse.tga");

    delete ModelInstance;

    return 0;
}