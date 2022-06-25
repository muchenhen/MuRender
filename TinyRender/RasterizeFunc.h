#pragma once
#include "Primitive.h"
#include "TGAImage.h"
#include "OtherFunc.h"
#include <math.h>
#include <iostream>
#include "Model.h"

//数值微分画线
template<class T>
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
template<class T>
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
template<class T>
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
	T x0 = P1.x;
	T y0 = P1.y;
	T x1 = P2.x;
	T y1 = P2.y;
	bool steep = false;
	if (std::abs(x0 - x1) < std::abs(y0 - y1)) {
		std::swap(x0, y0);
		std::swap(x1, y1);
		steep = true;
	}
	if (x0 > x1) {
		std::swap(x0, x1);
		std::swap(y0, y1);
	}
	int dx = x1 - x0;
	int dy = y1 - y0;
	int derror2 = std::abs(dy) * 2;
	int error2 = 0;
	int y = y0;
	for (int x = x0; x <= x1; ++x) {
		if (steep) {
			Image.set(y, x, Color);
		}
		else {
			Image.set(x, y, Color);
		}
		std::cout << "x: " << x << " ,y: " << y << std::endl;
		error2 += derror2;
		if (error2 > dx) {
			y += (y1 > y0 ? 1 : -1);
			error2 -= dx * 2;
		}
	}
}

// 第二次尝试
//void SecondLine(const Point& P1, const Point& P2, TGAImage& image, TGAColor color) {
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
			p1.x = (p1.x + 1) * (image.get_width() / 2);
			p2.x = (p2.x + 1) * (image.get_width() / 2);
			p1.y = (p1.y + 1) * (image.get_height() / 2);
			p2.y = (p2.y + 1) * (image.get_height() / 2);

			DrawLineBresenham(p1, p2, image, color);
		}
	}
}