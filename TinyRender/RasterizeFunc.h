#pragma once
#include "Primitive.h"
#include "TGAImage.h"
#include "OtherFunc.h"
#include <math.h>
#include <iostream>

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
	const int dx = P2.x - P1.x;
	const int dy = P2.y - P1.y;
	int g = -dx;
	int x = P1.x;
	int y = P1.y;
	for (; x <= P2.x; x++)
	{
		Image.set(x, y, Color);
		std::cout << "x: " << x << " ,y: " << y << std::endl;
		g = g + dy + dy;
		if (g >= 0)
		{
			y++;
			g = g - dx - dx;
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

void DrawWireframe()
{

}