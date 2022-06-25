#include "Device.h"

Device::~Device()
{
}

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

    mTheta = 1.5f * PI;
    mPhi = 0.4f * PI;
    mRadius = 5.0f;

    optimized = 1;

    pModel = new Model();

    pTexture = new Texture();

    mWorld.setUnitMatrix();
    mView.setUnitMatrix();
    mProj.setUnitMatrix();
    mScale.setUnitMatrix();

    mZoom = 0.5f;
    mScale = MathSet::scaleMatrix(mZoom, mZoom, mZoom);
    setFrustum(PI / 4, (float)width / height, 1.0f, 1000.f);
}

void Device::destroy()
{
    if (pModel)
        delete pModel;
    if (pointFrameBuffer)
        delete pointFrameBuffer;
    if (zBuffer)
        delete[] zBuffer;
}

void Device::clearBuffer(const MuVector& color)
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

void Device::update()
{
    float x = mRadius * sinf(mPhi) * cosf(mTheta);
    float z = mRadius * sinf(mPhi) * sinf(mTheta);
    float y = mRadius * cosf(mPhi);

    MuVector eyePos(x, y, z, 1.0f);
    MuVector at(0.0f, 0.0f, 0.0f, 1.0f);
    MuVector up(0.0f, 1.0f, 0.0f, 0.0f);

    setCameraPos(eyePos, at);
}

void Device::setCameraPos(const MuVector& look, const MuVector& at)
{
    mView = MathSet::MatrixLookAtLh(look, at, MuVector(0.0f, 1.0f, 0.0f, 0.0f));
}

void Device::setFrustum(float fov, float ratio, float n, float f)
{
    mProj = MathSet::MatrixPerspectiveFovLH(fov, ratio, n, f);
}

void Device::setMVP()
{
    mMvp = mWorld * mView * mProj * mScale;
}

void Device::drawPrimitive(Mesh* mesh)
{
    Vertex v0, v1, v2;
    for (unsigned int i = 0; i < mesh->indexBuffer.size(); i += 3)
    {
        v0 = mesh->vertexBuffer[mesh->indexBuffer[i]];
        v1 = mesh->vertexBuffer[mesh->indexBuffer[i + 1]];
        v2 = mesh->vertexBuffer[mesh->indexBuffer[i + 2]];
        v0.pos = MathSet::multVector4(v0.pos, mMvp);
        v1.pos = MathSet::multVector4(v1.pos, mMvp);
        v2.pos = MathSet::multVector4(v2.pos, mMvp);
        if (clip(v0.pos) == false || clip(v1.pos) == false || clip(v2.pos) == false)
        {
            continue;
        }
        if (renderMode == RenderMode::RenderStateWireFrame)
        {
            if (backFaceCulling(v0.pos, v1.pos, v2.pos))
            {
                continue;
            }
        }
        ndc2Screen(v0);
        ndc2Screen(v1);
        ndc2Screen(v2);
        Rasterize(v0, v1, v2);
    }
}

bool Device::clip(MuVector& pos)
{
    if (pos.X >= -pos.W && pos.X <= pos.W && pos.Y >= -pos.W && pos.Y <= pos.W && pos.Z >= 0.f && pos.Z <= pos.W)
    {
        return true;
    }
    return false;
}

void Device::ndc2Screen(Vertex& vertexout)
{
    float reciprecalw = 1.0f / vertexout.pos.W;
    vertexout.pos.X = (vertexout.pos.X * reciprecalw + 1.0f) * 0.5f * width;
    vertexout.pos.Y = (vertexout.pos.Y * reciprecalw + 1.0f) * 0.5f * height;
    vertexout.pos.Z = vertexout.pos.W;
    vertexout.pos.W = reciprecalw;
}

void Device::Interpolate(const Vertex& v0, const Vertex& v1, const Vertex& v2, Vertex& pixelSamp, const MuVector& vw)
{
    pixelSamp.pos.Z = 1.0f / (vw.X + vw.Y + vw.Z);
    if (renderMode == RenderMode::RenderStateColor)
    {
        pixelSamp.color = (v0.color * vw.X + v1.color * vw.Y + v2.color * vw.Z) * pixelSamp.pos.Z;
    }
    else
    {
        pixelSamp.uv = (v0.uv * vw.X + v1.uv * vw.Y + v2.uv * vw.Z) * pixelSamp.pos.Z;
    }
}