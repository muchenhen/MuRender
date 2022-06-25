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

class Device
{
public:
    int width;
    int height;
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

    Device() = default;
    ~Device();

    void Initialize(int W, int H, void* FrameBuffer);

    void destroy();

    void clearBuffer(const MuVector& color);

    void update();

    void setCameraPos(const MuVector& look, const MuVector& at);

    void setFrustum(float fov, float ratio, float n, float f);

    void setMVP();

    void drawPrimitive(Mesh* mesh);

    bool clip(MuVector& pos);

    void ndc2Screen(Vertex& vertexout);

    void Interpolate(const Vertex& v0, const Vertex& v1, const Vertex& v2, Vertex& pixelSamp, const MuVector& vw);

    void drawPoint(int x, int y, const unsigned int color, const float z);

    void drawLine(int x0, int y0, int x1, int y1, const MuVector& color);
    void drawLine(const MuVector& p0, const MuVector& p1, const MuVector& color);

    bool triangleCheck(const Vertex& v0, const Vertex& v1, const Vertex& v2, Vertex& p, MuVector& Vw);

    bool backFaceCulling(const MuVector& p0, const MuVector& p1, const MuVector& p2);

    void fillTriangleVertexColor(Vertex& v0, Vertex& v1, Vertex& v2);

    void fillTriangleTexture(Vertex& v0, Vertex& v1, Vertex& v2);

    void Rasterize(Vertex& v0, Vertex& v1, Vertex& v2);
};
