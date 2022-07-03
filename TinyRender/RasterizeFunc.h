#pragma once
#include "Primitive.h"
#include "TGAImage.h"
#include "OtherFunc.h"
#include <math.h>
#include <iostream>
#include "Model.h"

// 定义TGA颜色 白色
const TGAColor WHITE = TGAColor(255, 255, 255, 255);
// 定义TGA颜色 红色
const TGAColor RED = TGAColor(255, 0, 0, 255);

const TGAColor GREEN = TGAColor(0, 255, 0);

constexpr int WIDTH = 800;
constexpr int HEIGHT = 800;

//数值微分画线
template <class T>
void DrawLineDDA(const Point<T>& P1, const Point<T>& P2, TGAImage& Image, const TGAColor& Color)
{
    if (P1.x == P2.x)
        return;
    float k = fabs((P2.y - P1.y) / P2.x - P1.x);

    float x = P1.x;
    float y = P1.y;

    if (0 < k <= 1)
    {
        k = k;
    }
    else
    {
        k = 1 / k;
        std::swap(x, y);
    }

    for (; x <= P2.x; x++)
    {
        auto realY = floor(y + 0.5);
        Image.set(x, realY, Color);
        std::cout << x << " " << realY << std::endl;
        y = y + k;
    }
}

//中点画线
template <class T>
void DrawLineMidPoint(const Point<T>& P1, const Point<T>& P2, TGAImage& Image, const TGAColor& Color)
{
    const int A = P1.y - P2.y;
    const int B = P2.x - P1.x;
    const int C = P2.x * P1.y - P1.x * P2.y;
    int d = A + A + B;

    int x = floor(P1.x);
    int y = floor(P1.y);

    float deltaD;

    for (; x <= P2.x; x++)
    {
        Image.set(x, y, Color);
        std::cout << "x: " << x << " ,y: " << y << " ,d: " << d << std::endl;
        if (d < 0)
        {
            deltaD = A + B;
        }
        else
        {
            deltaD = A;
        }
        d += deltaD;

        if (d < 0)
        {
            y++;
        }
        else
        {
            y = y;
        }
    }
}

// Bresenham画线
template <class T>
void DrawLineBresenham(const Point<T>& P1, const Point<T>& P2, TGAImage& Image, const TGAColor& Color)
{
    // const int dx = round(P2.x - P1.x);
    // const int dy = round(P2.y - P1.y);
    // int g = -dx;
    // int x = round(P1.x);
    // int y = round(P1.y);
    // for (; x <= P2.x; x++)
    // {
    // 	Image.set(x, y, Color);
    // 	std::cout << "x: " << x << " ,y: " << y << std::endl;
    // 	g = g + dy + dy;
    // 	if (g >= 0)
    // 	{
    // 		y++;
    // 		g = g - dx - dx;
    // 	}
    // }
    T x0 = P1.X;
    T y0 = P1.Y;
    T x1 = P2.X;
    T y1 = P2.Y;
    bool steep = false;
    if (std::abs(x0 - x1) < std::abs(y0 - y1))
    {
        std::swap(x0, y0);
        std::swap(x1, y1);
        steep = true;
    }
    if (x0 > x1)
    {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }
    int dx = x1 - x0;
    int dy = y1 - y0;
    int derror2 = std::abs(dy) * 2;
    int error2 = 0;
    int y = y0;
    for (int x = x0; x <= x1; ++x)
    {
        if (steep)
        {
            Image.set(y, x, Color);
        }
        else
        {
            Image.set(x, y, Color);
        }
        std::cout << "x: " << x << " ,y: " << y << std::endl;
        error2 += derror2;
        if (error2 > dx)
        {
            y += (y1 > y0 ? 1 : -1);
            error2 -= dx * 2;
        }
    }
}

// 第二次尝试
// void SecondLine(const Point& P1, const Point& P2, TGAImage& image, TGAColor color) {
//	int x0 = P1.x;
//	int y0 = P1.y;
//	int x1 = P2.x;
//	int y1 = P2.y;
//	for (int x = x0; x <= x1; x++) {
//		float t = (x - x0) / (float)(x1 - x0);
//		int y = y0 * (1. - t) + y1 * t;
//		image.set(x, y, color);
//		std::cout << "x: " << x << " ,y: " << y << std::endl;
//	}
//	std::cout << std::endl;
//}

void DrawWireframe(Model* model, TGAImage& image, const TGAColor& color)
{
    for (Face& face : model->GetFaces())
    {
        // 每个面的1-2,2-3,1-3的线画一下
        for (int i = 0; i < 3; i++)
        {
            Point<float> p1 = model->GetVertex(face[i]).ToPoint2D();
            Point<float> p2 = model->GetVertex(face[(i + 1) % 3]).ToPoint2D();
            p1.X = (p1.X + 1) * (image.get_width() / 2);
            p2.X = (p2.X + 1) * (image.get_width() / 2);
            p1.Y = (p1.Y + 1) * (image.get_height() / 2);
            p2.Y = (p2.Y + 1) * (image.get_height() / 2);

            DrawLineBresenham(p1, p2, image, color);
        }
    }
}

void DrawTriangle(Point<float> A, Point<float> B, Point<float> C, TGAImage& Image, const TGAColor& Color)
{
    DrawLineBresenham(A, B, Image, Color);
    DrawLineBresenham(A, C, Image, Color);
    DrawLineBresenham(B, C, Image, Color);
}

void DrawFilledTriangle(Vec2i T0, Vec2i T1, Vec2i T2, TGAImage& Image, const TGAColor Color)
{
    if (T0.y == T1.y && T0.y == T2.y) return; // 这能叫三角形啊（暴言
    // 给点按照y排序可以方便的确认高度
    if (T0.y > T1.y) std::swap(T0, T1);
    if (T0.y > T2.y) std::swap(T0, T2);
    if (T1.y > T2.y) std::swap(T1, T2);
    const int TotalHeight = T2.y - T0.y;
    for (int i = 0; i < TotalHeight; i++)
    {
        const bool SecondHalf = i > T1.y - T0.y || T1.y == T0.y;
        const int SegmentHeight = SecondHalf ? T2.y - T1.y : T1.y - T0.y;
        const float Alpha = static_cast<float>(i) / TotalHeight;
        const float Beta = static_cast<float>(i - (SecondHalf ? T1.y - T0.y : 0)) / SegmentHeight;
        Vec2i A = T0 + (T2 - T0) * Alpha;
        Vec2i B = SecondHalf ? T1 + (T2 - T1) * Beta : T0 + (T1 - T0) * Beta;
        if (A.x > B.x) std::swap(A, B);
        for (int j = A.x; j <= B.x; j++)
        {
            Image.set(j, T0.y + i, Color); // 需要注意int型可能导致的 t0.y+i != A.y
        }
    }
}

Vec3f Barycentric(const Vec3f Pts[3], Vec3f P)
{
    const Vec3f U =
        Vec3f(Pts[2].x - Pts[0].x, Pts[1].x - Pts[0].x, Pts[0].x - P.x) ^ Vec3f(Pts[2].y - Pts[0].y, Pts[1].y - Pts[0].y, Pts[0].y - P.y);
    if (std::abs(U.z) < 1)
        return {-1, 1, 1};
    return {1.f - (U.x + U.y) / U.z, U.y / U.z, U.x / U.z};
}

void DrawTriangleEdgeFunc(Vec3f Pts[3], TGAImage& Image, const TGAColor Color, float* ZBuffer)
{
    Vec2f BboxMin(Image.get_width() - 1, Image.get_height() - 1);
    Vec2f BboxMax(0, 0);
    const Vec2f Clamp(Image.get_width() - 1, Image.get_height() - 1);
    for (int i = 0; i < 3; i++)
    {
        BboxMin.x = std::max(0.0f, std::min(BboxMin.x, Pts[i].x));
        BboxMin.y = std::max(0.0f, std::min(BboxMin.y, Pts[i].y));

        BboxMax.x = std::min(Clamp.x, std::max(BboxMax.x, Pts[i].x));
        BboxMax.y = std::min(Clamp.y, std::max(BboxMax.y, Pts[i].y));
    }
    Vec3f P;
    for (P.x = BboxMin.x; P.x <= BboxMax.x; P.x++)
    {
        for (P.y = BboxMin.y; P.y <= BboxMax.y; P.y++)
        {
            Vec3<float> BcScreen = Barycentric(Pts, P);
            if (BcScreen.x < 0 || BcScreen.y < 0 || BcScreen.z < 0)
                continue;
            P.z = 0;
            for (int i = 0; i < 3; i++)
            {
                P.z += Pts[i][2] * BcScreen[i];
            }
            if (ZBuffer[static_cast<int>(P.x + P.y * WIDTH)] < P.z)
            {
                ZBuffer[static_cast<int>(P.x + P.y * WIDTH)] = P.z;
                Image.set(P.x, P.y, Color);
            }
        }
    }
}

// 从贴图采样颜色
void DrawTriangleEdgeFunc(Vec3f Pts[3], Vec3f UVs[3], TGAImage& Image, TGAImage& TextureImage, float* ZBuffer, const float& Intensity)
{
    Vec3f t0, t1, t2, uv0, uv1, uv2;
    t0 = (Pts[0]);
    t1 = (Pts[1]);
    t2 = (Pts[2]);
    uv0 = (UVs[0]);
    uv1 = (UVs[1]);
    uv2 = (UVs[2]);

    if (t0.y == t1.y && t0.y == t2.y) return; // i dont care about degenerate triangles
    if (t0.y > t1.y)
    {
        std::swap(t0, t1);
        std::swap(uv0, uv1);
    }
    if (t0.y > t2.y)
    {
        std::swap(t0, t2);
        std::swap(uv0, uv2);
    }
    if (t1.y > t2.y)
    {
        std::swap(t1, t2);
        std::swap(uv1, uv2);
    }

    int total_height = t2.y - t0.y;
    for (int i = 0; i < total_height; i++)
    {
        bool second_half = i > t1.y - t0.y || t1.y == t0.y;
        int segment_height = second_half ? t2.y - t1.y : t1.y - t0.y;
        float alpha = (float)i / total_height;
        float beta = (float)(i - (second_half ? t1.y - t0.y : 0)) / segment_height; 
        Vec3f A = t0 + Vec3f(t2 - t0) * alpha;
        Vec3f B = second_half ? t1 + Vec3f(t2 - t1) * beta : t0 + Vec3f(t1 - t0) * beta;
        // 通过插值计算每个点的uv
        Vec3f uvA = uv0 + (uv2 - uv0) * alpha;
        Vec3f uvB = second_half ? uv1 + (uv2 - uv1) * beta : uv0 + (uv1 - uv0) * beta;
        if (A.x > B.x)
        {
            std::swap(A, B);
            std::swap(uvA, uvB);
        }
        for (int j = A.x; j <= B.x; j++)
        {
            float phi = B.x == A.x ? 1. : (float)(j - A.x) / (float)(B.x - A.x);
            Vec3f P = Vec3f(A) + Vec3f(B - A) * phi;
            Vec3f uvP = uvA + (uvB - uvA) * phi;
            int idx = P.x + P.y * WIDTH;
            if (ZBuffer[idx] < P.z)
            {
                ZBuffer[idx] = P.z;
                TGAColor color = TextureImage.get(uvP.x, uvP.y) * Intensity;
                Image.set(P.x, P.y, color);
            }
        }
    }
}

// 简单的从世界转到屏幕空间的转换
Vec3f WorldToScreen(const Vec3f& V)
{
    return Vec3f(int((V.x + 1.) * WIDTH / 2. + .5), int((V.y + 1.) * HEIGHT / 2. + .5), V.z);
}

Vec3i WorldToScreen(const Vec3i& V)
{
    return Vec3i(int((V.x + 1.) * WIDTH / 2. + .5), int((V.y + 1.) * HEIGHT / 2. + .5), V.z);
}