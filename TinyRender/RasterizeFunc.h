#pragma once
#include "Primitive.h"
#include "TGAImage.h"
#include "OtherFunc.h"
#include <math.h>
#include <iostream>
#include "Model.h"

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

Vec3f Barycentric(const Vec2i Pts[3], Vec2i P)
{
    const Vec3f U =
        Vec3f(Pts[2].x - Pts[0].x,Pts[1].x - Pts[0].x,Pts[0].x - P.x) ^ Vec3f(Pts[2].y - Pts[0].y,Pts[1].y - Pts[0].y,Pts[0].y - P.y);
    if (std::abs(U.z) < 1)
        return {-1, 1, 1};
    return {1.f - (U.x + U.y) / U.z, U.y / U.z, U.x / U.z};
}

void DrawTriangleEdgeFunc(Vec2i Pts[3], TGAImage& Image, const TGAColor Color)
{
    Vec2i BboxMin(Image.get_width() - 1, Image.get_height() - 1);
    Vec2i BboxMax(0, 0);
    const Vec2i Clamp(Image.get_width() - 1, Image.get_height() - 1);
    for (int i = 0; i < 3; i++)
    {
        BboxMin.x = std::max(0, std::min(BboxMin.x, Pts[i].x));
        BboxMin.y = std::max(0, std::min(BboxMin.y, Pts[i].y));

        BboxMax.x = std::min(Clamp.x, std::max(BboxMax.x, Pts[i].x));
        BboxMax.y = std::min(Clamp.y, std::max(BboxMax.y, Pts[i].y));
    }
    Vec2i P;
    for (P.x = BboxMin.x; P.x <= BboxMax.x; P.x++)
    {
        for (P.y = BboxMin.y; P.y <= BboxMax.y; P.y++)
        {
            Vec3f BcScreen = Barycentric(Pts, P);
            if (BcScreen.x < 0 || BcScreen.y < 0 || BcScreen.z < 0) continue;
            Image.set(P.x, P.y, Color);
        }
    }
}