#include "Device.h"

Device::~Device()
{
}

//初始化
void Device::Initialize(const int W, const int H, void* FrameBuffer)
{
	width = W;
	height = H;

	pointFrameBuffer = static_cast<unsigned int*>(FrameBuffer);

	zBuffer = new float[height * width];
	for (int i = 0; i < height * width; i++)
	{
		zBuffer[i] = 1000.f;
	}

	renderMode = RenderMode::RenderStateWireFrame;

	mTheta = 1.5f * Pi;
	mPhi = 0.4f * Pi;
	mRadius = 5.0f;

	optimized = 1;
	
	pModel = new Model();
	
	pTexture = new Texture();

	mWorld.setUnitMatrix();//转换到世界坐标系
	mView.setUnitMatrix();//转换到视角
	mProj.setUnitMatrix();//投影
	mScale.setUnitMatrix();//缩放

	//初始的相机位置由WM_Paint消息device->update函数根据鼠标位置提供，此处无需提供
	
	mZoom = 0.5f;//缩放系数初始值
	mScale = MathSet::scaleMatrix(mZoom, mZoom, mZoom);//设置缩放矩阵
	setFrustum(Pi / 4, (float)width / height, 1.0f, 1000.f);//设置投影矩阵
}

//销毁
void Device::destroy()
{
	if (pModel)
		delete pModel;
	if (pointFrameBuffer)
		delete pointFrameBuffer;
	if (zBuffer)
		delete[] zBuffer;
}

//清空屏幕
void Device::clearBuffer(const Vector4& color)
{
	for (int i = 0; i < width; ++i)
	{
		for (int j = 0; j < height; ++j)
		{
			pointFrameBuffer[width * j + i] = MathSet::color2UINT(color);
			zBuffer[width * j + i] = 1000.0f;
		}
	}
}

//状态更新
void Device::update()
{
	//球面坐标系转换到笛卡尔坐标系
	float x = mRadius * sinf(mPhi) * cosf(mTheta);
	float z = mRadius * sinf(mPhi) * sinf(mTheta);
	float y = mRadius * cosf(mPhi);

	Vector4 eyePos(x, y, z, 1.0f);
	Vector4 at(0.0f, 0.0f, 0.0f, 1.0f);
	Vector4 up(0.0f, 1.0f, 0.0f, 0.0f);
	
	setCameraPos(eyePos, at);
}

//设置相机位置
void Device::setCameraPos(const Vector4& look, const Vector4& at)
{
	mView = MathSet::MatrixLookAtLh(look, at, Vector4(0.0f, 1.0f, 0.0f, 0.0f));
}

//设置视锥体
void Device::setFrustum(float fov, float ratio, float n, float f)
{
	mProj = MathSet::MatrixPerspectiveFovLH(fov, ratio, n, f);
}

//设置变换矩阵
void Device::setMVP()
{
	mMvp = mWorld * mView * mProj * mScale;
}

//图元绘制
void Device::drawPrimitive(Mesh* mesh)
{
	Vertex v0, v1, v2;
	for (unsigned int i = 0; i < mesh->indexBuffer.size(); i += 3)
	{
		v0 = mesh->vertexBuffer[mesh->indexBuffer[i]];
		v1 = mesh->vertexBuffer[mesh->indexBuffer[i + 1]];
		v2 = mesh->vertexBuffer[mesh->indexBuffer[i + 2]];
		//对顶点进行MVP投影变换，投影之后w保存原有的z值
		v0.pos = MathSet::multVector4(v0.pos, mMvp);
		v1.pos = MathSet::multVector4(v1.pos, mMvp);
		v2.pos = MathSet::multVector4(v2.pos, mMvp);
		//判断是否通过裁剪
		if (clip(v0.pos) == false || clip(v1.pos) == false || clip(v2.pos) == false)
		{
			continue;
		}
		//判断是否需要背面消隐
		/*
		如果是线框模式就不需要背面消隐
		*/
		if (renderMode == RenderMode::RenderStateWireFrame)
		{
			if (backFaceCulling(v0.pos, v1.pos, v2.pos))
			{
				continue;
			}
		}
		//进行透视除法转换到NDC空间，进行视口映射，准备光栅化
		ndc2Screen(v0);
		ndc2Screen(v1);
		ndc2Screen(v2);
		//光栅化
		Rasterize(v0, v1, v2);
	}
}

/*
裁剪 区间(-w,w) 顶点位于齐次裁剪空间
进行透视变换后原z值在w中 z的区间为[0,1]
*/
bool Device::clip(Vector4& pos)
{
	if (pos.x >= -pos.w && pos.x <= pos.w &&
		pos.y >= -pos.w && pos.y <= pos.w &&
		pos.z >= 0.f && pos.z <= pos.w)
	{
		return true;
	}
	return false;
}

//透视除法和视口映射
/*
透视除法/w,从裁剪空间转至NDC(设备规范化空间)
NDC到视口变换,映射X,Y坐标区间[-1，1]至屏幕[m_width,m_height],UV不变
*/
void Device::ndc2Screen(Vertex& vertexout)
{
	float reciprecalw = 1.0f / vertexout.pos.w;//透视变换之后z值保留在w
	//映射到屏幕
	vertexout.pos.x = (vertexout.pos.x * reciprecalw + 1.0f) * 0.5f * width;
	vertexout.pos.y = (vertexout.pos.y * reciprecalw + 1.0f) * 0.5f * height;
	vertexout.pos.z = vertexout.pos.w;//z=w
	vertexout.pos.w = reciprecalw;//相当于1/z存储在了w
}

//透视矫正插值
void Device::Interpolate(const Vertex& v0, const Vertex& v1, const Vertex& v2, Vertex& pixelSamp, const Vector4& vw)
{
	/*
	求出样本的z坐标，作为插值系数
	公式 ： 1 / p.z = lambda0 / v0.z + lambda1 / v1.z + lambda2 / v2.z
	http://www.scratchapixel.com/lessons/3d-basic-rendering/rasterization-practical-implementation/visibility-problem-depth-buffer-depth-interpolation
	*/
	pixelSamp.pos.z = 1.0f / (vw.x + vw.y + vw.z);
	//XY坐标由循环中获取，略去pos插值
	//
	if (renderMode == RenderMode::RenderStateColor)
	{
		pixelSamp.color = (v0.color * vw.x + v1.color * vw.y + v2.color * vw.z) * pixelSamp.pos.z;
	}
	else
	{
		pixelSamp.uv = (v0.uv * vw.x + v1.uv * vw.y + v2.uv * vw.z) * pixelSamp.pos.z;//纹理插值
	}
}