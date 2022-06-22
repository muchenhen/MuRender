#include "Device.h"

Device::~Device()
{
}

//��ʼ��
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

	mWorld.setUnitMatrix();//ת������������ϵ
	mView.setUnitMatrix();//ת�����ӽ�
	mProj.setUnitMatrix();//ͶӰ
	mScale.setUnitMatrix();//����

	//��ʼ�����λ����WM_Paint��Ϣdevice->update�����������λ���ṩ���˴������ṩ
	
	mZoom = 0.5f;//����ϵ����ʼֵ
	mScale = MathSet::scaleMatrix(mZoom, mZoom, mZoom);//�������ž���
	setFrustum(Pi / 4, (float)width / height, 1.0f, 1000.f);//����ͶӰ����
}

//����
void Device::destroy()
{
	if (pModel)
		delete pModel;
	if (pointFrameBuffer)
		delete pointFrameBuffer;
	if (zBuffer)
		delete[] zBuffer;
}

//�����Ļ
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

//״̬����
void Device::update()
{
	//��������ϵת�����ѿ�������ϵ
	float x = mRadius * sinf(mPhi) * cosf(mTheta);
	float z = mRadius * sinf(mPhi) * sinf(mTheta);
	float y = mRadius * cosf(mPhi);

	Vector4 eyePos(x, y, z, 1.0f);
	Vector4 at(0.0f, 0.0f, 0.0f, 1.0f);
	Vector4 up(0.0f, 1.0f, 0.0f, 0.0f);
	
	setCameraPos(eyePos, at);
}

//�������λ��
void Device::setCameraPos(const Vector4& look, const Vector4& at)
{
	mView = MathSet::MatrixLookAtLh(look, at, Vector4(0.0f, 1.0f, 0.0f, 0.0f));
}

//������׶��
void Device::setFrustum(float fov, float ratio, float n, float f)
{
	mProj = MathSet::MatrixPerspectiveFovLH(fov, ratio, n, f);
}

//���ñ任����
void Device::setMVP()
{
	mMvp = mWorld * mView * mProj * mScale;
}

//ͼԪ����
void Device::drawPrimitive(Mesh* mesh)
{
	Vertex v0, v1, v2;
	for (unsigned int i = 0; i < mesh->indexBuffer.size(); i += 3)
	{
		v0 = mesh->vertexBuffer[mesh->indexBuffer[i]];
		v1 = mesh->vertexBuffer[mesh->indexBuffer[i + 1]];
		v2 = mesh->vertexBuffer[mesh->indexBuffer[i + 2]];
		//�Զ������MVPͶӰ�任��ͶӰ֮��w����ԭ�е�zֵ
		v0.pos = MathSet::multVector4(v0.pos, mMvp);
		v1.pos = MathSet::multVector4(v1.pos, mMvp);
		v2.pos = MathSet::multVector4(v2.pos, mMvp);
		//�ж��Ƿ�ͨ���ü�
		if (clip(v0.pos) == false || clip(v1.pos) == false || clip(v2.pos) == false)
		{
			continue;
		}
		//�ж��Ƿ���Ҫ��������
		/*
		������߿�ģʽ�Ͳ���Ҫ��������
		*/
		if (renderMode == RenderMode::RenderStateWireFrame)
		{
			if (backFaceCulling(v0.pos, v1.pos, v2.pos))
			{
				continue;
			}
		}
		//����͸�ӳ���ת����NDC�ռ䣬�����ӿ�ӳ�䣬׼����դ��
		ndc2Screen(v0);
		ndc2Screen(v1);
		ndc2Screen(v2);
		//��դ��
		Rasterize(v0, v1, v2);
	}
}

/*
�ü� ����(-w,w) ����λ����βü��ռ�
����͸�ӱ任��ԭzֵ��w�� z������Ϊ[0,1]
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

//͸�ӳ������ӿ�ӳ��
/*
͸�ӳ���/w,�Ӳü��ռ�ת��NDC(�豸�淶���ռ�)
NDC���ӿڱ任,ӳ��X,Y��������[-1��1]����Ļ[m_width,m_height],UV����
*/
void Device::ndc2Screen(Vertex& vertexout)
{
	float reciprecalw = 1.0f / vertexout.pos.w;//͸�ӱ任֮��zֵ������w
	//ӳ�䵽��Ļ
	vertexout.pos.x = (vertexout.pos.x * reciprecalw + 1.0f) * 0.5f * width;
	vertexout.pos.y = (vertexout.pos.y * reciprecalw + 1.0f) * 0.5f * height;
	vertexout.pos.z = vertexout.pos.w;//z=w
	vertexout.pos.w = reciprecalw;//�൱��1/z�洢����w
}

//͸�ӽ�����ֵ
void Device::Interpolate(const Vertex& v0, const Vertex& v1, const Vertex& v2, Vertex& pixelSamp, const Vector4& vw)
{
	/*
	���������z���꣬��Ϊ��ֵϵ��
	��ʽ �� 1 / p.z = lambda0 / v0.z + lambda1 / v1.z + lambda2 / v2.z
	http://www.scratchapixel.com/lessons/3d-basic-rendering/rasterization-practical-implementation/visibility-problem-depth-buffer-depth-interpolation
	*/
	pixelSamp.pos.z = 1.0f / (vw.x + vw.y + vw.z);
	//XY������ѭ���л�ȡ����ȥpos��ֵ
	//
	if (renderMode == RenderMode::RenderStateColor)
	{
		pixelSamp.color = (v0.color * vw.x + v1.color * vw.y + v2.color * vw.z) * pixelSamp.pos.z;
	}
	else
	{
		pixelSamp.uv = (v0.uv * vw.x + v1.uv * vw.y + v2.uv * vw.z) * pixelSamp.pos.z;//�����ֵ
	}
}