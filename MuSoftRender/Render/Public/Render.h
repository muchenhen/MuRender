#pragma once

#include <vector>
#include <cstdint>

#include "Camera.h"
#include "DepthTexture.h"
#include "FragmentShader.h"
#include "MeshObject.h"
#include "RenderPipeline.h"
#include "Scene.h"
#include "VertexShader.h"



class Renderer
{
private:
    int ScreenWidth;
    int ScreenHeight;
    std::vector<uint32_t> FrameBuffer;
    std::vector<float> DepthBuffer;

    bool UseEarlyDepthTest = true;

public:
    Renderer(int Width, int Height);

    void Clear(uint32_t Color = 0);

    const std::vector<uint32_t>& GetFrameBuffer() const;

    int GetWidth() const;

    int GetHeight() const;

private:
    static Eigen::Vector3f ComputeBarycentric(const int X, const int Y, const float X0, const float Y0, const float X1, const float Y1, const float X2, const float Y2);

    static bool IsInsideTriangle(int X, int Y, const float X0, const float Y0, const float X1, const float Y1, const float X2, const float Y2);

    static uint32_t ColorToUint32(const Eigen::Vector4f& Color);

    static uint32_t ColorToUint32(const Eigen::Vector3f& Color);

public:
    void DrawPixel(int X, int Y, uint32_t Color);

    void DrawPixel(int X, int Y, float Depth, uint32_t Color);

    void DrawLine(int X1, int Y1, int X2, int Y2, uint32_t Color);

    void DrawLine(int X1, int Y1, float Z1, uint32_t Color1,
                  int X2, int Y2, float Z2, uint32_t Color2);

    void DrawTriangle(int X1, int Y1, int X2, int Y2, int X3, int Y3, uint32_t Color);

    void FillTriangle(int X1, int Y1, int X2, int Y2, int X3, int Y3, uint32_t Color);

    void FillTriangle(int X0, int Y0, float Z0, uint32_t Color0,
                      int X1, int Y1, float Z1, uint32_t Color1,
                      int X2, int Y2, float Z2, uint32_t Color2);

    void DrawTexture(const Texture* Texture);

private:
    void ProcessTriangle(const Vertex& V1, const Vertex& V2, const Vertex& V3,
                         const Eigen::Matrix4f& ModelMatrix, const Eigen::Matrix4f& MVPMatrix,
                         const StandardVertexShader& VS, const FragmentShader& FS,
                         const Material* Material);

    void ProcessTriangle(const Vertex& V1, const Vertex& V2, const Vertex& V3,
                         const Eigen::Matrix4f& ModelMatrix, const Eigen::Matrix4f& MVPMatrix, const Eigen::Matrix3f& NormalMatrix,
                         const NormalVertexShader& VS, const SimpleLitFragmentShader& FS,
                         const Material* Material, const DirectionalLight* DirectionalLightPtr);

    void RasterizeTriangle(const VertexShaderOutput& V1, const VertexShaderOutput& V2, const VertexShaderOutput& V3,
                           const FragmentShader& FS, const Material* Material);

    void RasterizeTriangle(const VertexShaderOutput& V1, const VertexShaderOutput& V2, const VertexShaderOutput& V3,
                           const SimpleLitFragmentShader& FS, const Material* Material, const DirectionalLight* DirectionalLightPtr);

    void RasterizeTriangle(const VertexShaderOutput& V1, const VertexShaderOutput& V2, const VertexShaderOutput& V3,
                       const ShadowMapFragmentShader& FS, const Material* Material, const DirectionalLight* DirectionalLightPtr,
                       const DepthTexture* ShadowMap, const Eigen::Matrix4f& LightSpaceMatrix);


public:
    void RenderCamera(const Scene& Scene, const Camera& Camera);

    void RenderScene(const Scene* Scene, const Camera* Camera, const RenderPipeline* Pipeline);

    void RenderScene(Scene* Scene, const Camera* Camera, const NormalRenderPipeline* Pipeline);

    void RenderMeshObject(const MeshObject* MeshObject, const Camera* Camera, const RenderPipeline* Pipeline);

    void RenderMeshObject(const MeshObject* MeshObject, const Camera* Camera, const NormalRenderPipeline* Pipeline, const DirectionalLight* DirectionalLightPtr);

    void RenderMeshObject(const MeshObject* MeshObject, const Camera* Camera, const NormalRenderPipeline* Pipeline,
                          const DirectionalLight* DirectionalLightPtr,
                          const DepthTexture* ShadowMap,
                          const Eigen::Matrix4f& LightSpaceMatrix);

private:
    bool EarlyDepthTest(int X, int Y, float Depth);

public:
    void SetUseEarlyDepthTest(bool Enable);

    // 渲染阴影贴图
    void RenderShadowMap(const Scene* Scene, DepthTexture* DepthTexture, float DepthBias, DirectionalLight* DirectionalLightPtr, const BoundingBox& SceneBoundingBox);

    void RenderObjectDepth(const MeshObject* MeshObject, DepthTexture* DepthTexture, float DepthBias, const Eigen::Matrix4f& LightSpaceMVP);

    void RasterizeTriangleDepth(const Eigen::Vector3f& V1, const Eigen::Vector3f& V2, const Eigen::Vector3f& V3, DepthTexture* DepthTexturePtr);


private:
    Eigen::Matrix4f Orthographic(const float Left, const float Right, const float Bottom, const float Top, const float Near, const float Far);

    Eigen::Matrix4f LookAt(const Eigen::Vector3f& Eye, const Eigen::Vector3f& Center, const Eigen::Vector3f& Up);

public:
    void DrawBoundingBox(const BoundingBox& bbox, const Eigen::Matrix4f& viewProjectionMatrix, uint32_t color);
};