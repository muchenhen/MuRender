#pragma once
#include "Primitive.h"
#include "TGAImage.h"
#include "OtherFunc.h"
#include <math.h>
#include <iostream>

//数值微分画线算法
void DrawLineDDA(const Point& P1, const Point& P2, TGAImage& Image, const TGAColor& Color)
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
		Swap(x, y);
	}

	for (; x <= P2.x; x++)
	{
		auto realY = floor(y + 0.5);
		Image.set(x, realY, Color);
		std::cout << x << " " << realY << std::endl;
		y = y + k;
	}
}

//中点画线算法
void DrawLineMidPoint(const Point& P1, const Point& P2, TGAImage& Image, const TGAColor& Color)
{
	const float A = P1.y - P2.y;
	const float B = P2.x - P1.x;
	const float C = P2.x * P1.y - P1.x * P2.y;
	int d = 2 * A + B;

	float x = P1.x;
	float y = P1.y;

	float deltaD;

	for (; x <= P2.x; x++)
	{
		auto realY = floor(y + 0.5);
		Image.set(x, realY, Color);
		std::cout << x << " " << realY << std::endl;

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