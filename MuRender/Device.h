#pragma once
#include "MathUnion.h"
#include "Texture.h"
#include "Mesh.h"
#include "Model.h"
#include "Vertex.h"

enum class RenderMode
{
    RenderStateWireFrame = 0,
    RenderStateColor = 1,
    RenderStateTexture = 2
};

/**
 * \brief  Device class
 */
class Device
{
public:
    // 
    int width;
    int height;
    // 渲染模式
    RenderMode renderMode;
    unsigned int* pointFrameBuffer;
    float* zBuffer;
    Matrix mWorld;
    Matrix mView;
    Matrix mProj;
    Matrix mMvp;
    Matrix mScale;
    float mTheta;
    float mPhi;
    float mRadius;
    float mZoom;
    unsigned int optimized;
    Model* pModel;
    Texture* pTexture;
    POINT lastMousePos;

    Device();
    ~Device();
    
public:
    void Initialize(int W, int H, void* FrameBuffer);

    void Destroy();

    void ClearBuffer(const MuVector& color);

    void Update();

    void SetCameraPos(const MuVector& look, const MuVector& at);

    void SetFrustum(float fov, float ratio, float n, float f);

    void SetMvp();

    void DrawPrimitive(Mesh* mesh);

    bool Clip(MuVector& pos);

    void NDCToScreen(Vertex& Vertexout);

    void Interpolate(const Vertex& v0, const Vertex& v1, const Vertex& v2, Vertex& pixelSamp, const MuVector& vw);

    void DrawPoint(int x, int y, const unsigned int color, const float z);

    void DrawLine(int x0, int y0, int x1, int y1, const MuVector& color);
    void DrawLine(const MuVector& p0, const MuVector& p1, const MuVector& color);

    bool TriangleCheck(const Vertex& v0, const Vertex& v1, const Vertex& v2, Vertex& p, MuVector& Vw);

    bool BackFaceCulling(const MuVector& p0, const MuVector& p1, const MuVector& p2);

    void FillTriangleVertexColor(Vertex& v0, Vertex& v1, Vertex& v2);

    void FillTriangleTexture(Vertex& v0, Vertex& v1, Vertex& v2);

    void Rasterize(Vertex& v0, Vertex& v1, Vertex& v2);
};
