#pragma once
#include "Primitive.h"
#include <math.h>
void DrawLineDDA(const Point& P1, const Point& P2)
{
	if (P1.x == P2.x)
		return;
	float k = fabs((P2.y - P1.y) / P2.x - P1.x);

}