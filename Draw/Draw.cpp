#include "Device.h"

void Device::drawPoint(int x, int y, const unsigned int color, const float z)
{
	if (x >= 0 && x < width && y >= 0 && y < height)
	{
		pointFrameBuffer[width * y + x] = color;
		zBuffer[width * y + x] = z; // write z buffer;
	}
}

void Device::drawLine(int x0, int y0, int x1, int y1, const Vector4& color)
{
	int dx = x1 - x0;
	int dy = y1 - y0;
	int ux = (dx > 0) ? 1 : -1;//x方向步进值
	int uy = (dy > 0) ? 1 : -1;//y方向步进值
	dx = abs(x1 - x0);
	dy = abs(y1 - y0);
	if (dx > dy)
	{
		int e = 2 * dy - dx;
		int y = y0;
		for (int x = x0; x != x1; x += ux)
		{
			drawPoint(x, y, MathSet::color2UINT(color), 0);
			e += dy;
			if (e > 0)
			{
				y += uy;
				e -= dx;
			}
		}
	}
	else
	{
		int e = 2 * dx - dy;
		int x = x0;
		for (int y = y0; y != y1; y += uy)
		{
			drawPoint(x, y, MathSet::color2UINT(color), 0);
			e += dx;
			if (e >= 0)
			{
				x += ux;
				e -= dy;
			}
		}
	}
}

void Device::drawLine(const Vector4& p0, const Vector4& p1, const Vector4& color)
{
	int x0 = (int)floor(p0.x);
	int x1 = (int)floor(p1.x);
	int y0 = (int)floor(p0.y);
	int y1 = (int)floor(p1.y);
	drawLine(x0, y0, x1, y1, color);
}

bool Device::triangleCheck(const Vertex& v0, const Vertex& v1, const Vertex& v2, Vertex& p, Vector4& Vw)
{
	// lambda0,lambda1,lambda2 stored in Vector weight,multiply 1 / Z which stored in pos.w
	//注意三个顶点的传入顺序将影响颜色分量的位置分布	
	//float w0 = MathSet::EdgeFunc(v1.pos, v2.pos, p.pos);
	//float w1 = MathSet::EdgeFunc(v2.pos, v0.pos, p.pos);
	//float w2 = MathSet::EdgeFunc(v0.pos, v1.pos, p.pos);
	Vw.x = MathSet::edgeFunc(v1.pos, v2.pos, p.pos) * v0.pos.w / Vw.w;//lambda0/z0
	Vw.y = MathSet::edgeFunc(v2.pos, v0.pos, p.pos) * v1.pos.w / Vw.w;//lambda1/z1
	Vw.z = MathSet::edgeFunc(v0.pos, v1.pos, p.pos) * v2.pos.w / Vw.w;//lambda2/z2

	return (Vw.x >= 0 && Vw.y >= 0 && Vw.z >= 0);
	return false;
}

bool Device::backFaceCulling(const Vector4& p0, const Vector4& p1, const Vector4& p2)
{
	return ((p0 * Vector4(multiplicationCross((p2 - p0), (p1 - p0)))) > 0);
}

void Device::fillTriangleVertexColor(Vertex& v0, Vertex& v1, Vertex& v2)
{
	//获得三角形包围盒
	float xmax = MathSet::max3(v0.pos.x, v1.pos.x, v2.pos.x);
	float ymax = MathSet::max3(v0.pos.y, v1.pos.y, v2.pos.y);
	float xmin = MathSet::min3(v0.pos.x, v1.pos.x, v2.pos.x);
	float ymin = MathSet::min3(v0.pos.y, v1.pos.y, v2.pos.y);

	int x0 = std::max<>(0, (int)std::floor(xmin));
	int y0 = std::max<>(0, (int)std::floor(ymin));
	int x1 = std::min<>(width - 1, (int)std::floor(xmax));
	int y1 = std::min<>(height - 1, (int)std::floor(ymax));

	Vector4 Vweight = { 0,0,0,MathSet::edgeFunc(v0.pos,v1.pos,v2.pos) };

	if (optimized)
	{
		//关于edgefunction优化算法
		//ref:http://www.scratchapixel.com/lessons/3d-basic-rendering/rasterization-practical-implementation/rasterization-practical-implementation
		//这里只进行了部分优化，循环中还是带有2个乘法，FPS比优化前略有提升
		float w0base = (v2.pos.x - v1.pos.x) * (y0 + 0.5f - v1.pos.y) - (v2.pos.y - v1.pos.y) * (x0 + 0.5f - v1.pos.x);
		float w1base = (v0.pos.x - v2.pos.x) * (y0 + 0.5f - v2.pos.y) - (v0.pos.y - v2.pos.y) * (x0 + 0.5f - v2.pos.x);
		float w2base = (v1.pos.x - v0.pos.x) * (y0 + 0.5f - v0.pos.y) - (v1.pos.y - v0.pos.y) * (x0 + 0.5f - v0.pos.x);
		float w0stepX = -(v2.pos.y - v1.pos.y);
		float w0stepY = (v2.pos.x - v1.pos.x);
		float w1stepX = -(v0.pos.y - v2.pos.y);
		float w1stepY = (v0.pos.x - v2.pos.x);
		float w2stepX = -(v1.pos.y - v0.pos.y);
		float w2stepY = (v1.pos.x - v0.pos.x);
		for (int x = x0, sx = 0; x <= x1; x++, sx++)
		{
			for (int y = y0, sy = 0; y <= y1; y++, sy++)
			{
				Vertex samplePixel = { {x + 0.5f,y + 0.5f,0.0f,0.0f},{} };

				float w0 = w0base + w0stepX * sx + w0stepY * sy;
				float w1 = w1base + w1stepX * sx + w1stepY * sy;
				float w2 = w2base + w2stepX * sx + w2stepY * sy;

				if (!(w0 >= 0 && w1 >= 0 && w2 >= 0))//跳过外部点
				{
					continue;
				}

				Vweight.x = w0 * v0.pos.w / Vweight.w;
				Vweight.y = w1 * v1.pos.w / Vweight.w;
				Vweight.z = w2 * v2.pos.w / Vweight.w;

				Interpolate(v0, v1, v2, samplePixel, Vweight);

				drawPoint(x, y, MathSet::color2UINT(samplePixel.color), samplePixel.pos.z);
			}
		}
	}
	else
	{
		for (int x = x0; x <= x1; x++)
		{
			for (int y = y0; y <= y1; y++)
			{
				Vertex samplePixel = { {x + 0.5f,y + 0.5f,0.0f,0.0f},{} };
				if (!triangleCheck(v0, v1, v2, samplePixel, Vweight))
				{
					continue;
				}
				Interpolate(v0, v1, v2, samplePixel, Vweight);
				drawPoint(x, y, MathSet::color2UINT(samplePixel.color), samplePixel.pos.z);
			}//endfor
		}
	}//endelse
}

void Device::fillTriangleTexture(Vertex& v0, Vertex& v1, Vertex& v2)
{
	//get the triangle bounding box in float
	float xmax = MathSet::max3(v0.pos.x, v1.pos.x, v2.pos.x);
	float ymax = MathSet::max3(v0.pos.y, v1.pos.y, v2.pos.y);
	float xmin = MathSet::min3(v0.pos.x, v1.pos.x, v2.pos.x);
	float ymin = MathSet::min3(v0.pos.y, v1.pos.y, v2.pos.y);
	//to int
	int x0 = std::max<>(0, (int)std::floor(xmin));
	int y0 = std::max<>(0, (int)std::floor(ymin));
	int x1 = std::min<>(width - 1, (int)std::floor(xmax));
	int y1 = std::min<>(height - 1, (int)std::floor(ymax));
	//weight stored in vector
	Vector4 Vweight = { 0,0,0,MathSet::edgeFunc(v0.pos, v1.pos, v2.pos) };
	float w0base = (v2.pos.x - v1.pos.x) * (y0 + 0.5f - v1.pos.y) - (v2.pos.y - v1.pos.y) * (x0 + 0.5f - v1.pos.x);
	float w1base = (v0.pos.x - v2.pos.x) * (y0 + 0.5f - v2.pos.y) - (v0.pos.y - v2.pos.y) * (x0 + 0.5f - v2.pos.x);
	float w2base = (v1.pos.x - v0.pos.x) * (y0 + 0.5f - v0.pos.y) - (v1.pos.y - v0.pos.y) * (x0 + 0.5f - v0.pos.x);
	float  w0_stepX = -(v2.pos.y - v1.pos.y);
	float  w0_stepY = (v2.pos.x - v1.pos.x);
	float  w1_stepX = -(v0.pos.y - v2.pos.y);
	float  w1_stepY = (v0.pos.x - v2.pos.x);
	float  w2_stepX = -(v1.pos.y - v0.pos.y);
	float  w2_stepY = (v1.pos.x - v0.pos.x);
	//start 
	for (int x = x0, sx = 0; x <= x1; x++, sx++)
		for (int y = y0, sy = 0; y <= y1; y++, sy++)
		{
			Vertex SamplePixel = { {x + 0.5f ,y + 0.5f ,0.f,0.f},{} }; //采样点初值,颜色,UV坐标通过插值获取

			float w0 = w0base + w0_stepX * sx + w0_stepY * sy;
			float w1 = w1base + w1_stepX * sx + w1_stepY * sy;
			float w2 = w2base + w2_stepX * sx + w2_stepY * sy;

			if (!(w0 >= 0 && w1 >= 0 && w2 >= 0))//skip  pixel outside
				continue;
			Vweight.x = w0 * v0.pos.w / Vweight.w;//pos.w=1/z,查看NDC2Screen
			Vweight.y = w1 * v1.pos.w / Vweight.w;
			Vweight.z = w2 * v2.pos.w / Vweight.w;//存储 lambda/z 至Vweight
			//透视校正插值
			Interpolate(v0, v1, v2, SamplePixel, Vweight);
			//
			drawPoint(x, y, pTexture->SampColor(SamplePixel.uv), SamplePixel.pos.z);
		}
}

void Device::Rasterize(Vertex& v0, Vertex& v1, Vertex& v2)
{
	if (renderMode == RenderMode::RenderStateWireFrame)
	{
		drawLine(v0.pos, v1.pos, ColorsL::White);
		drawLine(v0.pos, v2.pos, ColorsL::White);
		drawLine(v1.pos, v2.pos, ColorsL::White);
	}
	else if (renderMode == RenderMode::RenderStateColor)
	{
		fillTriangleVertexColor(v0, v1, v2);
	}
	else
	{
		fillTriangleTexture(v0, v1, v2);
	}
}
