#include "FragmentShader.h"

#include "Logger.h"

// #include "Logger.h"

FragmentShader DefaultFragmentShader = [](const FragmentShaderInput& Input, const Material* MaterialPtr)
{
    Eigen::Vector3f Color = Eigen::Vector3f(1, 1, 1);
    if (MaterialPtr != nullptr)
    {
        Color = MaterialPtr->GetBaseColor();
        if (MaterialPtr->HasTexture())
        {
            Color = Color.cwiseProduct(MaterialPtr->SampleTexture(Input.UV));
        }
    }
    return Eigen::Vector4f(Color.x(), Color.y(), Color.z(), 1);
};

SimpleLitFragmentShader DefaultSimpleLitFragmentShader = [](const FragmentShaderInput& Input, const Material* MaterialPtr, const DirectionalLight* LightPtr)
{
    Eigen::Vector3f Color = Eigen::Vector3f(1, 1, 1);
    if (MaterialPtr != nullptr)
    {
        Color = MaterialPtr->GetBaseColor();
        if (MaterialPtr->HasTexture())
        {
            Color = Color.cwiseProduct(MaterialPtr->SampleTexture(Input.UV));
        }
    }

    // Ambient
    float AmbientStrength = 0.1f;
    Eigen::Vector3f AmbientColor = AmbientStrength * Color;

    Eigen::Vector3f LightDir = LightPtr->Direction;

    // Half Lambert
    float NdotL = -Input.WorldNormal.dot(LightDir);
    float HalfLambert = 0.5f * NdotL + 0.5f;
    // HalfLambert = HalfLambert * HalfLambert; 

    // Diffuse
    Eigen::Vector3f LightColor = LightPtr->Color;
    float LightIntensity = LightPtr->Intensity;

    // float DiffuseFactor = std::max(0.0f, Input.WorldNormal.dot(-LightDir));

    Eigen::Vector3f DiffuseColor = HalfLambert * LightColor * LightIntensity;

    Eigen::Vector3f LightingColor = AmbientColor + DiffuseColor;

    Eigen::Vector3f FinalColor = Color.cwiseProduct(LightingColor);

    FinalColor = FinalColor.cwiseMin(1.0f).cwiseMax(0.0f);
    return Eigen::Vector4f(FinalColor.x(), FinalColor.y(), FinalColor.z(), 1);
};

bool isPrinted = false;

ShadowMapFragmentShader DefaultShadowMapFragmentShader = [](const FragmentShaderInput& Input,
                                                            const Material* MaterialPtr,
                                                            const DirectionalLight* LightPtr,
                                                            const DepthTexture* ShadowMap,
                                                            const Eigen::Matrix4f& LightSpaceMatrix)
{
    // 基本颜色计算
    Eigen::Vector3f Color = Eigen::Vector3f(1, 1, 1);
    if (MaterialPtr != nullptr)
    {
        Color = MaterialPtr->GetBaseColor();
        if (MaterialPtr->HasTexture())
        {
            Color = Color.cwiseProduct(MaterialPtr->SampleTexture(Input.UV));
        }
    }

    // 环境光
    float AmbientStrength = 0.1f;
    Eigen::Vector3f AmbientColor = AmbientStrength * Color;

    Eigen::Vector3f LightDir = LightPtr->Direction.normalized();

    // Half Lambert
    float NdotL = Input.WorldNormal.dot(LightDir); // 注意：移除了负号
    float HalfLambert = 0.5f * NdotL + 0.5f;

    // 漫反射
    Eigen::Vector3f LightColor = LightPtr->Color;
    float LightIntensity = LightPtr->Intensity;
    Eigen::Vector3f DiffuseColor = HalfLambert * LightColor * LightIntensity;

    // 阴影计算
    // 计算当前片元在光源空间的位置
    Eigen::Vector4f LightSpacePos = LightSpaceMatrix * Eigen::Vector4f(Input.WorldPosition.x(), Input.WorldPosition.y(), Input.WorldPosition.z(), 1.0f);
    // 透视除法
    Eigen::Vector3f ProjCoords = LightSpacePos.head<3>() / LightSpacePos.w();

    // 转换到[0,1]范围 用于采样深度贴图
    ProjCoords.x() = ProjCoords.x() * 0.5f + 0.5f;
    ProjCoords.y() = ProjCoords.y() * 0.5f + 0.5f;
    ProjCoords.z() = ProjCoords.z() * 0.5f + 0.5f;

    // 获取最近深度（阴影贴图中的深度，距离光源最近的深度）
    float ClosestDepth = ShadowMap->SampleDepth(ProjCoords.x(), ProjCoords.y());
    // 获取当前深度，当前片元转换到光源空间对应的深度
    float CurrentDepth = ProjCoords.z();

    // 阴影因子 (调整比较逻辑)
    float bias = 0.5f; // 可以根据需要调整这个偏移值
    float ShadowFactor = (CurrentDepth - bias) < ClosestDepth ? 1.0f : 0.5f;

    // 最终颜色计算
    Eigen::Vector3f LightingColor = AmbientColor + ShadowFactor * DiffuseColor;
    Eigen::Vector3f FinalColor = Color.cwiseProduct(LightingColor);

    FinalColor = FinalColor.cwiseMin(1.0f).cwiseMax(0.0f);
    return Eigen::Vector4f(FinalColor.x(), FinalColor.y(), FinalColor.z(), 1.0f);
};