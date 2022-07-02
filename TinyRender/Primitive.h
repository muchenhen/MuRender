#pragma once

template<class T> 
struct Point
{
	T X;
	T Y;

	Point(T InX, T InY)
	{
		X = InX;
		Y = InY;
	}

	Point<T> operator + (const T& P)
	{
		return Point<T>(P.X + X, P.Y + Y);
	}

	Point<T> operator - (const T& P)
	{
		return Point<T>(P.X - X, P.Y - Y);
	}
};